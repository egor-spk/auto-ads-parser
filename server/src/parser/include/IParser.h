#ifndef AUTO_ADS_PARSER_IPARSER_H
#define AUTO_ADS_PARSER_IPARSER_H

#include "Ad.h"

#include <vector>
#include <unordered_set>

namespace parser
{
    /**
     * @brief Интерфейс парсера объявлений
     */
    struct IParser
    {
        virtual std::unordered_set<Ad> &getResult() noexcept = 0;

        virtual void parse() = 0;

        virtual size_t countResult() const noexcept = 0;

        virtual void clean() noexcept = 0;

        virtual void stop() noexcept = 0;

        virtual ~IParser() = default;
    };

    /**
     * @brief Ошибка парсинга
     */
    class ParseError : public std::exception
    {
    public:
        explicit ParseError(const char *msg) : msg_{msg} {}

        explicit ParseError(std::string msg) : msg_{std::move(msg)} {}

        const char *what() const noexcept override { return msg_.c_str(); }

        const std::string &getMsg() const noexcept { return msg_; }

    private:
        std::string msg_;
    };
}

#endif //AUTO_ADS_PARSER_IPARSER_H
