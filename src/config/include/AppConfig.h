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

        const std::string &getAutoruLink() const noexcept override { return autoruLink_; }

        const std::string &getAvitoLink() const noexcept override { return avitoLink_; }

        uint16_t getPort() const noexcept override { return port_; }

        const std::string &getApiKey() const noexcept override { return apiKey_; }

        LogLevel getLogLevel() const noexcept override { return level_; }

    private:
        void parseFields(const nlohmann::json &jsConfig);

        std::string autoruLink_;
        std::string avitoLink_;
        LogLevel level_;
        uint16_t port_;
        std::string apiKey_;
    };
}

#endif //AUTO_ADS_PARSER_APPCONFIG_H
