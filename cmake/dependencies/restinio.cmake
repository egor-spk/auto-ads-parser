include(ExternalProject)

set(RESTINIO_VER "0.6.3")
set(SOBJECTIZER_VER "5.5.24.3")
set(RESTINIO_SRC_DIR "${FETCHCONTENT_BASE_DIR}/restinio-src")
set(RESTINIO_INST_DIR "${FETCHCONTENT_BASE_DIR}/restinio-build")
set(FMT_LIB "libfmt.a")
set(HTTP_PARSER_LIB "libhttp_parser.a")
set(SOBJECTIZER_LIB "libso_s.${SOBJECTIZER_VER}.a")
set(FMT_LIBRARY "${RESTINIO_SRC_DIR}/fmt/${FMT_LIB}")
set(HTTP_PARSER_LIBRARY "${RESTINIO_SRC_DIR}/nodejs/http_parser/${HTTP_PARSER_LIB}")
set(SOBJECTIZER_LIBRARY "${RESTINIO_SRC_DIR}/so_5/${SOBJECTIZER_LIB}")
set(ZLIB_LIBRARY "${RESTINIO_SRC_DIR}/restinio/third_party/zlib/libz.a")
set(ZLIB_LIBRARY_DIR "${RESTINIO_SRC_DIR}/dev/restinio/third_party/zlib")

# restinio
ExternalProject_Add(restinio
        PREFIX ${RESTINIO_INST_DIR}
        INSTALL_DIR ${RESTINIO_INST_DIR}
        URL https://github.com/Stiffstream/restinio/releases/download/v.${RESTINIO_VER}/restinio-${RESTINIO_VER}-full.tar.bz2
        SOURCE_DIR ${RESTINIO_SRC_DIR}
        UPDATE_COMMAND ""
        CONFIGURE_COMMAND cmake <SOURCE_DIR>/dev
        INSTALL_COMMAND ""
        BUILD_IN_SOURCE TRUE
        EXCLUDE_FROM_ALL TRUE
        LOG_CONFIGURE TRUE
        LOG_BUILD TRUE
        LOG_INSTALL TRUE)

set(RESTINIO_INCLUDE_DIR "${RESTINIO_SRC_DIR}/dev/restinio")
set(FMT_INCLUDE_DIR "${RESTINIO_SRC_DIR}/dev/fmt/include")
set(ASIO_INCLUDE_DIR "${RESTINIO_SRC_DIR}/dev/asio/include")
set(HTTP_PARSER_INCLUDE_DIR "${RESTINIO_SRC_DIR}/dev/nodejs/http_parser")
include_directories(${RESTINIO_INCLUDE_DIR} ${ASIO_INCLUDE_DIR}
        ${FMT_INCLUDE_DIR} ${HTTP_PARSER_INCLUDE_DIR} "${RESTINIO_SRC_DIR}/dev" ${ZLIB_LIBRARY_DIR})

set(RESTINIO_LIBS ${FMT_LIBRARY} ${HTTP_PARSER_LIBRARY} ${SOBJECTIZER_LIBRARY} ${ZLIB_LIBRARY})

add_definitions(-DASIO_STANDALONE -DASIO_HAS_STD_CHRONO)
