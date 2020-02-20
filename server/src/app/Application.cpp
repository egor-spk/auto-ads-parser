#include "include/Application.h"
#include "../parser/include/IWebPageInfoParser.h"
#include "../parser/include/IWebSiteTransport.h"
#include "../parser/include/CurlTransport.h"
#include "../parser/include/WebSiteParser.h"
#include "../parser/include/AutoruPageInfoParser.h"
#include "../parser/include/AvitoPageInfoParser.h"
#include "include/SignalHandler.h"

#include <future>
#include <restinio/all.hpp>
#include <restinio/transforms/zlib.hpp>

using router_t = restinio::router::express_router_t<>;
using server_traits_t = restinio::traits_t<
        restinio::asio_timer_manager_t,
        restinio::null_logger_t,
        router_t>;

namespace
{
    /**
     * @brief Вспомогательный функтор для запуска процесса парсинга в потоке
     * @tparam T IWebPageInfoParser
     */
    template<typename T>
    class ParserRunner
    {
        static_assert(std::is_base_of<parser::IWebPageInfoParser, T>::value,
                      "T should be derived from IWebPageInfoParser");
    public:
        ParserRunner(std::string name, std::string link) :
                name_{std::move(name)},
                link_{std::move(link)},
                parser_{std::make_shared<parser::WebSiteParser>(std::make_unique<parser::CurlTransport>(),
                                                                std::make_unique<T>(),
                                                                link_)} {}

        ParserRunner(std::string name,
                     std::string link,
                     const std::unordered_map<std::string, std::string> &cookie) :
                name_{std::move(name)},
                link_{std::move(link)},
                parser_{std::make_shared<parser::WebSiteParser>(std::make_unique<parser::CurlTransport>(cookie),
                                                                std::make_unique<T>(),
                                                                link_)} {}

        nlohmann::json operator()()
        {
            try
            {
                using namespace parser;
                parser_->parse();
                size_t countResult = parser_->countResult();
                LOG_INFO("{}: successfully parse {} ads", name_, countResult);

                if (countResult)
                {
                    auto jsRes = nlohmann::json::array();
                    const auto res = parser_->getResult();

                    jsRes.get_ptr<nlohmann::json::array_t *>()->reserve(countResult);
                    std::transform(res.cbegin(), res.cend(), std::back_inserter(jsRes), [](auto &&ad) { return ad; });
                    return jsRes;
                }
            }
            catch (const parser::ParseError &e)
            {
                LOG_ERROR("{} parsing error: {}", name_, e.what());
            }
            catch (const parser::WebSiteTransportError &e)
            {
                LOG_ERROR("{} fetch error: {}", name_, e.what());
            }

            return {};
        }

        std::shared_ptr<parser::IParser> getParser() const noexcept { return parser_; };

        void clean() noexcept { parser_->clean(); };
    private:
        std::string name_;
        std::string link_;
        std::shared_ptr<parser::IParser> parser_;
    };
}

Application::Application(app_config::IAppConfig &config) :
        config_(config)
{
    // инициализируем логгер
    initLogger(config.getLogPath(), config.getLogLevel());
    const auto &autoruLink = config.getAutoruLink();
    const auto &avitoLink = config.getAvitoLink();
    LOG_INFO("App start with links: {}{}",
             autoruLink ? fmt::format("\nautoru - \"{}\"", *autoruLink) : "",
             avitoLink ? fmt::format("\navito - \"{}\"", *avitoLink) : "");
}

Application::~Application()
{
    if (isWork)
        stopAll();
    parserThr_->join();
}

int Application::run()
{
    // запускаем поток парсинга
    parserThr_ = std::make_unique<std::thread>(&Application::startParsing, this);

    // запускаем http сервер
    auto router = std::make_unique<router_t>();
    router->http_get("/v1/:api_key/ad", [this](auto req, auto params)
    {
        // проверяем apikey
        static const auto configApiKey = config_.getApiKey();
        const auto apiKey = restinio::cast_to<std::string>(params["api_key"]);
        if (configApiKey != apiKey)
            req->create_response(restinio::status_forbidden())
                    .done();

        std::lock_guard<std::mutex> _{cacheMutex_};
        auto resp = req->create_response()
                .append_header(restinio::http_field::content_type, "application/json; charset=utf-8")
                .set_body(gzipAds_);
        auto ba = restinio::transforms::zlib::gzip_body_appender(resp);
        return resp.done();
    });
    restinio::http_server_t<server_traits_t> server{
            restinio::own_io_context(),
            [&config = config_, &router](auto &settings)
            {
                settings.port(config.getPort());
                settings.address("0.0.0.0");
                settings.request_handler(std::move(router));
            }};
    restinio::on_pool_runner_t<restinio::http_server_t<server_traits_t>> runner{
            1,
            server
    };
    runner.start();
    LOG_INFO("HTTP server started");

    // ожидаем сигнал от системы
    try
    {
        SignalHandler handler;
        handler.setSigintHandler(std::bind(&Application::stopAll, this));
        handler.setSigtermHandler(std::bind(&Application::stopAll, this));
        handler.wait();
    } catch (const SignalHandlerError &e)
    {
        LOG_CRITICAL("An error occurred while processing unix signals: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void Application::startParsing() noexcept
{
    LOG_TRACE("Start parser thread");

    // нужно ли запускать функцию парсинга?
    using namespace parser;
    const auto &avitoLink = config_.getAvitoLink();
    const auto &autoruLink = config_.getAutoruLink();
    if (!avitoLink && !autoruLink)
    {
        LOG_INFO("No parsing links specified. Parsing function is disabled");
        return;
    }

    while (isWork)
    {
        using namespace std::chrono_literals;
        LOG_TRACE("Start parsing");

        try
        {
            std::future<nlohmann::json> avito, autoru;
            {
                std::lock_guard<std::mutex> _{waitMutex_};
                // парсер avito
                if (avitoLink)
                {
                    ParserRunner<AvitoPageInfoParser> avitoParser{"Avito", *avitoLink};
                    parsers_.push_back(avitoParser.getParser());
                    avito = std::async(std::launch::async, std::move(avitoParser));
                    LOG_DEBUG("Start avito parser thread");
                }

                // парсер autoru
                if (autoruLink)
                {

                    ParserRunner<AutoruPageInfoParser> autoruParser{"Autoru", *autoruLink,
                                                                    {{"autoru_gdpr", "1"}}};
                    parsers_.push_back(autoruParser.getParser());
                    autoru = std::async(std::launch::async, std::move(autoruParser));
                    LOG_DEBUG("Start autoru parser thread");
                }
            }

            // достаем результаты
            bool isChange{false};
            if (autoru.valid())
            {
                LOG_TRACE("Get autoru parsing result");
                auto autoruRes = autoru.get();
                if (!autoruRes.empty())
                {
                    std::lock_guard<std::mutex> _{cacheMutex_};
                    cache_[AutoruCacheField] = std::move(autoruRes);
                    isChange = true;
                } else
                {
                    LOG_WARN("Autoru parser return empty result");
                }
            }

            // достаем результаты авито
            if (avito.valid())
            {
                LOG_TRACE("Get avito parsing result");
                auto avitoRes = avito.get();
                if (!avitoRes.empty())
                {
                    std::lock_guard<std::mutex> _{cacheMutex_};
                    cache_[AvitoCacheField] = std::move(avitoRes);
                    isChange = true;
                } else
                {
                    LOG_WARN("Avito parser return empty result");
                }
            }

            // сжимаем результат
            if (isChange)
            {
                std::lock_guard<std::mutex> _{cacheMutex_};
                gzipAds_ = restinio::transforms::zlib::gzip_compress(cache_.dump(), 9);
            }
        } catch (const std::exception &e)
        {
            LOG_ERROR("Uncaught exception: {}", e.what());
        }
        parsers_.clear();
        LOG_TRACE("End parsing");

        // ждем следующего раза
        if (isWork)
        {
            LOG_DEBUG("Waiting {} hours for next parsing", timeout.count());
            logger->flush();
            std::unique_lock<std::mutex> lock{waitMutex_};
            if (waitCv_.wait_for(lock, 6h, [&] { return !isWork; }))
            {
                LOG_DEBUG("Parser thread woke up from a signal");
            } else
            {
                LOG_DEBUG("Parser thread woke up in timeout");
            }
        }
    }

    LOG_TRACE("End parser thread");
}

void Application::stopAll() noexcept
{
    LOG_TRACE("Stopping application");

    {
        std::lock_guard<std::mutex> _{waitMutex_};
        for (const auto &parser : parsers_) { parser->stop(); }
        isWork = false;
    }
    waitCv_.notify_one();
}

