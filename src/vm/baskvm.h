#ifndef BASK_VM_H
#define BASK_VM_H

#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <array>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <cstdio>
#include <filesystem>
#include "../datatypes.h"
#include "../byte_ops.h"
#include "../opcode.h"
#include "stack.h"
#include "logger.h"
#include "../values.h"
#include "inbuilt.h"
#include "../input_parser.h"

// 10kB
#define BUFFER_SIZE 1024 * 10

#define GET_CONSTANT() constants[GET_WORD()]

class BaskVM
{
public:
    BaskVM();

    std::string read_string();
    std::vector<BYTE> load(const char* fname);
    void exec(int argc, char *argv[]);
    void read_consts();
    void read_labels();
    void read_funcs(int num);
    void eval();

    BYTE read_byte();

    std::string name;
    WORD ip;
    WORD code_start;
    std::map<std::string, uint64_t> labels;
    Stack* default_stack;
    Stack* current_stack;
    std::map<std::string, Stack*> symbol_table;
    std::map<std::string, std::vector<BYTE>> functions;

    std::vector<Value *> constants;
    std::vector<BYTE> code;
};

#endif