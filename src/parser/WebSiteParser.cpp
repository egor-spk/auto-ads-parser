#include "include/WebSiteParser.h"
#include "../common/log.h"

#include <fmt/format.h>
#include <chrono>
#include <thread>

namespace parser
{

    WebSiteParser::WebSiteParser(std::unique_ptr<IWebSiteTransport> transport,
                                 std::unique_ptr<IWebPageInfoParser> pageParser,
                                 std::string link) :
            transport_(std::move(transport)),
            pageParser_(std::move(pageParser)),
            link_(std::move(link))
    {
    }

    void WebSiteParser::parse()
    {
        // открываем первую страницу и определяем общее количество страниц
        auto r = transport_->get(fmt::format(link_, 1));
        CDocument doc;
        doc.parse(r);
        const size_t pageCount = pageParser_->getPageCount(doc);

        // обрабатываем первую страницу и если есть другие, то проходим и по ним.
        logger->debug("Start parse first page");
        parsePageAds(doc);

        for (int page = 2; page <= pageCount; ++page)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(5s); // не наглеем

            logger->debug("Start parse page {}", page);
            r = transport_->get(fmt::format(link_, page));
            doc.parse(r);
            parsePageAds(doc);
        }
    }

    void WebSiteParser::parsePageAds(CDocument &document)
    {
        // ищем блок с объявлениями
        auto select = document.find(pageParser_->adsBlockQuery());
        if (select.nodeNum() == 0)
            throw ParseError("Unable to find ads block");

        // находим объявления
        auto ads = select.nodeAt(0).find(pageParser_->singleAdBlockQuery());
        const auto adsCount = ads.nodeNum();
        if (adsCount == 0)
            throw ParseError("Unable to find ads in ads block");

        ads_.reserve(adsCount);

        // обходим объявления и извлекаем информацию
        for (int i = 0; i < adsCount; ++i)
        {
            std::string id;
            try
            {
                auto ad = ads.nodeAt(i);
                const std::string link = pageParser_->getLink(ad);
                id = pageParser_->getId(link);
                const uint32_t price = pageParser_->getPrice(ad);
                const uint16_t year = pageParser_->getYear(ad);
                const uint32_t mileage = pageParser_->getMileage(ad);

                // добавляем объявление
                bool isSuccess;
                std::tie(std::ignore, isSuccess) = ads_.emplace(id, link, price, year, mileage);
                if (!isSuccess)
                {
                    logger->warn("Ad {} already exist", id);
                }
            } catch (const ParseError &e)
            {
                logger->warn("Failed to fully parse ad {}: {}", (id.empty() ? "?" : id), e.what());
            }
        }
    }
}