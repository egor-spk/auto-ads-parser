#include "common/log.h"
#include "parser/include/IParser.h"
#include "parser/include/WebSiteParser.h"
#include "parser/include/CurlTransport.h"
#include "parser/include/AutoruPageInfoParser.h"
#include "parser/include/AvitoPageInfoParser.h"

#include <iostream>
#include <fmt/color.h>
#include <fstream>
#include <future>

inline const auto autoruLink = "https://auto.ru/moskva/cars/hyundai/solaris/20922677/used/?catalog_equipment=front-centre-armrest&km_age_to=60000&owners_count_group=ONE&transmission=AUTOMATIC&sort=fresh_relevance_1-desc&output_type=list&page={}";
inline const auto avitoLink = "https://www.avito.ru/moskva/avtomobili/s_probegom/hyundai/solaris/avtomat/odin_vladelec?cd=1&pmin=600000&radius=0&f=188_19775b0.1286_14765b0&p={}";

void outputResult(std::shared_ptr<parser::IParser> &parser, const std::string &filename);

int main() try
{
    initLogger();

    auto autoru = std::async(std::launch::async, []()
    {
        using namespace parser;
        std::shared_ptr<IParser> parser = std::make_shared<WebSiteParser>(std::make_unique<CurlTransport>(),
                                                                          std::make_unique<AutoruPageInfoParser>(),
                                                                          autoruLink);
        try
        {
            parser->parse();
            LOG_INFO("Autoru: successfully parse {} ads", parser->countResult());
            outputResult(parser, "autoru.csv");
        } catch (const ParseError &e)
        {
            LOG_ERROR("Autoru parsing error: {}", e.what());
        }
        return std::move(parser);
    });
    auto avito = std::async(std::launch::async, []()
    {
        using namespace parser;
        std::shared_ptr<IParser> parser = std::make_shared<WebSiteParser>(std::make_unique<CurlTransport>(),
                                                                          std::make_unique<AvitoPageInfoParser>(),
                                                                          avitoLink);
        try
        {
            parser->parse();
            LOG_INFO("Avito: successfully parse {} ads", parser->countResult());
            outputResult(parser, "avito.csv");
        } catch (const ParseError &e)
        {
            LOG_ERROR("Avito parsing error: {}", e.what());
        }
        return std::move(parser);
    });

    // вывод общего результата
    std::stringstream ss;
    ss << "id" << ","
       << "link" << ","
       << "price" << ","
       << "year" << ","
       << "mileage" << std::endl;


    std::vector<std::shared_ptr<parser::IParser>> parsers{autoru.get(), avito.get()};
    for (const auto &parser : parsers)
    {
        const auto &ads = parser->getResult();
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
} catch (const std::exception &e)
{
    const auto message = fmt::format("Uncaught exception: {}", e.what());
    if (logger)
    {
        LOG_CRITICAL(message);
    } else
    {
        std::cerr << fmt::format(fg(fmt::color::red), message);
    }
}

void outputResult(std::shared_ptr<parser::IParser> &parser, const std::string &filename)
{
    std::stringstream ss;
    ss << "id" << ","
       << "link" << ","
       << "price" << ","
       << "year" << ","
       << "mileage" << std::endl;

    const auto &ads = parser->getResult();
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