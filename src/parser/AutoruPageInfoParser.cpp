#include "include/AutoruPageInfoParser.h"
#include "include/IParser.h"
#include "../common/utils.h"

#include <regex>

namespace parser
{
    size_t AutoruPageInfoParser::getPageCount(CDocument &doc)
    {
        auto select = doc.find("span.ListingPagination-module__pages");
        if (select.nodeNum() == 0)
            return 1;

        // выбираем последнию кнопку
        auto node = select.nodeAt(0);
        select = node.childAt(node.childNum() - 1).find("span.Button__text");
        if (select.nodeNum() == 0)
            throw ParseError("Unable to find count page tag");

        return std::stoi(select.nodeAt(0).text());
    }

    std::string AutoruPageInfoParser::getLink(CNode &node)
    {
        auto select = node.find("a.ListingItemTitle-module__link");
        if (select.nodeNum() == 0)
            throw ParseError("Unable to find link");
        auto link = select.nodeAt(0).attribute("href");
        if (link.empty())
            throw ParseError("Found link is empty");
        return link;
    }

    std::string AutoruPageInfoParser::getId(const std::string &link)
    {
        static std::regex reg("\\d{10}-\\w{8}", std::regex::ECMAScript);
        std::smatch match;
        std::regex_search(link, match, reg);
        if (match.empty() || match.size() > 1)
            throw ParseError("Unable to parse id");

        return match[0].str();
    }

    uint32_t AutoruPageInfoParser::getPrice(CNode &node)
    {
        auto select = node.find("div.ListingItemPrice-module__content");
        if (select.nodeNum() == 0)
            throw ParseError("Unable to find price");
        auto rawPrice = select.nodeAt(0).text();
        if (rawPrice.empty())
            throw ParseError("Found price is empty");

        try
        {
            return utils::getDecimalFromStr(rawPrice);
        } catch (const std::invalid_argument &e)
        {
            throw ParseError{e.what()};
        }
    }

    uint16_t AutoruPageInfoParser::getYear(CNode &node)
    {
        auto select = node.find("div.ListingItem-module__year");
        if (select.nodeNum() == 0)
            throw ParseError("Unable to find year");
        auto year = select.nodeAt(0).text();
        if (year.empty())
            throw ParseError("Found year is empty");
        return std::stoi(year);
    }

    uint32_t AutoruPageInfoParser::getMileage(CNode &node)
    {
        auto select = node.find("div.ListingItem-module__kmAge");
        if (select.nodeNum() == 0)
            throw ParseError("Unable to find mileage");
        auto rawMileage = select.nodeAt(0).text();
        if (rawMileage.empty())
            throw ParseError("Found mileage is empty");

        try
        {
            return utils::getDecimalFromStr(rawMileage);
        } catch (const std::invalid_argument &e)
        {
            throw ParseError{e.what()};
        }
    }
}