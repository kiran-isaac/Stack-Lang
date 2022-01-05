#ifndef BASK_VM_H
#define BASK_VM_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include "byte_ops.h"
#include "opcode.h"
#include "stack.h"
#include "logger.h"
#include "values.h"
#include "inbuilt.h"

#define READ_BYTE() *ip++
#define READ_BYTES(n) {for (int i = 0; i < n; i++) {READ_BYTE();}}
#define GET_CONSTANT() constants[GET_WORD()]

class BaskVM
{
public:
    BaskVM(Stack* stack) {
        this->stack = stack;
    }

    void exec()
    {
        constants.push_back(new NUMBER(100.1));
        code = {
            0x01,
            0x00,
            0x00,
            0x00,
            0x00,
            OP_FUNC_CALL,
            0x00,
            0x70,
            0x72, 
            0x69, 
            0x6e, 
            0x74,
            0x00,
            0x00
        };

        ip = &code[0];

        eval();
    }

    Value eval()
    {
        while (true)
        {
            auto opcode = READ_BYTE();
            switch (opcode)
            {
            case OP_HALT:
                return *(stack->pop());
            case OP_CONST:
                stack->push(GET_CONSTANT());
                break;
            case OP_FUNC_CALL:
                switch (READ_BYTE())
                {
                case 0x00:
                    auto is_null = [](uint8_t i){ return i == 0x00; };
                    auto p = std::partition_point(code.begin(), code.end(), is_null);
                    std::vector<uint8_t> string(this->code.begin(), p);
                    call_inbuilt(string)
                    READ_BYTES(string.size());
                    break;
                }
                break;
            default:
                DIE << "Unknown Opcode: " << std::hex << opcode;
            }
        }
    }

    uint8_t *ip;
    Stack* stack;

    std::vector<Value *> constants;
    std::vector<uint8_t> code;
};

#endif