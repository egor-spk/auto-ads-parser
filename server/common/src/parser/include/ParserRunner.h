#ifndef AUTO_ADS_PARSER_PARSERRUNNER_H
#define AUTO_ADS_PARSER_PARSERRUNNER_H

#include "IWebPageInfoParser.h"
#include "WebSiteParser.h"
#include "CurlTransport.h"

namespace parser
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
#endif //AUTO_ADS_PARSER_PARSERRUNNER_H
