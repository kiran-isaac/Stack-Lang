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
    BaskVM(std::vector<BYTE> code, Stack* stack) {
        this->stack = stack;
        this->code = code;
    }

    void exec()
    {
        // code = {
        //          // 2 constants
        //     0x00,
        //     0x00,
        //     0x00,
        //     0x02,

        //          // Const 1
        //     0x00,
        //     0x42,
        //     0xc8,
        //     0x33,
        //     0x33,

        //          // Const 2
        //     0x00,
        //     0x80,
        //     0x00,
        //     0x00,
        //     0x00,

        //          // Push const 0
        //     0x01,
        //     0x00,
        //     0x00,
        //     0x00,
        //     0x00,

        //          // Push const 1
        //     0x01,
        //     0x00,
        //     0x00,
        //     0x00,
        //     0x01,

        //     0x02,
        //     0x00,
        //          // Func name
        //     0x70
        //     0x72 
        //     0x69 
        //     0x6E 
        //     0x74
        //     0x00,
        //          // Push const 1
        //     0x01,
        //     0x00,
        //     0x00,
        //     0x00,
        //     0x00,
        //
        //     0x02,
        //     0x00,
        //          // Func name
        //     0x65 
        //     0x78 
        //     0x69 
        //     0x74
        //     0x00
        // };

        ip = &code[0];

        read_const();
        eval();
    }

    void read_const() {
        uint32_t const_num = GET_WORD();
        for (int i = 0; i < const_num; i++) {
            switch (READ_BYTE()) {
                case DT_NUM:
                    uint32_t value = GET_WORD();
                    float f;
                    memcpy(&f, &value, 4);
                    constants.push_back(new NUMBER(f));
                    break;
            }
        }
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
                    BYTE byte;
                    std::vector<BYTE> string;
                    do {
                        byte = READ_BYTE();
                        string.push_back(byte);
                    } while (byte != 0x00);
                    string.pop_back();
                    call_inbuilt(string, stack);
                    break;
                }
                break;
            default:
                FAIL << "Unknown Opcode: " << std::hex << opcode;
            }
        }
    }

    BYTE *ip;
    Stack* stack;

    std::vector<Value *> constants;
    std::vector<BYTE> code;
};

#endif