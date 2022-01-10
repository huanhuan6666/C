cmake_minimum_required(VERSION 3.21)
project(TestConfig)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
add_executable(TestConfig test.cpp config_op.h config_op.cpp)
