cmake_minimum_required(VERSION 3.9)
project(onmodels DESCRIPTION "run the on model on various graphs")

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_C_STANDARD 11)

include(GNUInstallDirs)

add_subdirectory(src)
add_subdirectory(lib)