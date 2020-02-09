#include <iostream>
#include <fmt/color.h>
#include <sstream>
#include <fstream>
#include <future>
#include "include/AutoruParser.h"
#include "include/AvitoParser.h"

void outputResult(std::shared_ptr<IParser> &parser, const std::string &filename);

int main() try
{
    auto autoru = std::async(std::launch::async, []()
    {
        std::shared_ptr<IParser> parser = std::make_shared<AutoruParser>(
                "https://auto.ru/moskva/cars/hyundai/solaris/20922677/used/?catalog_equipment=front-centre-armrest&km_age_to=60000&owners_count_group=ONE&transmission=AUTOMATIC&sort=fresh_relevance_1-desc&output_type=list&page={}");
        parser->parse();
        std::cout << fmt::format(fg(fmt::color::green), "Autoru: Successfully parse {} ads", parser->countResult())
                  << std::endl;
        outputResult(parser, "autoru.csv");
        return std::move(parser);
    });
    auto avito = std::async(std::launch::async, []()
    {
        std::shared_ptr<IParser> parser = std::make_shared<AvitoParser>(
                "https://www.avito.ru/moskva/avtomobili/s_probegom/hyundai/solaris/avtomat/odin_vladelec?cd=1&pmin=600000&radius=0&f=188_19775b0.1286_14765b0&p={}");
        parser->parse();
        std::cout << fmt::format(fg(fmt::color::green), "Avito: Successfully parse {} ads", parser->countResult())
                  << std::endl;
        outputResult(parser, "avito.csv");
        return std::move(parser);
    });

    // вывод общего результата
    std::stringstream ss;
    ss << "id" << ","
       << "link" << ","
       << "price" << ","
       << "year" << ","
       << "mileage" << std::endl;


    std::vector<std::shared_ptr<IParser>> parsers{autoru.get(), avito.get()};
    for (const auto &parser : parsers)
    {
        const auto &ads = parser->getAds();
        for (const auto &ad : ads)
        {
            ss << ad.id << ","
               << ad.link << ","
               << ad.price << ","
               << ad.year << ","
               << ad.mileage << std::endl;
        }
    }

    std::ofstream os("common.csv");
    os << ss.str();

    return EXIT_SUCCESS;
} catch (const ParseError &e)
{
    std::cerr << fmt::format(fg(fmt::color::red), "Parsing error: {}", e.what());
} catch (const std::exception &e)
{
    std::cerr << fmt::format(fg(fmt::color::red), "Uncaught exception: {}", e.what());
}

void outputResult(std::shared_ptr<IParser> &parser, const std::string &filename)
{
    std::stringstream ss;
    ss << "id" << ","
       << "link" << ","
       << "price" << ","
       << "year" << ","
       << "mileage" << std::endl;

    const auto &ads = parser->getAds();
    for (const auto &ad : ads)
    {
        ss << ad.id << ","
           << ad.link << ","
           << ad.price << ","
           << ad.year << ","
           << ad.mileage << std::endl;
    }

    std::ofstream os(filename);
    os << ss.str();
}