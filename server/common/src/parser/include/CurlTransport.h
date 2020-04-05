#ifndef AUTO_ADS_PARSER_CURLTRANSPORT_H
#define AUTO_ADS_PARSER_CURLTRANSPORT_H

#include "IWebSiteTransport.h"

#include <restclient-cpp/restclient.h>
#include <fmt/format.h>
#include <restclient-cpp/connection.h>
#include <regex>

namespace parser
{
    class CurlTransport : public IWebSiteTransport
    {
    public:
        CurlTransport() = default;

        explicit CurlTransport(const std::unordered_map<std::string, std::string> &cookie)
                : cookie_(cookieToStr(cookie)) {}

        std::string get(const std::string &link) override
        {
            RestClient::Response response;
            bool repeatRequest{false};
            uint8_t requestCount{};
            auto link_ = link;

            do
            {
                ++requestCount;
                auto conn = std::make_unique<RestClient::Connection>(link_);
                if (!cookie_.empty())
                {
                    conn->AppendHeader("Cookie", cookie_);
                }

                response = conn->get("");
                if (response.code == 301 && requestCount < 3)
                {
                    const auto &headers = response.headers;
                    const char *LocationFieldName = "Location";
                    if (headers.find(LocationFieldName) == headers.cend())
                    {
                        throw WebSiteTransportError(fmt::format("Got 301 response code from \"{}\""
                                                                ", but unable to find \"Location\" header field.",
                                                                link_));
                    }

                    link_ = fixSiteUrl(link_, headers.at(LocationFieldName));
                    repeatRequest = true;
                } else
                {
                    repeatRequest = false;
                }
            } while (repeatRequest);

            if (response.code != 200)
            {
                throw WebSiteTransportError(fmt::format("Unable to fetch \"{}\". Error {}: {}",
                                                        link, response.code, response.body));
            }

            return response.body;
        }

    private:
        std::string cookieToStr(const std::unordered_map<std::string, std::string> &cookie) const noexcept
        {
            std::string res;
            for (const auto &[key, value] : cookie)
            {
                res += fmt::format("{}={};", key, value);
            }
            return res;
        }

        /**
         * @brief Добавляет протокол и адрес сайта, если в новом url его нет
         * @param originUrl полный url сайта
         * @param newUrl новый url
         * @return url с протоколом и адресом сайта
         */
        std::string fixSiteUrl(const std::string &originUrl, const std::string &newUrl)
        {
            // если новый url начинается со слеша, то требуется добавить протокол и адрес сайта
            if (*newUrl.cbegin() != '/')
                return newUrl;

            // получаем недостающую часть
            static const std::regex reg{R"(^https?\:\/\/[^\/]+)", std::regex_constants::icase};
            std::smatch match;
            if (!std::regex_search(originUrl, match, reg))
            {
                throw WebSiteTransportError(fmt::format("Unable find site and protocol in origin url: {}",
                                                        originUrl));
            }

            return match[0].str() + newUrl;
        }

        std::string cookie_;
    };
}

#endif //AUTO_ADS_PARSER_CURLTRANSPORT_H
