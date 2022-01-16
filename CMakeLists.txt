cmake_minimum_required(VERSION 3.16)

project(bask)

set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.50 REQUIRED COMPONENTS program_options)
message(STATUS "Boost version: ${Boost_VERSION}")
include_directories(${Boost_INCLUDE_DIR})

add_executable(bask src/main.cpp src/vm/inbuilt.cpp src/vm/stack.cpp src/vm/byte_ops.cpp src/vm/baskvm.cpp src/compiler/compiler.cpp src/compiler/functions.cpp src/compiler/lexer.cpp src/compiler/pre_process.cpp src/compiler/code_gen.cpp src/compiler/macros.cpp src/compiler/string_ops.cpp)

target_link_libraries(bask PUBLIC ${Boost_LIBRARIES})

set(CMAKE_BUILD_TYPE Debug)