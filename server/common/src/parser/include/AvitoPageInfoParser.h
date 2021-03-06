#ifndef AUTO_ADS_PARSER_AVITOPAGEINFOPARSER_H
#define AUTO_ADS_PARSER_AVITOPAGEINFOPARSER_H

#include "IWebPageInfoParser.h"

namespace parser
{
    class AvitoPageInfoParser : public IWebPageInfoParser
    {
    public:
        size_t getPageCount(CDocument &doc) override;

        std::string getLink(CNode &node) override;

        std::string getId(const std::string &link) override;

        uint32_t getPrice(CNode &node) override;

        uint16_t getYear(CNode &node) override;

        uint32_t getMileage(CNode &node) override;

        std::vector<std::string> getImageLinks(CNode &node) override;

        const char *adsBlockQuery() const noexcept override { return "div.js-catalog_serp"; }

        const char *singleAdBlockQuery() const noexcept override { return "div.item-with-contact"; }

        const char *name() const noexcept override { return "Avito"; }
    };
}

#endif //AUTO_ADS_PARSER_AVITOPAGEINFOPARSER_H
