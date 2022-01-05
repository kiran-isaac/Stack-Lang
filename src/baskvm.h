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
        constants.push_back(new NUMBER(1));
        code = {
            0x01,
            0x00,
            0x00,
            0x00,
            0x00,
            0x01,
            0x00,
            0x00,
            0x00,
            0x01,
            OP_FUNC_CALL,
            FUNC_CALL_MODE_INBUILT,
            'p',
            'r', 
            'i', 
            'n', 
            't',
            0x00,
            0x00
        };

        ip = &code[0];

        eval();
    }

    void eval()
    {
        while (true)
        {
            auto opcode = READ_BYTE();
            switch (opcode)
            {
            case OP_HALT:
                return;
            case OP_CONST:
                stack->push(GET_CONSTANT());
                break;
            case OP_FUNC_CALL:
                switch (READ_BYTE())
                {
                case 0x00:
                    uint8_t byte;
                    std::vector<uint8_t> string;
                    do {
                        byte = READ_BYTE();
                        string.push_back(byte);
                    } while (byte != 0x00);
                    string.pop_back();
                    call_inbuilt(string, stack);
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