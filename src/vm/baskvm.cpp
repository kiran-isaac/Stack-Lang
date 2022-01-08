#include "baskvm.h"

using namespace std;

BaskVM::BaskVM() {
    default_stack = new Stack("default");
    current_stack = default_stack;
    symbol_table = new std::map<std::string, Stack*>;
    (*symbol_table)["default"] = default_stack;
    code = std::vector<BYTE>();
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
    //         // 2 constants
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x04,

    //         // Const 1
    //     0x00,
    //     0x42,
    //     0xc8,
    //     0x33,
    //     0x33,

    //         // Const 2
    //     0x01,
    //     65,
    //         // Const 3
    //     0x01,
    //     '\n',

    //         //Const 4
    //     0x00,
    //     0x3F,
    //     0x80,
    //     0x00,
    //     0x00,

    //     OP_CREATE,
    //     'x',
    //     0x00,

    //     OP_SWITCH,
    //     'x',
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
    //     0x70,
    //     0x72,
    //     0x69,
    //     0x6E,
    //     0x74,
    //     0x00,
        
    //     // Add newline
    //     0x01,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x02,

    //     // Add char
    //     0x01,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x03,
    //     0x02,
    //     0x00,
    //     0x70,
    //     0x72,
    //     0x69,
    //     0x6E,
    //     0x74,
    //     0x00,

    //          // Exit
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
                {
                    uint32_t value = GET_WORD();
                    constants.push_back(new Value({DT_NUM, value}));
                }
                break;
            case DT_CHAR:
                {
                    char value = (char)READ_BYTE();
                    constants.push_back(new CHAR(value));
                }
                break;
        }
    }
}

string BaskVM::read_string() {
    BYTE byte;
    vector<BYTE> bytes;
    do {
        byte = READ_BYTE();
        bytes.push_back(byte);
    } while (byte != 0x00);
    bytes.pop_back();
    return bytes_to_string(bytes);
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
            current_stack->push(GET_CONSTANT());
            break;
        case OP_CREATE:
            {
                string name = read_string();
                if (symbol_table->find(name) != symbol_table->end()) {
                    FAIL << "Stack '" << name << "' already exists";
                }
                (*symbol_table)[name] = new Stack(name);
            }
            break;
        case OP_SWITCH:
            {
                string name = read_string();
                if (symbol_table->find(name) == symbol_table->end()) {
                    FAIL << "Invalid stack identifier: " << name;
                }
                current_stack = ((*symbol_table)[name]);
            }
            break;
        case OP_FUNC_CALL:
            switch (READ_BYTE())
            {
            case FUNC_CALL_MODE_INBUILT:
                call_inbuilt(read_string(), current_stack);
                break;
            }
            break;
        default:
            FAIL << "Unknown Opcode: " << std::hex << opcode;
        }
    }
}