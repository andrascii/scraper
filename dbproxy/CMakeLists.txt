cmake_minimum_required(VERSION 3.16)

set(THIS_TARGET_NAME ${DB_PROXY_BIN})
project(${THIS_TARGET_NAME})

#
# current target sources directories
#
set(SOURCES_DIR src)
set(INCLUDES_DIR include)

#
# precompiled header
#
set(PRECOMPILED_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/stdafx.h")

#
# deps include directories
#
list(APPEND ADDITIONAL_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/${API_LIB}/include")
list(APPEND ADDITIONAL_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/${COMMON_LIB}/include")
list(APPEND ADDITIONAL_INCLUDE_DIRECTORIES "${INCLUDES_DIR}")
list(APPEND ADDITIONAL_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}")

#
# target deps
#
list(APPEND DEPS ${API_LIB})
list(APPEND DEPS ${COMMON_LIB})
list(APPEND DEPS ${Boost_LIBRARIES})
list(APPEND DEPS ${CPPKAFKA_LIBRARY})
list(APPEND DEPS spdlog::spdlog)
list(APPEND DEPS cxxopts::cxxopts)
list(APPEND DEPS PostgreSQL::PostgreSQL)
list(APPEND DEPS libpqxx::pqxx)
list(APPEND DEPS simdjson::simdjson)
#list(APPEND DEPS simdjson::simdjson-flags)
#list(APPEND DEPS simdjson::simdjson-headers)
list(APPEND DEPS RdKafka::rdkafka)
list(APPEND DEPS RdKafka::rdkafka++)
list(APPEND DEPS OpenSSL::SSL)
list(APPEND DEPS OpenSSL::Crypto)
list(APPEND DEPS fmt::fmt)
list(APPEND DEPS tl::expected)
list(APPEND DEPS nlohmann_json)
list(APPEND DEPS nlohmann_json::nlohmann_json)

#
# collecting sources and headers
#
aux_source_directory(${SOURCES_DIR} SOURCES_LIST)
file(GLOB_RECURSE HEADERS_LIST "${INCLUDES_DIR}/*.h")

#
# adding precompiled header into specific folder
#
source_group("Precompiled Headers" FILES ${PRECOMPILED_HEADER})

#
# creating target
#
add_executable(${THIS_TARGET_NAME} ${HEADERS_LIST} ${SOURCES_LIST})

#
# adding include directories to created target
#
include_directories(${ADDITIONAL_INCLUDE_DIRECTORIES})

#
# adding precompiled header
#
target_precompile_headers(${THIS_TARGET_NAME} PRIVATE ${PRECOMPILED_HEADER})

#
# linking this target with other targets
#
target_link_libraries(${THIS_TARGET_NAME} PRIVATE ${DEPS})
