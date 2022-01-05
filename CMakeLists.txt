cmake_minimum_required(VERSION 3.20)
project(baskvm)

set(CMAKE_CXX_STANDARD 17)

add_executable(baskvm src/baskvm.cpp src/inbuilt.cpp)

set(CMAKE_BUILD_TYPE Debug)