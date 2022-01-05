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
#include "datatypes.h"
#include "byte_ops.h"
#include "opcode.h"
#include "stack.h"
#include "logger.h"
#include "values.h"
#include "inbuilt.h"

#define BUFFER_SIZE 1000

#define READ_BYTE() *ip++
#define READ_BYTES(n) {for (int i = 0; i < n; i++) {READ_BYTE();}}
#define GET_CONSTANT() constants[GET_WORD()]

class BaskVM
{
public:
    BaskVM();

    void load(char* fname);

    void exec();

    void read_const();

    void eval();

    BYTE *ip;
    Stack* stack;

    std::vector<Value *> constants;
    std::vector<BYTE> code;
};

#endif