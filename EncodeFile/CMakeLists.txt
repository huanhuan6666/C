cmake_minimum_required(VERSION 3.21)
project(EncodeFile)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE Debug)
add_executable(EncodeFile test.cpp des.h des.cpp)
