include(${CMAKE_SOURCE_DIR}/cmake/dependencies/restinio.cmake)

# Рекурсивно собираем файлы с текущего каталога
file(GLOB_RECURSE TARGET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# Добавляем исполняемый файл
add_executable(parser-master ${TARGET_SRC})
add_dependencies(parser-master restinio)
target_link_libraries(parser-master common ${RESTINIO_LIBS})
target_include_directories(parser-master PRIVATE ${RESTINIO_INCLUDE_DIR})