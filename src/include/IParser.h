#ifndef AUTO_ADS_PARSER_IPARSER_H
#define AUTO_ADS_PARSER_IPARSER_H

#include <vector>
#include "Ad.h"

struct IParser
{
    virtual std::vector<Ad> &getAds() noexcept = 0;

    virtual void parse() = 0;

    virtual size_t countResult() const noexcept = 0;

    virtual void clean() noexcept = 0;

    virtual ~IParser() = default;
};

class ParseError : public std::exception
{
public:
    explicit ParseError(const char *msg) : msg_{msg} {}

    const char *what() const noexcept override { return msg_; }

private:
    const char *msg_;
};

#endif //AUTO_ADS_PARSER_IPARSER_H
