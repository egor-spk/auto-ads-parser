#ifndef AUTO_ADS_PARSER_AD_H
#define AUTO_ADS_PARSER_AD_H

#include <nlohmann/json.hpp>

#include <string>
#include <utility>
#include <vector>

namespace parser
{
    /**
     * @brief Представление объявления
     */
    struct Ad
    {
        static constexpr const char *IdField = "id";
        static constexpr const char *LinkField = "link";
        static constexpr const char *PriceField = "price";
        static constexpr const char *YearField = "year";
        static constexpr const char *MileageField = "mileage";
        static constexpr const char *ImagesField = "images";

        Ad(std::string id,
           std::string link,
           uint32_t price,
           uint16_t year,
           uint32_t mileage,
           std::vector<std::string> images) : id(std::move(id)),
                                              link(std::move(link)),
                                              price(price),
                                              year(year),
                                              mileage(mileage),
                                              images(std::move(images)) {}

        std::string id;
        std::string link;
        uint32_t price;
        uint16_t year;
        uint32_t mileage;
        std::vector<std::string> images;

        bool operator==(const Ad &rhs) const
        {
            return id == rhs.id;
        }

        bool operator!=(const Ad &rhs) const
        {
            return !(rhs == *this);
        }

        operator nlohmann::json() const
        {
            return {
                    {IdField,      id},
                    {LinkField,    link},
                    {PriceField,   price},
                    {YearField,    year},
                    {MileageField, mileage},
                    {ImagesField,  images}
            };
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
