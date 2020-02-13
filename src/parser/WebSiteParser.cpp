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
        LOG_DEBUG("{}: found {} pages", pageParser_->name(), pageCount);

        // обрабатываем первую страницу и если есть другие, то проходим и по ним.
        LOG_TRACE("{}: start parse first page", pageParser_->name());
        parsePageAds(doc);
        LOG_TRACE("{}: end parse first page", pageParser_->name());

        for (int page = 2; page <= pageCount; ++page)
        {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(5s); // не наглеем

            LOG_TRACE("{}: start parse page {}", pageParser_->name(), page);
            r = transport_->get(fmt::format(link_, page));
            doc.parse(r);
            parsePageAds(doc);
            LOG_TRACE("{}: end parse page {}", pageParser_->name(), page);
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
            LOG_TRACE("{}: start parse ad {} ", pageParser_->name(), i);
            std::string id;
            try
            {
                auto ad = ads.nodeAt(i);
                std::string link = pageParser_->getLink(ad);
                id = pageParser_->getId(link);
                LOG_DEBUG("{}: ad {} has id {}", pageParser_->name(), i, id);
                const uint32_t price = pageParser_->getPrice(ad);
                const uint16_t year = pageParser_->getYear(ad);
                const uint32_t mileage = pageParser_->getMileage(ad);
                auto images = pageParser_->getImageLinks(ad);

                // добавляем объявление
                bool isSuccess;
                std::tie(std::ignore, isSuccess) = ads_.emplace(id, std::move(link), price, year, mileage,
                                                                std::move(images));
                if (!isSuccess)
                {
                    LOG_WARN("{}: ad {} already exist", pageParser_->name(), id);
                }
            } catch (const ParseError &e)
            {
                LOG_WARN("{}: failed to fully parse ad {}: {}", pageParser_->name(), (id.empty() ? "?" : id), e.what());
            }
            LOG_TRACE("{}: end parse ad {} ", pageParser_->name(), i);
        }
    }
}