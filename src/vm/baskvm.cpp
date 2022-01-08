#include "baskvm.h"

using namespace std;

BYTE BaskVM::read_byte() {
    ip++;
    return code[ip - 1];
}

BaskVM::BaskVM() {
    labels = new map<string, WORD>;
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
    // // 2 constants
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x02,

    // // Const 1: 1
    //     0x00,
    //     0x3F,
    //     0xF0,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,

    // // Const 2: 0
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,

    // // 1 label
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x01,
    
    //     'm', 'a', 'i', 'n', 0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,

    // // Push const 1
    //     OP_CONST,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,

    // // Push const 2
    //     OP_CONST,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x00,
    //     0x01,

    //     OP_FUNC_CALL,
    //     FUNC_CALL_MODE_INBUILT,
    //     'p',
    //     'r',
    //     'i',
    //     'n',
    //     't',
    //     0x00,

    //     OP_GOTO,
    //     'm', 'a', 'i', 'n', 0x00,
    // };

    ip = 0;

    read_const();
    read_label();

    code_start = ip;
    eval();
}

void BaskVM::read_const() {
    WORD const_num = GET_WORD();
    for (int i = 0; i < const_num; i++) {
        switch (read_byte()) {
            case DT_NUM:
                {
                    WORD value = GET_WORD();
                    constants.push_back(new Value({DT_NUM, value}));
                }
                break;
            case DT_CHAR:
                {
                    char value = (char)read_byte();
                    constants.push_back(new CHAR(value));
                }
                break;
        }
    }
}

void BaskVM::read_label() {
    WORD const_num = GET_WORD();
    for (int i = 0; i < const_num; i++) {
        string str = read_string();
        WORD location = GET_WORD();
        if (labels->find(str) != labels->end()) {
            FAIL << "The label " << str << " is defined multiple times" << endl;
        } 
        (*labels)[str] = location;
    }
}

string BaskVM::read_string() {
    BYTE byte;
    vector<BYTE> bytes;
    do {
        byte = read_byte();
        bytes.push_back(byte);
    } while (byte != 0x00);
    bytes.pop_back();
    return bytes_to_string(bytes);
}

void BaskVM::eval()
{
    while (true)
    {
        auto opcode = read_byte();
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
        case OP_GOTO:
            {
                string name = read_string();
                if (labels->find(name) == labels->end()) {
                    FAIL << "Invalid label identifier: " << name;
                }
                ip = code_start + (*labels)[name];
            }
            break;
        case OP_BRANCH:
            {
                string name = read_string();
                if (!(current_stack->pop("Cannot pop branch conditional value")->b())) continue;

                if (labels->find(name) == labels->end()) {
                    FAIL << "Invalid label identifier: " << name;
                }
                ip = code_start + (*labels)[name];
            }
            break;
        case OP_FUNC_CALL:
            switch (read_byte())
            {
            case FUNC_CALL_MODE_INBUILT:
                call_inbuilt(read_string(), current_stack);
                break;
            }
            break;
        case OP_BRING:
            {
                string name = read_string();
                if (symbol_table->find(name) == symbol_table->end()) {
                    FAIL << "Invalid stack identifier: " << name;
                }
                Value* val = (*symbol_table)[name]->pop("Cannot bring from stack '" + name + "' to stack '" + current_stack->name + "' as '" + name + "' is empty");
                current_stack->push(val);
            }
            break;
        case OP_COPY:
            {
                string name = read_string();
                if (symbol_table->find(name) == symbol_table->end()) {
                    FAIL << "Invalid stack identifier: " << name;
                }
                Stack* stack = (*symbol_table)[name];
                Value* val = stack->pop("Cannot copy from stack '" + name + "' to stack '" + current_stack->name + "' as '" + name + "' is empty");
                current_stack->push(val);
                stack->push(val);
            }
            break;
        case OP_POP:
            current_stack->pop("Cannot pop from stack '" + current_stack->name + "'");
            break;
        default:
            FAIL << "Unknown Opcode: " << std::hex << opcode;
        }
    }
}