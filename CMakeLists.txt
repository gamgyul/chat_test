# Cmake Minimum requirement
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(PROJ_HOME $ENV{CHAT_HOME})

set(CMAKE_C_COMPILER_ID "Clang")
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")

# Project name
PROJECT(GAMGYUL_CHAT)

# prohibit in-source-builds
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR
    " In-source builds not allowed. Please make a new directory (e.g. called build) and run CMake from there.\n \n"
    " CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}\n"
    " cannot be\n"
    " CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}\n \n"
    " However, before that, you have to remove the already created files,\n  e.g. with `rm -rf CMakeCache.txt CMakeFiles/`")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJ_HOME}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJ_HOME}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJ_HOME}/bin)

FIND_PACKAGE(Boost 1.67 REQUIRED)
FIND_PACKAGE(Protobuf REQUIRED)

add_subdirectory(src)
