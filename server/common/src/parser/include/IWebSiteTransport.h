#ifndef AUTO_ADS_PARSER_IWEBSITETRANSPORT_H
#define AUTO_ADS_PARSER_IWEBSITETRANSPORT_H

#include <string>
#include <utility>

namespace parser
{
    /**
     * @brief Интерфейс получения веб-страниц
     */
    struct IWebSiteTransport
    {
        virtual std::string get(const std::string &link) = 0;

        virtual ~IWebSiteTransport() = default;
    };

    /**
     * @brief Ошибка получения данных
     */
    class WebSiteTransportError : public std::exception
    {
    public:
        explicit WebSiteTransportError(const char *msg) : msg_{msg} {}

        explicit WebSiteTransportError(std::string msg) : msg_{std::move(msg)} {}

        const char *what() const noexcept override { return msg_.c_str(); }

        const std::string &getMsg() const noexcept { return msg_; }

    private:
        std::string msg_;
    };
}

#endif //AUTO_ADS_PARSER_IWEBSITETRANSPORT_H
