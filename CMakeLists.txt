cmake_minimum_required(VERSION 3.16)

project(slang)

set(CMAKE_CXX_STANDARD 17)

add_executable(slang src/main.cpp src/config.cpp src/vm/inbuilt.cpp src/vm/stack.cpp src/vm/byte_ops.cpp src/vm/vm.cpp src/compiler/compiler.cpp src/compiler/functions.cpp src/compiler/lexer.cpp src/compiler/pre_process.cpp src/compiler/code_gen.cpp src/compiler/macros.cpp src/compiler/string_ops.cpp)

set(CMAKE_BUILD_TYPE Debug)