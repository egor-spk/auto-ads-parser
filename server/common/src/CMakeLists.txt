include(${CMAKE_SOURCE_DIR}/cmake/dependencies/fmtlib.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/nlohmann-json.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/restclient-cpp.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/gumbo-query.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/spdlog.cmake)

# Рекурсивно собираем файлы с текущего каталога
file(GLOB_RECURSE TARGET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# Добавляем библиотеку
add_library(common OBJECT ${TARGET_SRC})
target_include_directories(common PUBLIC ${RESTCLIENT_INCLUDE_DIRS} ${GUMBO_QUERY_INCLUDE_DIRS})
target_include_directories(common INTERFACE include parser/include)
target_link_libraries(common spdlog::spdlog nlohmann_json::nlohmann_json stdc++fs restclient-cpp fmt::fmt
        gumbo_query_static)