### This file is used for build example projects.

# set this will supress some warnings
set(BUILDING_SDK "yes" CACHE INTERNAL "")

# basic config
if (NOT PROJ)
    message(FATAL_ERROR "PROJ must be set. e.g. -DPROJ=hello_world")
endif ()
cmake_minimum_required(VERSION 3.0)
include(./cmake/common.cmake)
project(${PROJ})

# config self use headers
include(./cmake/macros.internal.cmake)
header_directories(${SDK_ROOT}/lib)
# build library first
add_subdirectory(lib)

# TODO
#add_subdirectory(third_party)

# compile project
add_custom_target(build_metis ALL
        COMMAND ${CMAKE_MAKE_PROGRAM}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/openmv/src/micropython/ports/k210-standalone
        COMMENT "Original metis makefile target")
add_source_files(src/${PROJ}/main/*.c)
include(./cmake/executable.cmake)

