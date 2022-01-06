cmake_minimum_required(VERSION 3.20)
project(baskvm)

set(CMAKE_CXX_STANDARD 17)

add_executable(baskvm src/main.cpp src/vm/inbuilt.cpp src/vm/byte_ops.cpp src/vm/baskvm.cpp src/compiler/compiler.cpp)

set(CMAKE_BUILD_TYPE Debug)