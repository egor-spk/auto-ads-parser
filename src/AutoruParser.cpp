#include "include/AutoruParser.h"
#include "include/log.h"

#include <restclient-cpp/restclient.h>
#include <Document.h>
#include <fmt/format.h>
#include <Node.h>
#include <regex>
#include <thread>

AutoruParser::AutoruParser(const char *link) : link_{link} {}

std::vector<Ad> &AutoruParser::getAds() noexcept
{
    return ads_;
}

void AutoruParser::parse()
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

size_t AutoruParser::countResult() const noexcept
{
    return ads_.size();
}

void AutoruParser::clean() noexcept
{
    ads_.clear();
}

size_t AutoruParser::getPageCount(CDocument &doc)
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

void AutoruParser::parsePageAds(CDocument &doc)
{
    // ищем блок с объявлениями
    auto select = doc.find("div.ListingCars-module__list");
    if (select.nodeNum() == 0)
        throw ParseError("Unable to find ads block");

    // находим объявления
    auto ads = select.nodeAt(0).find("div.ListingCars-module__listingItem");
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
            logger->warn("Failed to fully parse ad {}: {}", (id.empty() ? "?" : id), e.what());
        }
    }
}

std::string AutoruParser::getLink(CNode &node)
{
    auto select = node.find("a.ListingItemTitle-module__link");
    if(select.nodeNum() == 0)
         throw ParseError("Unable to find link");
    auto link = select.nodeAt(0).attribute("href");
    if(link.empty())
        throw ParseError("Found link is empty");
    return link;
}

std::string AutoruParser::getId(const std::string &link)
{
    static std::regex reg("\\d{10}-\\w{8}", std::regex::ECMAScript);
    std::smatch match;
    std::regex_search(link, match, reg);
    if(match.empty() || match.size() > 1)
        throw ParseError("Unable to parse id");

    return match[0].str();
}

uint32_t AutoruParser::getPrice(CNode &node)
{
    auto select = node.find("div.ListingItemPrice-module__content");
    if(select.nodeNum() == 0)
        throw ParseError("Unable to find price");
    auto rawPrice = select.nodeAt(0).text();
    if(rawPrice.empty())
        throw ParseError("Found price is empty");

    static std::regex reg("\\d", std::regex::ECMAScript);
    auto numBegin =
            std::sregex_iterator(rawPrice.begin(), rawPrice.end(), reg);
    auto numEnd = std::sregex_iterator();
    const auto countMatch = std::distance(numBegin, numEnd);
    if(countMatch == 0)
        throw ParseError("Unable to parse price");

    std::string price;
    price.reserve(countMatch);
    std::for_each(numBegin, numEnd, [&price](const auto& match) {
        price += match.str();
    });
    return std::stoi(price);
}

uint16_t AutoruParser::getYear(CNode &node)
{
    auto select = node.find("div.ListingItem-module__year");
    if(select.nodeNum() == 0)
        throw ParseError("Unable to find year");
    auto year = select.nodeAt(0).text();
    if(year.empty())
        throw ParseError("Found year is empty");
    return std::stoi(year);
}

uint32_t AutoruParser::getMileage(CNode &node)
{
    auto select = node.find("div.ListingItem-module__kmAge");
    if(select.nodeNum() == 0)
        throw ParseError("Unable to find mileage");
    auto rawMileage = select.nodeAt(0).text();
    if(rawMileage.empty())
        throw ParseError("Found mileage is empty");

    static std::regex reg("\\d", std::regex::ECMAScript);
    auto numBegin =
            std::sregex_iterator(rawMileage.begin(), rawMileage.end(), reg);
    auto numEnd = std::sregex_iterator();
    const auto countMatch = std::distance(numBegin, numEnd);
    if(countMatch == 0)
        throw ParseError("Unable to parse mileage");

    std::string mileage;
    mileage.reserve(countMatch);
    std::for_each(numBegin, numEnd, [&mileage](const auto& match) {
        mileage += match.str();
    });
    return std::stoi(mileage);
}
