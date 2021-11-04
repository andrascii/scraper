cmake_minimum_required(VERSION 3.16)

#
# by default generates project to build release package
#
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

option(USE_CLANG "Enables using clang as a compiler" ON)
option(BUILD_TESTS "Enables building tests project" OFF)

#
# by default on UNIX systems we are using clang compiler
#
if (UNIX AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" AND USE_CLANG)
  set(CMAKE_C_COMPILER "clang")
  set(CMAKE_CXX_COMPILER "clang++")
endif()

project("Scraper" LANGUAGES C CXX)

#
# adding another one path to find .cmake files
#
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#
# adding settings for cl/clang compiler (depends on which compiler actully used)
#
include(clang_options)
include(msvc_options)

#
# function to register source files for reformatting by clang-format
#
include(add_clang_format)

message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

#
# package version
#
set(VERSION_MAJOR 1 CACHE STRING "Major version #" FORCE)
set(VERSION_MINOR 0 CACHE STRING "Minor version #" FORCE)
set(VERSION_PATCH 0 CACHE STRING "Patch version #" FORCE)

#
# C++ compiler settings
#
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

#
# deps packages
#
include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

find_path(
  CPPKAFKA_INCLUDES
  NAMES cppkafka.h
  HINTS ${CURRENT_INSTALLED_DIR}
  PATH_SUFFIXES cppkafka
)

find_library(
  CPPKAFKA_LIBRARY_DEBUG
  NAMES cppkafka
  PATH_SUFFIXES lib
  PATHS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug"
  NO_DEFAULT_PATH REQUIRED
)

find_library(
  CPPKAFKA_LIBRARY_RELEASE
  NAMES cppkafka
  PATH_SUFFIXES lib
  PATHS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}"
  NO_DEFAULT_PATH REQUIRED
)

select_library_configurations(CPPKAFKA)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS program_options REQUIRED)
find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(simdjson CONFIG REQUIRED)
find_package(RdKafka CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(tl-expected CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

#
# project variables
#
set(CORE_LIB core)
set(SCHEDULER_BIN scheduler)
set(TESTS_BIN tests)
set(COMMON_LIB common)

add_subdirectory(${COMMON_LIB})
add_subdirectory(${CORE_LIB})
add_subdirectory(${SCHEDULER_BIN})

if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(${TESTS_BIN})
  message(STATUS "Tests enabled and will be compiled")
endif()

#
# collecting sources to register them for reformat by clang-format utility
#
foreach(CONCRETE_TARGET IN ITEMS ${CORE_LIB} ${SCHEDULER_BIN} ${COMMON_LIB})
  get_property(SOURCES TARGET ${CONCRETE_TARGET} PROPERTY SOURCES_PROPERTY)
  list(APPEND REFORMAT_SOURCES ${SOURCES})
endforeach()

if (BUILD_TESTS)
  get_property(SOURCES TARGET ${TESTS_BIN} PROPERTY SOURCES_PROPERTY)
  list(APPEND REFORMAT_SOURCES ${SOURCES})
endif()

#
# creating target 'reformat' to trigger calling clang-format utility for each source/header file in the project
#
add_clang_format(
  TARGET_NAME reformat
  SOURCES_LIST ${REFORMAT_SOURCES}
)