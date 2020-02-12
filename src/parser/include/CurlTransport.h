#ifndef AUTO_ADS_PARSER_CURLTRANSPORT_H
#define AUTO_ADS_PARSER_CURLTRANSPORT_H

#include "IWebSiteTransport.h"

#include <restclient-cpp/restclient.h>
#include <fmt/format.h>

namespace parser
{
    class CurlTransport : public IWebSiteTransport
    {
    public:
        std::string get(const std::string &link) override
        {
            RestClient::Response r = RestClient::get(link);

            if (r.code != 200)
            {
                throw WebSiteTransport(fmt::format("Unable to fetch \"{}\". Error {}: {}",
                                                   link, r.code, r.body));
            }

            return r.body;
        }
    };
}

#endif //AUTO_ADS_PARSER_CURLTRANSPORT_H
