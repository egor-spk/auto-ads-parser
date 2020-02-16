#include "common/log.h"
#include "app/include/Application.h"
#include "config/include/AppConfig.h"

#include <iostream>
#include <fmt/color.h>
#include <fstream>

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

    // запускаем приложение
    Application app{config};
    return app.run();
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