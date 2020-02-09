include(${CMAKE_SOURCE_DIR}/cmake/Utils.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/restclient-cpp.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies/fmtlib.cmake)

add_executable_from_cur_dir(auto-ads-parser)
target_link_libraries(auto-ads-parser stdc++fs restclient-cpp fmt::fmt)
target_include_directories(auto-ads-parser PRIVATE ${RESTCLIENT_INCLUDE_DIRS})