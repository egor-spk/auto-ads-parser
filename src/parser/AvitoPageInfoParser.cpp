#include "include/AvitoPageInfoParser.h"
#include "include/IParser.h"
#include "../common/utils.h"

#include <regex>

namespace parser
{
    size_t AvitoPageInfoParser::getPageCount(CDocument &doc)
    {
        auto select = doc.find("div.pagination-root-2oCjZ");
        if (select.nodeNum() == 0)
            return 1;

        // выбираем последнию кнопку
        auto node = select.nodeAt(0);
        select = node.childAt(node.childNum() - 2).find("span.pagination-item-1WyVp");
        if (select.nodeNum() == 0)
            throw ParseError("Unable to find count page tag");

        return std::stoi(select.nodeAt(0).text());
    }

    std::string AvitoPageInfoParser::getLink(CNode &node)
    {
        auto select = node.find("a.snippet-link");
        if (select.nodeNum() == 0)
            throw ParseError("Unable to find link");
        auto link = select.nodeAt(0).attribute("href");
        if (link.empty())
            throw ParseError("Found link is empty");
        return "https://avito.ru" + link;
    }

    std::string AvitoPageInfoParser::getId(const std::string &link)
    {
        static std::regex reg("\\w+\\_+.+", std::regex::ECMAScript);
        std::smatch match;
        std::regex_search(link, match, reg);
        if (match.empty() || match.size() > 1)
            throw ParseError("Unable to parse id");

        return match[0].str();
    }

    uint32_t AvitoPageInfoParser::getPrice(CNode &node)
    {
        auto select = node.find("div.about span");
        if (select.nodeNum() < 2)
            throw ParseError("Unable to find price");

        auto rawPrice = select.nodeAt(1).text();
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

    uint16_t AvitoPageInfoParser::getYear(CNode &node)
    {
        auto select = node.find("a.snippet-link");
        if (select.nodeNum() == 0)
            throw ParseError("Unable to find year");
        auto rawYear = select.nodeAt(0).text();
        if (rawYear.empty())
            throw ParseError("Found year is empty");

        try
        {
            return utils::getDecimalFromStr(rawYear);
        } catch (const std::invalid_argument &e)
        {
            throw ParseError{e.what()};
        }
    }

    uint32_t AvitoPageInfoParser::getMileage(CNode &node)
    {
        auto select = node.find("div.specific-params");
        if (select.nodeNum() == 0)
            throw ParseError("Unable to find mileage");
        auto rawMileage = select.nodeAt(0).text();
        if (rawMileage.empty())
            throw ParseError("Found mileage is empty");

        const auto pos = rawMileage.find("км");
        if (pos == std::string::npos)
            throw ParseError("Unable to parse mileage");
        rawMileage.erase(pos);

        try
        {
            return utils::getDecimalFromStr(rawMileage);
        } catch (const std::invalid_argument &e)
        {
            throw ParseError{e.what()};
        }
    }
}