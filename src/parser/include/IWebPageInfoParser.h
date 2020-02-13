#ifndef AUTO_ADS_PARSER_IWEBPAGEINFOPARSER_H
#define AUTO_ADS_PARSER_IWEBPAGEINFOPARSER_H

#include <string>
#include <Node.h>
#include <Document.h>

namespace parser
{
    /**
     * @brief Интерфейс получения информации со страницы
     */
    struct IWebPageInfoParser
    {
        virtual size_t getPageCount(CDocument &doc) = 0;

        virtual std::string getLink(CNode &node) = 0;

        virtual std::string getId(const std::string &link) = 0;

        virtual uint32_t getPrice(CNode &node) = 0;

        virtual uint16_t getYear(CNode &node) = 0;

        virtual uint32_t getMileage(CNode &node) = 0;

        virtual std::vector<std::string> getImageLinks(CNode &node) = 0;

        virtual const char* adsBlockQuery() const noexcept = 0;

        virtual const char* singleAdBlockQuery() const noexcept = 0;

        virtual const char* name() const noexcept = 0;

        virtual ~IWebPageInfoParser() = default;
    };
}

#endif //AUTO_ADS_PARSER_IWEBPAGEINFOPARSER_H
