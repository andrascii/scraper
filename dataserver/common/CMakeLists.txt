cmake_minimum_required(VERSION 3.16)

include(make_static_library_sanitized_target)

set(THIS_TARGET_NAME ${COMMON_LIB})
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
list(APPEND ADDITIONAL_INCLUDE_DIRECTORIES "${INCLUDES_DIR}")
list(APPEND ADDITIONAL_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}")

#
# deps
#
list(APPEND DEPS ${Boost_LIBRARIES})
list(APPEND DEPS spdlog::spdlog)
list(APPEND DEPS tl::expected)

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
add_library(${THIS_TARGET_NAME} STATIC ${HEADERS_LIST} ${SOURCES_LIST})

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

#
# creating sanitized version of this target to check UB
#
make_static_library_sanitized_target(
  TARGET_NAME ${THIS_TARGET_NAME}_sanitized
  SOURCES ${SOURCES_LIST}
  HEADERS ${HEADERS_LIST}
  DEPS ${DEPS}
  PRECOMPILED_HEADER ${PRECOMPILED_HEADER}
)
