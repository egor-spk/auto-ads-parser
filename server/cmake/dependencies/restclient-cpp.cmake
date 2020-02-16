include(FetchContent)

set(NAME "restclient-cpp")
set(TAG "master")

FetchContent_Declare(${NAME}
        GIT_REPOSITORY https://github.com/mrtazz/restclient-cpp.git
        GIT_TAG ${TAG}
        )

FetchContent_GetProperties(${NAME})
string(TOLOWER ${NAME} lcName)
if (NOT ${lcName}_POPULATED)
    FetchContent_Populate(${NAME})

    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

    add_subdirectory(${${lcName}_SOURCE_DIR} ${${lcName}_BINARY_DIR} EXCLUDE_FROM_ALL)

    set(RESTCLIENT_INCLUDE_DIRS ${${lcName}_SOURCE_DIR}/include ${${lcName}_BINARY_DIR}/include)
endif ()