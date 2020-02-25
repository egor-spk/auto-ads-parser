#ifndef AUTO_ADS_PARSER_IAPPCONFIG_H
#define AUTO_ADS_PARSER_IAPPCONFIG_H

#include <iosfwd>
#include "log.h"

namespace app_config
{
    /**
     * @brief Интерфейс конфига приложения
     */
    struct IAppConfig
    {
        virtual const std::optional<std::string> &getAutoruLink() const noexcept = 0;

        virtual const std::optional<std::string> &getAvitoLink() const noexcept = 0;

        virtual uint16_t getPort() const noexcept = 0;

        virtual const std::string &getApiKey() const noexcept = 0;

        virtual LogLevel getLogLevel() const noexcept = 0;

        virtual const std::string &getLogPath() const noexcept = 0;

        virtual ~IAppConfig() = default;
    };

    /**
     * @brief Ошибка парсинга
     */
    class AppConfigError : public std::exception
    {
    public:
        explicit AppConfigError(const char *msg) : msg_{msg} {}

        explicit AppConfigError(std::string msg) : msg_{std::move(msg)} {}

        const char *what() const noexcept override { return msg_.c_str(); }

        const std::string &getMsg() const noexcept { return msg_; }

    private:
        std::string msg_;
    };
}

#endif //AUTO_ADS_PARSER_IAPPCONFIG_H
