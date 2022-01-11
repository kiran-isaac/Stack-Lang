project(bask)

set(CMAKE_CXX_STANDARD 17)

add_executable(bask src/main.cpp src/vm/inbuilt.cpp src/vm/stack.cpp src/vm/byte_ops.cpp src/vm/baskvm.cpp src/compiler/compiler.cpp src/compiler/functions.cpp src/compiler/lexer.cpp src/compiler/pre_process.cpp src/compiler/code_gen.cpp src/compiler/macros.cpp src/compiler/string_ops.cpp)

set(CMAKE_BUILD_TYPE Debug)