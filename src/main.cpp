#include "common/log.h"
#include "parser/include/IParser.h"
#include "parser/include/WebSiteParser.h"
#include "parser/include/CurlTransport.h"
#include "parser/include/AutoruPageInfoParser.h"
#include "parser/include/AvitoPageInfoParser.h"
#include "config/include/AppConfig.h"

#include <iostream>
#include <fmt/color.h>
#include <fstream>
#include <future>

void outputResult(std::shared_ptr<parser::IParser> &parser, const std::string &filename);

int main(int argc, char **argv) try
{
    // Проверяем единственный параметр - конфиг
    if (argc < 2)
    {
        std::cerr << fmt::format(fg(fmt::color::red), "No path to config") << std::endl;
        return EXIT_FAILURE;
    }
    const std::string path = argv[1];
    if (!std::filesystem::exists(path))
    {
        std::cerr << fmt::format(fg(fmt::color::red), "Config does not found at path: {}", path) << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream is(path);
    app_config::AppConfig config(is);
    is.close();

    // инициализируем логгер
    initLogger(config.getLogLevel());
    LOG_DEBUG("App start with links: \nautoru - \"{}\"\navito - \"{}\"", config.getAutoruLink(), config.getAvitoLink());

    // парсинг
    auto autoru = std::async(std::launch::async, [&config]()
    {
        using namespace parser;
        std::shared_ptr<IParser> parser = std::make_shared<WebSiteParser>(std::make_unique<CurlTransport>(),
                                                                          std::make_unique<AutoruPageInfoParser>(),
                                                                          config.getAutoruLink());
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
    auto avito = std::async(std::launch::async, [&config]()
    {
        using namespace parser;
        std::shared_ptr<IParser> parser = std::make_shared<WebSiteParser>(std::make_unique<CurlTransport>(),
                                                                          std::make_unique<AvitoPageInfoParser>(),
                                                                          config.getAvitoLink());
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