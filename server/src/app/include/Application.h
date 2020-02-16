#ifndef AUTO_ADS_PARSER_APPLICATION_H
#define AUTO_ADS_PARSER_APPLICATION_H

#include "../../config/include/IAppConfig.h"
#include "../../parser/include/IParser.h"

#include <nlohmann/json.hpp>
#include <type_traits>
#include <condition_variable>

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
    static constexpr const char* AvitoCacheField = "avito";
    static constexpr const char* AutoruCacheField = "autoru";

    void startParsing() noexcept;

    void stopAll() noexcept;

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
