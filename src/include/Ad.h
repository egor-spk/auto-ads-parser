#ifndef AUTO_ADS_PARSER_AD_H
#define AUTO_ADS_PARSER_AD_H

#include <string>
#include <utility>

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
};

#endif //AUTO_ADS_PARSER_AD_H
