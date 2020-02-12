#ifndef AUTO_ADS_PARSER_LOG_H
#define AUTO_ADS_PARSER_LOG_H

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <filesystem>

inline std::shared_ptr<spdlog::logger> logger;

inline void initLogger()
{
    namespace fs = std::filesystem;

    // если нет папки для логов, то создаем ее
    const fs::path path = "log/";
    if(fs::exists(path))
        fs::create_directories(path);

    logger = spdlog::rotating_logger_mt("main", "log/log.log", 1024 * 1024 * 5, 3);
}
#endif //AUTO_ADS_PARSER_LOG_H
