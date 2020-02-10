#ifndef AUTO_ADS_PARSER_UTILS_H
#define AUTO_ADS_PARSER_UTILS_H

#include <string>
#include <regex>

namespace utils
{
    template <typename T = int>
    inline T getDecimalFromStr(const std::string &str)
    {
        static std::regex reg("\\d", std::regex::ECMAScript);
        auto numBegin =
                std::sregex_iterator(str.cbegin(), str.cend(), reg);
        auto numEnd = std::sregex_iterator();
        const auto countMatch = std::distance(numBegin, numEnd);
        if (countMatch == 0)
            throw std::invalid_argument("Unable find decimals in string");

        std::string strNumbers;
        strNumbers.reserve(countMatch);
        std::for_each(numBegin, numEnd, [&strNumbers](const auto &match)
        {
            strNumbers += match.str();
        });

        if constexpr (std::is_same_v<T, int>)
            return std::stoi(strNumbers);
        if constexpr (std::is_same_v<T, long>)
            return std::stol(strNumbers);
        if constexpr (std::is_same_v<T, unsigned long>)
            return std::stoul(strNumbers);
        if constexpr (std::is_same_v<T, long long>)
            return std::stoll(strNumbers);
        if constexpr (std::is_same_v<T, unsigned long long>)
            return std::stoull(strNumbers);
    }
}

#endif //AUTO_ADS_PARSER_UTILS_H
