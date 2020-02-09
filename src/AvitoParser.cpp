#include "include/AvitoParser.h"

#include <restclient-cpp/restclient.h>
#include <Document.h>
#include <fmt/format.h>
#include <Node.h>
#include <fmt/color.h>
#include <iostream>
#include <regex>
#include <thread>

AvitoParser::AvitoParser(const char *link) : link_{link} {}

std::vector<Ad> &AvitoParser::getAds() noexcept
{
    return ads_;
}

void AvitoParser::parse()
{
    // открываем первую страницу и определяем общее количество страниц
    RestClient::Response r = RestClient::get(fmt::format(link_, 1));
    CDocument doc;
    doc.parse(r.body);
    const size_t pageCount = getPageCount(doc);

    // обрабатываем первую страницу и если есть другие, то проходим и по ним.
    parsePageAds(doc);

    for (int page = 2; page <= pageCount; ++page)
    {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(5s); // не наглеем

        r = RestClient::get(fmt::format(link_, page));
        doc.parse(r.body);
        parsePageAds(doc);
    }
}

size_t AvitoParser::countResult() const noexcept
{
    return ads_.size();
}

void AvitoParser::clean() noexcept
{
    ads_.clear();
}

size_t AvitoParser::getPageCount(CDocument &doc)
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

void AvitoParser::parsePageAds(CDocument &doc)
{
    // ищем блок с объявлениями
    auto select = doc.find("div.js-catalog_serp");
    if (select.nodeNum() == 0)
        throw ParseError("Unable to find ads block");

    // находим объявления
    auto ads = select.nodeAt(0).find("div.item-with-contact");
    const auto adsCount = ads.nodeNum();
    if (adsCount == 0)
        throw ParseError("Unable to find ads in ads block");

    // обходим объявления и извлекаем информацию
    for (int i = 0; i < adsCount; ++i)
    {
        std::string id;
        try
        {
            auto ad = ads.nodeAt(i);
            const std::string link = getLink(ad);
            id = getId(link);
            const uint32_t price = getPrice(ad);
            const uint16_t year = getYear(ad);
            const uint32_t mileage = getMileage(ad);

            ads_.emplace_back(id, link, price, year, mileage);
        } catch (const ParseError &e)
        {
            std::cout << fmt::format(fg(fmt::color::yellow), "Failed to fully parse ad {}: {}",
                                     (id.empty() ? "?" : id), e.what())
                      << std::endl;
        }
    }
}

std::string AvitoParser::getLink(CNode &node)
{
    auto select = node.find("a.snippet-link");
    if (select.nodeNum() == 0)
        throw ParseError("Unable to find link");
    auto link = select.nodeAt(0).attribute("href");
    if (link.empty())
        throw ParseError("Found link is empty");
    return "https://avito.ru" + link;
}

std::string AvitoParser::getId(const std::string &link)
{
    static std::regex reg("\\w+\\_+.+", std::regex::ECMAScript);
    std::smatch match;
    std::regex_search(link, match, reg);
    if (match.empty() || match.size() > 1)
        throw ParseError("Unable to parse id");

    return match[0].str();
}

uint32_t AvitoParser::getPrice(CNode &node)
{
    auto select = node.find("div.about span");
    if (select.nodeNum() < 2)
        throw ParseError("Unable to find price");

    auto rawPrice = select.nodeAt(1).text();
    if (rawPrice.empty())
        throw ParseError("Found price is empty");

    static std::regex reg("\\d", std::regex::ECMAScript);
    auto numBegin =
            std::sregex_iterator(rawPrice.begin(), rawPrice.end(), reg);
    auto numEnd = std::sregex_iterator();
    const auto countMatch = std::distance(numBegin, numEnd);
    if (countMatch == 0)
        throw ParseError("Unable to parse price");

    std::string price;
    price.reserve(countMatch);
    std::for_each(numBegin, numEnd, [&price](const auto &match)
    {
        price += match.str();
    });
    return std::stoi(price);
}

uint16_t AvitoParser::getYear(CNode &node)
{
    auto select = node.find("a.snippet-link");
    if (select.nodeNum() == 0)
        throw ParseError("Unable to find year");
    auto rawYear = select.nodeAt(0).text();
    if (rawYear.empty())
        throw ParseError("Found year is empty");

    static std::regex reg("\\d", std::regex::ECMAScript);
    auto numBegin =
            std::sregex_iterator(rawYear.begin(), rawYear.end(), reg);
    auto numEnd = std::sregex_iterator();
    const auto countMatch = std::distance(numBegin, numEnd);
    if (countMatch == 0)
        throw ParseError("Unable to parse year");

    std::string year;
    year.reserve(countMatch);
    std::for_each(numBegin, numEnd, [&year](const auto &match)
    {
        year += match.str();
    });
    return std::stoi(year);
}

uint32_t AvitoParser::getMileage(CNode &node)
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

    static std::regex reg("\\d", std::regex::ECMAScript);
    auto numBegin =
            std::sregex_iterator(rawMileage.begin(), rawMileage.end(), reg);
    auto numEnd = std::sregex_iterator();
    const auto countMatch = std::distance(numBegin, numEnd);
    if (countMatch == 0)
        throw ParseError("Unable to parse year");

    std::string mileage;
    mileage.reserve(countMatch);
    std::for_each(numBegin, numEnd, [&mileage](const auto &match)
    {
        mileage += match.str();
    });
    return std::stoi(mileage);
}
