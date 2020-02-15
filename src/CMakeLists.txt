include(${CMAKE_SOURCE_DIR}/cmake/Utils.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/restclient-cpp.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/fmtlib.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/gumbo-query.cmake)
set(SPDLOG_FMT_EXTERNAL ON CACHE INTERNAL "")
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/spdlog.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/nlohmann-json.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/restinio.cmake)

add_executable_from_cur_dir(auto-ads-parser)
add_dependencies(auto-ads-parser restinio)

target_link_libraries(auto-ads-parser stdc++fs restclient-cpp fmt::fmt gumbo_query_static spdlog::spdlog
        nlohmann_json::nlohmann_json ${RESTINIO_LIBS})
target_include_directories(auto-ads-parser PRIVATE ${RESTCLIENT_INCLUDE_DIRS} ${GUMBO_QUERY_INCLUDE_DIRS}
        ${RESTINIO_INCLUDE_DIR})