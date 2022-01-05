#include "baskvm.h"

using namespace std;

BaskVM::BaskVM() {
    this->stack = new Stack();
    this->code = std::vector<BYTE>();
    init_inbuilts();
}

void BaskVM::load(char* fname) {
    string filename = fname;
    ifstream file(filename, ios::in | ios::binary);
    char buffer[BUFFER_SIZE];
    file.read(buffer, BUFFER_SIZE);
    for (char chr : buffer) {
        this->code.push_back((uint8_t)chr);
    }
}

void BaskVM::exec() {
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

void BaskVM::read_const() {
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

void BaskVM::eval()
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