#include "include/Application.h"
#include "IWebPageInfoParser.h"
#include "IWebSiteTransport.h"
#include "CurlTransport.h"
#include "WebSiteParser.h"
#include "AutoruPageInfoParser.h"
#include "AvitoPageInfoParser.h"
#include "SignalHandler.h"
#include "ParserRunner.h"

#include <future>

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
            nlohmann::json res;
            if (autoru.valid())
            {
                LOG_TRACE("Get autoru parsing result");
                auto autoruRes = autoru.get();
                if (!autoruRes.empty())
                {
                    res[AutoruJsonField] = std::move(autoruRes);
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
                    res[AvitoJsonField] = std::move(avitoRes);
                } else
                {
                    LOG_WARN("Avito parser return empty result");
                }
            }

            // если есть результаты, то отправляем их мастеру
            if (!res.empty())
            {
                const auto &masterAddr = config_.getMasterAddr();
                LOG_DEBUG("Send parsing result to master: {}", masterAddr);
                RestClient::Response response = RestClient::post(masterAddr, "application/json",
                                                                 res.dump());
                if (response.code != 200)
                {
                    LOG_ERROR("Unable send parsing result to master: {}", masterAddr);
                }
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

