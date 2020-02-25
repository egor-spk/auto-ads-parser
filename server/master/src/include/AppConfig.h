#ifndef AUTO_ADS_PARSER_APPCONFIG_H
#define AUTO_ADS_PARSER_APPCONFIG_H

#include <nlohmann/json.hpp>
#include "IAppConfig.h"

namespace app_config
{
    /**
     * @brief Конфиг приложения
     */
    class AppConfig : public IAppConfig
    {
    public:
        explicit AppConfig(std::istream &is);

        const std::optional<std::string> &getAutoruLink() const noexcept override { return autoruLink_; }

        const std::optional<std::string> &getAvitoLink() const noexcept override { return avitoLink_; }

        uint16_t getPort() const noexcept override { return port_; }

        const std::string &getApiKey() const noexcept override { return apiKey_; }

        LogLevel getLogLevel() const noexcept override { return level_; }

        const std::string &getLogPath() const noexcept override { return logPath_; }

    private:
        void parseFields(const nlohmann::json &jsConfig);

        std::optional<std::string> autoruLink_;
        std::optional<std::string> avitoLink_;
        LogLevel level_;
        uint16_t port_;
        std::string apiKey_;
        std::string logPath_;
    };
}

#endif //AUTO_ADS_PARSER_APPCONFIG_H
