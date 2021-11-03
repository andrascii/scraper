cmake_minimum_required(VERSION 3.16)

include(make_coverage_target)
include(make_executable_sanitized_target)

set(THIS_TARGET_NAME ${TESTS_BIN})
project(${THIS_TARGET_NAME})

#
# deps include directories
#
list(APPEND ADDITIONAL_INCLUDE_LIST "${CMAKE_CURRENT_SOURCE_DIR}/include")
list(APPEND ADDITIONAL_INCLUDE_LIST "${CMAKE_SOURCE_DIR}/${API_LIB}/include")
list(APPEND ADDITIONAL_INCLUDE_LIST "${CMAKE_SOURCE_DIR}/${COMMON_LIB}/include")
list(APPEND ADDITIONAL_INCLUDE_LIST "${CMAKE_SOURCE_DIR}/deps/tl/")
list(APPEND ADDITIONAL_INCLUDE_LIST "${Boost_INCLUDE_DIRS}")

#
# current target sources directories
#
set(SOURCES_DIR src)
set(INCLUDES_DIR include)

#
# Precompiled header
#
set(PRECOMPILED_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/stdafx.h")

#
# deps
#
list(APPEND DEPS ${API_LIB})
list(APPEND DEPS ${COMMON_LIB})
list(APPEND DEPS ${Boost_LIBRARIES})
list(APPEND DEPS spdlog::spdlog)
list(APPEND DEPS GTest::gtest)
list(APPEND DEPS GTest::gtest_main)

#
# collecting sources and headers
#
file(GLOB_RECURSE SOURCES_LIST "${SOURCES_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS_LIST "${INCLUDES_DIR}/*.h")

#
# adding precompiled header into specific folder
#
source_group("Precompiled Headers" FILES "include/stdafx.h")

#
# creating target
#
add_executable(${THIS_TARGET_NAME} ${SOURCES_LIST} ${HEADERS_LIST})

#
# adding include directories to created target
#
include_directories(${ADDITIONAL_INCLUDE_LIST})

#
# adding precompiled header
#
target_precompile_headers(${THIS_TARGET_NAME} PRIVATE ${PRECOMPILED_HEADER})

#
# linking this target with other targets
#
target_link_libraries(${THIS_TARGET_NAME} PRIVATE ${DEPS})

#
# creating new tests target with clang's coverage flags
# to test code coverage
#
make_coverage_target(
  TARGET_NAME ${THIS_TARGET_NAME}
)

list(APPEND TESTS_SANITIZED_DEPS ${DEPS})
list(REMOVE_ITEM TESTS_SANITIZED_DEPS ${API_LIB})
list(REMOVE_ITEM TESTS_SANITIZED_DEPS ${COMMON_LIB})
list(APPEND TESTS_SANITIZED_DEPS ${COMMON_LIB}_sanitized)
list(APPEND TESTS_SANITIZED_DEPS ${API_LIB}_sanitized)

#
# creating sanitized version of this target to check UB
#
make_executable_sanitized_target(
  TARGET_NAME ${THIS_TARGET_NAME}-sanitized
  SOURCES ${SOURCES_LIST}
  HEADERS ${HEADERS_LIST}
  DEPS ${TESTS_SANITIZED_DEPS}
  PRECOMPILED_HEADER ${PRECOMPILED_HEADER}
)

#
# register test cases added by gtest (to start them - run 'test' target)
#
gtest_discover_tests(${THIS_TARGET_NAME})

register_for_clang_format(
  TARGET_NAME ${THIS_TARGET_NAME}
  SOURCES_LIST ${HEADERS_LIST} ${SOURCES_LIST}
)