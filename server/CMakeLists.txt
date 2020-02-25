cmake_minimum_required(VERSION 3.15)
project(auto-ads-parser CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

option(BUILD_PARSER_MASTER "Скомпилировать master парсер" ON)
option(BUILD_PARSER_SLAVE "Скомпилировать slave парсер" ON)

add_subdirectory(common)

if (${BUILD_PARSER_MASTER})
    add_subdirectory(master)
endif ()

if (${BUILD_PARSER_SLAVE})
    add_subdirectory(slave)
endif ()