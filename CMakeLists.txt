cmake_minimum_required(VERSION 3.16)

option(USE_CLANG "Enables using clang as a compiler" ON)
option(BUILD_TESTS "Enables building tests project" OFF)

if (UNIX AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT CMAKE_CXX_COMPILER_ID MATCHES "AppleClang" AND USE_CLANG)
  set(CMAKE_C_COMPILER "clang")
  set(CMAKE_CXX_COMPILER "clang++")
endif()

project("Scraper" LANGUAGES C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(clang_options)
include(msvc_options)

message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

set(VERSION_MAJOR 0 CACHE STRING "Major version #" FORCE)
set(VERSION_MINOR 1 CACHE STRING "Minor version #" FORCE)
set(VERSION_PATCH 0 CACHE STRING "Patch version #" FORCE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

#
# deps packages
#
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS thread program_options REQUIRED)
find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(simdjson CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Sql CONFIG REQUIRED)

#
# project variables
#
set(API_LIB api)
set(DB_PROXY_BIN dbproxy)
set(TESTS_BIN tests)
set(COMMON_LIB common)

add_subdirectory(${COMMON_LIB})
add_subdirectory(${API_LIB})
add_subdirectory(${DB_PROXY_BIN})

if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(${TESTS_BIN})
  message(STATUS "Tests enabled and will be compiled")
endif()

include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "TGZ;RPM")
set(CPACK_PACKAGE_NAME "Scraper")
set(CPACK_PACKAGE_CONTACT "apugachevdev@gmail.com")
set(CPACK_PACKAGE_VENDOR "Andrey Pugachev")
set(CPACK_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_COMPONENT_DISPLAY_NAME "Scraper")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}.x86_64")
set(CPACK_PACKAGE_DESCRIPTION "Scraper Release")
set(CPACK_PACKAGE_CHECKSUM SHA256)
set(CPACK_INSTALL_CMAKE_PROJECTS "${CMAKE_CURRENT_BINARY_DIR};CMake;ALL;/")
set(CPACK_PACKAGING_INSTALL_PREFIX "/")
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/bin")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/bin" isSystemDir)

if("${isSystemDir}" STREQUAL "-1")
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/bin")
endif("${isSystemDir}" STREQUAL "-1")

include(CPack)
