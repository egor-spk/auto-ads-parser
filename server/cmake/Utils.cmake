# @brief Добавляет все cpp/h файлы из текущей директории в цель
# @param TARGET имя цели
function(add_executable_from_cur_dir TARGET)
    # Рекурсивно собираем файлы с текущего каталога
    file(GLOB_RECURSE TARGET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
    # Добавляем исполняемый файл
    add_executable(${TARGET} ${TARGET_SRC})
endfunction()

# @brief Добавляет все cpp/h файлы из указанных директорий в цель
# @param TARGET имя цели
# @param DIRS список абсолютных путей до директорий
function(target_sources_from_dirs TARGET)
    cmake_parse_arguments(PARSE_ARGV 0 ARGS "" "" "DIRS")
    # Рекурсивно собираем файлы с каждого каталога из ARGS_DIRS
    set(TARGET_SOURCES)
    foreach (DIR ${ARGS_DIRS})
        file(GLOB_RECURSE TARGET_SRC "${DIR}/*.cpp" "${DIR}/*.h")
        set(TARGET_SOURCES ${TARGET_SOURCES} ${TARGET_SRC})
    endforeach ()
    # Добавляем к цели
    target_sources(${TARGET} PRIVATE ${TARGET_SOURCES})
endfunction()