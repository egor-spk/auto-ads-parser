#include <iostream>
#include <fmt/color.h>
#include <sstream>
#include <fstream>
#include "include/AutoruParser.h"

int main() try
{
    AutoruParser parser(
            "https://auto.ru/moskva/cars/hyundai/solaris/20922677/used/?catalog_equipment=front-centre-armrest&km_age_to=60000&owners_count_group=ONE&transmission=AUTOMATIC&sort=fresh_relevance_1-desc&output_type=list&page={}");
    parser.parse();
    std::cout << fmt::format(fg(fmt::color::green), "Successfully parse {} ads", parser.countResult());

    // выводим результат
    std::stringstream ss;
    ss << "id" << ","
       << "link" << ","
       << "price" << ","
       << "year" << ","
       << "mileage" << std::endl;

    const auto &ads = parser.getAds();
    for (const auto &ad : ads)
    {
        ss << ad.id << ","
           << ad.link << ","
           << ad.price << ","
           << ad.year << ","
           << ad.mileage << std::endl;
    }

    std::ofstream os("result.csv");
    os << ss.str();
    return EXIT_SUCCESS;
} catch (const ParseError &e)
{
    std::cerr << fmt::format(fg(fmt::color::red), "Parsing error: {}", e.what());
} catch (const std::exception &e)
{
    std::cerr << fmt::format(fg(fmt::color::red), "Uncaught exception: {}", e.what());
}