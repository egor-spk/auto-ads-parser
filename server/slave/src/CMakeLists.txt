# Рекурсивно собираем файлы с текущего каталога
file(GLOB_RECURSE TARGET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# Добавляем исполняемый файл
add_executable(parser-slave ${TARGET_SRC})
target_link_libraries(parser-slave common)