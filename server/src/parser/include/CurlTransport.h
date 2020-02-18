#ifndef AUTO_ADS_PARSER_CURLTRANSPORT_H
#define AUTO_ADS_PARSER_CURLTRANSPORT_H

#include "IWebSiteTransport.h"

#include <restclient-cpp/restclient.h>
#include <fmt/format.h>
#include <restclient-cpp/connection.h>

namespace parser
{
    class CurlTransport : public IWebSiteTransport
    {
    public:
        CurlTransport() = default;

        CurlTransport(const std::unordered_map<std::string, std::string> &cookie)
                : cookie_(cookieToStr(cookie)) {}

        std::string get(const std::string &link) override
        {
            auto conn = std::make_unique<RestClient::Connection>(link);
            if (!cookie_.empty())
            {
                conn->AppendHeader("Cookie", cookie_);
            }

            RestClient::Response r = conn->get("");

            if (r.code != 200)
            {
                throw WebSiteTransportError(fmt::format("Unable to fetch \"{}\". Error {}: {}",
                                                        link, r.code, r.body));
            }

            return r.body;
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

        std::string cookie_;
    };
}

#endif //AUTO_ADS_PARSER_CURLTRANSPORT_H
