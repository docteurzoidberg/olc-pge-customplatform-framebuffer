cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0077 NEW)

project(testpgefb VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -O2)

set(CMAKE_C_STANDARD 99)

# Set build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

#set compile definitions for fbgraphics
add_compile_definitions(
    WITHOUT_PNG
    WITHOUT_JPEG
    WITHOUT_STB_IMAGE
)

set(FBGRAPHICS_LIB_DIR ${CMAKE_SOURCE_DIR}/lib/fb)
MESSAGE(STATUS "CMAKE_BUILDFBGRAPHICS_LIB_DIR_TYPE: ${FBGRAPHICS_LIB_DIR}")

# Include the FBGraphics include directory
include_directories(${FBGRAPHICS_LIB_DIR}/include)

#Include the additional FBG's source directories
file(GLOB FBG_SOURCES ${FBGRAPHICS_LIB_DIR}/src/*.c)

set(SOURCES main.cpp)

list(APPEND SOURCES ${FBG_SOURCES})

# Define the executable source files
add_executable(${PROJECT_NAME} ${SOURCES})

# Threads
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
include_directories(${Threads_INCLUDE_DIRS})

find_package(PNG REQUIRED)
target_link_libraries(${PROJECT_NAME} PNG::PNG)
include_directories(${PNG_INCLUDE_DIRS})


