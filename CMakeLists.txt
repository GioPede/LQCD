# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# Project's name
project(LatticeQCD)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# The following folder will be included
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add all src
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(SHARED_SRC ${PROJECT_SOURCE_DIR}/ParallelTools/parallel.cpp)

# Set compiler and flags
set(CMAKE_CXX_COMPILER mpic++)
set(CMAKE_CXX_STANDARD 11)

# Set executables
add_executable(pgfg ${PROJECT_SOURCE_DIR}/Apps/pgfg.cpp ${SHARED_SRC})
