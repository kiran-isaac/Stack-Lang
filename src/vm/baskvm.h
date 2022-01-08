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
#include "../datatypes.h"
#include "../byte_ops.h"
#include "../opcode.h"
#include "stack.h"
#include "logger.h"
#include "../values.h"
#include "inbuilt.h"

// 10kB
#define BUFFER_SIZE 1024 * 10

#define GET_CONSTANT() constants[GET_WORD()]

class BaskVM
{
public:
    BaskVM();

    std::string read_string();

    void load(char* fname);

    void exec();

    void read_const();

    void eval();

    BYTE *ip;
    Stack* default_stack;
    Stack* current_stack;
    std::map<std::string, Stack*>* symbol_table;

    std::vector<Value *> constants;
    std::vector<BYTE> code;
};

#endif