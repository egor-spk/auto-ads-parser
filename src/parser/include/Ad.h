#ifndef AUTO_ADS_PARSER_AD_H
#define AUTO_ADS_PARSER_AD_H

#include <string>
#include <utility>

namespace parser
{
    /**
     * @brief Представление объявления
     */
    struct Ad
    {
        Ad(std::string id,
           std::string link,
           uint32_t price,
           uint16_t year,
           uint32_t mileage) : id(std::move(id)),
                               link(std::move(link)),
                               price(price),
                               year(year),
                               mileage(mileage) {}

        std::string id;
        std::string link;
        uint32_t price;
        uint16_t year;
        uint32_t mileage;

        bool operator==(const Ad &rhs) const
        {
            return id == rhs.id;
        }

        bool operator!=(const Ad &rhs) const
        {
            return !(rhs == *this);
        }
    };
}

namespace std
{
    template<>
    struct hash<parser::Ad>
    {
        size_t operator()(const parser::Ad &ad) const
        {
            const static auto func = hash<std::string>();
            return func(ad.id);
        }
    };
}

#endif //AUTO_ADS_PARSER_AD_H
