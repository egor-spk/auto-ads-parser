include(FetchContent)

set(NAME "gumbo-query")
set(TAG "master")

FetchContent_Declare(${NAME}
        GIT_REPOSITORY https://github.com/lazytiger/gumbo-query.git
        GIT_TAG ${TAG}
        )

FetchContent_GetProperties(${NAME})
string(TOLOWER ${NAME} lcName)
if (NOT ${lcName}_POPULATED)
    FetchContent_Populate(${NAME})

    add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR} EXCLUDE_FROM_ALL)
    set(GUMBO_QUERY_INCLUDE_DIRS ${${lcName}_SOURCE_DIR}/src)
endif ()