#ifndef AUTO_ADS_PARSER_APPLICATION_H
#define AUTO_ADS_PARSER_APPLICATION_H

#include "IAppConfig.h"
#include "IParser.h"

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

    void startParsing() noexcept;

    int run();

private:
    static constexpr const char* AvitoJsonField = "avito";
    static constexpr const char* AutoruJsonField = "autoru";

    void stopAll() noexcept;

    app_config::IAppConfig &config_;

    // парсинг
    std::unique_ptr<std::thread> parserThr_;
    std::vector<std::shared_ptr<parser::IParser>> parsers_;
    std::mutex waitMutex_;
    std::condition_variable waitCv_;
    const std::chrono::hours timeout{6};
    std::atomic_bool isWork{true};
};


#endif //AUTO_ADS_PARSER_APPLICATION_H
