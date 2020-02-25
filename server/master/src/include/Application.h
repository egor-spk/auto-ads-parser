#ifndef AUTO_ADS_PARSER_APPLICATION_H
#define AUTO_ADS_PARSER_APPLICATION_H

#include "IAppConfig.h"
#include "IParser.h"

#include <nlohmann/json.hpp>
#include <type_traits>
#include <condition_variable>
#include <request_handler.hpp>
#include <router/express.hpp>

/**
 * @brief Класс приложения
 */
class Application final
{
public:
    explicit Application(app_config::IAppConfig &config);

    ~Application();

    int run();

private:
    static constexpr const char* AvitoJsonField = "avito";
    static constexpr const char* AutoruJsonField = "autoru";
    static constexpr const char* DateTimeJsonField = "date_time";
    static constexpr const char* ItemsJsonField = "items";

    void startParsing() noexcept;

    void stopAll() noexcept;

    std::string getCurrentDateTime() const;

    app_config::IAppConfig &config_;

    // кеш результатов парсинга
    std::mutex cacheMutex_;
    nlohmann::json cache_;
    std::string gzipAds_;

    // парсинг
    std::unique_ptr<std::thread> parserThr_;
    std::vector<std::shared_ptr<parser::IParser>> parsers_;
    std::mutex waitMutex_;
    std::condition_variable waitCv_;
    const std::chrono::hours timeout{6};
    std::atomic_bool isWork{true};
};


#endif //AUTO_ADS_PARSER_APPLICATION_H
