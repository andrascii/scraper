cmake_minimum_required(VERSION 3.16)

include(make_coverage_target)
include(make_executable_sanitized_target)

set(THIS_TARGET_NAME ${ECHO_LIB_TESTS})
project(${THIS_TARGET_NAME})

#
# deps include directories
#
set(ECHO_LIB_INCLUDE_DIR "../${ECHO_LIB}/include")
set(COMMON_LIB_INCLUDE_DIR "../${COMMON_LIB}/include")
set(EXPECTED_LIB_INCLUDE_DIR "../tl/")

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
list(APPEND DEPS ${ECHO_LIB})
list(APPEND DEPS ${COMMON_LIB})
list(APPEND DEPS ${Boost_LIBRARIES})
list(APPEND DEPS spdlog::spdlog)
list(APPEND DEPS GTest::gtest)
list(APPEND DEPS GTest::gtest_main)

#
# collecting sources and headers
#
aux_source_directory(${SOURCES_DIR} SOURCES_LIST)
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
include_directories(${INCLUDES_DIR})
include_directories(${ECHO_LIB})
include_directories(${COMMON_LIB_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${EXPECTED_LIB_INCLUDE_DIR})

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

list(APPEND ECHO_LIB_TESTS_SANITIZED_DEPS ${DEPS})
list(REMOVE_ITEM ECHO_LIB_TESTS_SANITIZED_DEPS ${ECHO_LIB})
list(REMOVE_ITEM ECHO_LIB_TESTS_SANITIZED_DEPS ${COMMON_LIB})
list(APPEND ECHO_LIB_TESTS_SANITIZED_DEPS ${COMMON_LIB}_sanitized)
list(APPEND ECHO_LIB_TESTS_SANITIZED_DEPS ${ECHO_LIB}_sanitized)

#
# creating sanitized version of this target to check UB
#
make_executable_sanitized_target(
  TARGET_NAME ${THIS_TARGET_NAME}_sanitized
  SOURCES ${SOURCES_LIST}
  HEADERS ${HEADERS_LIST}
  DEPS ${ECHO_LIB_TESTS_SANITIZED_DEPS}
  PRECOMPILED_HEADER ${PRECOMPILED_HEADER}
)

#
# register test cases added by gtest (to start them - run 'test' target)
#
gtest_discover_tests(${THIS_TARGET_NAME})
