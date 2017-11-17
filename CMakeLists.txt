# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.1)

# Project's name
project(LatticeQCD)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_CACHEFILE_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# Compiler options
set(QMC_MPI 1)
set(CMAKE_CXX_COMPILER mpic++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

# The following folder will be included
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/user_extensions/include)
include_directories(/opt/software/boost/1.58.0--GCC-4.9/include)

# Core library
add_executable(LatticeQCD "src/CMakeLists.txt")
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# User extensions
add_library(LatticeQCDUserExtensions "user_extensions/CMakeLists.txt")
add_subdirectory(${CMAKE_SOURCE_DIR}/user_extensions)

# Link core and user-extensions
target_link_libraries(LatticeQCD LatticeQCDUserExtensions "-lboost_filesystem -lboost_system")

