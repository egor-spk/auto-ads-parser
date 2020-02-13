#ifndef AUTO_ADS_PARSER_LOG_H
#define AUTO_ADS_PARSER_LOG_H

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <spdlog/async.h>

inline std::shared_ptr<spdlog::logger> logger;

using LogLevel = spdlog::level::level_enum;

inline void initLogger(LogLevel level = LogLevel::debug)
{
    namespace fs = std::filesystem;

    // если нет папки для логов, то создаем ее
    const fs::path path = "log/";
    if (fs::exists(path))
        fs::create_directories(path);

    logger = spdlog::create_async_nb<spdlog::sinks::rotating_file_sink_mt>("main", "log/log.log", 1024 * 1024 * 5, 3);
    logger->set_pattern("%d-%m-%Y %H:%M:%S.%e [%t] [%l] [%s:%#] %v");
    logger->set_level(level);
}

#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(logger, __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(logger, __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(logger, __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(logger, __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(logger, __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(logger, __VA_ARGS__)

#endif //AUTO_ADS_PARSER_LOG_H
