#ifndef AUTO_ADS_PARSER_WEBSITEPARSER_H
#define AUTO_ADS_PARSER_WEBSITEPARSER_H

#include "IParser.h"
#include "IWebSiteTransport.h"
#include "IWebPageInfoParser.h"

#include <memory>
#include <unordered_set>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace parser
{
    /**
     * Общий класс парсинга веб-страниц
     */
    class WebSiteParser : public IParser
    {
    public:
        WebSiteParser(std::unique_ptr<IWebSiteTransport> transport,
                      std::unique_ptr<IWebPageInfoParser> pageParser,
                      std::string link);

        std::unordered_set<Ad> &getResult() noexcept override { return ads_; }

        void parse() override;

        size_t countResult() const noexcept override { return ads_.size(); }

        void clean() noexcept override { ads_.clear(); }

        void stop() noexcept override;

    private:
        void parsePageAds(CDocument &document);

        std::atomic_bool isWork{true};
        std::mutex waitMutex_;
        std::condition_variable waitCv_;
        std::unique_ptr<IWebSiteTransport> transport_;
        std::unique_ptr<IWebPageInfoParser> pageParser_;
        std::string link_;
        std::unordered_set<Ad> ads_;
    };
}
#endif //AUTO_ADS_PARSER_WEBSITEPARSER_H
