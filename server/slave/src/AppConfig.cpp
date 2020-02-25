#include "include/AppConfig.h"

#include <nlohmann/json.hpp>

namespace app_config
{
    AppConfig::AppConfig(std::istream &is)
    {
        try
        {
            if (!is.good())
            {
                throw AppConfigError("Input stream is invalid");
            }

            nlohmann::json jsConfig;
            is >> jsConfig;

            parseFields(jsConfig);
        } catch (const nlohmann::json::exception &e)
        {
            throw AppConfigError(fmt::format("An error occurred while parsing the config: {}", e.what()));
        }
    }

    void AppConfig::parseFields(const nlohmann::json &jsConfig)
    {
        // поля для парсинга
        const char *AutoruField = "autoru";
        const char *AvitoField = "avito";
        const char *LogLevelField = "loglevel";
        const char *MasterAddressField = "master_addr";
        const char *LogPathField = "log_path";

        // ссылки
        if (jsConfig.contains(AutoruField) && !jsConfig[AutoruField].empty())
        {
            autoruLink_ = jsConfig[AutoruField];
        }
        if (jsConfig.contains(AvitoField) && !jsConfig[AvitoField].empty())
        {
            avitoLink_ = jsConfig[AvitoField];
        }

        // адрес мастера
        if (!jsConfig.contains(MasterAddressField) || jsConfig[MasterAddressField].empty())
        {
            throw AppConfigError("master_addr field must be set");
        }
        masterAddr_ = jsConfig[MasterAddressField];

        // путь до лога
        if (!jsConfig.contains(LogPathField) || jsConfig[LogPathField].empty())
        {
            throw AppConfigError("log_path field must be set");
        }
        logPath_ = jsConfig[LogPathField];

        // уровень лога
        if (!jsConfig.contains(LogLevelField) || jsConfig[LogLevelField].empty())
        {
            throw AppConfigError("loglevel field must be set");
        }
        auto getLoglevel = [&]()
        {
            const std::unordered_map<std::string, LogLevel> levels = {
                    {"trace",    LogLevel::trace},
                    {"debug",    LogLevel::debug},
                    {"info",     LogLevel::info},
                    {"error",    LogLevel::err},
                    {"warning",  LogLevel::warn},
                    {"critical", LogLevel::critical},
                    {"off",      LogLevel::off}
            };
            const auto level = jsConfig[LogLevelField].get<std::string>();
            if (levels.find(level) == levels.cend())
                throw AppConfigError(fmt::format("Unknown log level: {}", level));
            return levels.at(level);
        };
        level_ = getLoglevel();
    }
}