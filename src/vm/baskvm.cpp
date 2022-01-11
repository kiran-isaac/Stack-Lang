#include "baskvm.h"

using namespace std;

BYTE BaskVM::read_byte() {
    ip++;
    return code[ip - 1];
}

BaskVM::BaskVM() {
    ip = 0;
    default_stack = new Stack("default");
    current_stack = default_stack;
    symbol_table["default"] = default_stack;
    code = std::vector<BYTE>();
    init_inbuilts();
}

void BaskVM::load(char* fname) {
    char buffer[BUFFER_SIZE];
    ifstream file("/home/kiran/baskvm/test/a.out", ios::in | ios::binary);
    file.read(buffer, BUFFER_SIZE);
    for (char chr : buffer) {
        this->code.push_back((uint8_t)chr);
    }
}

void BaskVM::exec() {
    read_consts();
    read_funcs();

    BaskVM* vm = new BaskVM();
    vm->code = functions["main"];
    vm->name = "main";
    vm->constants = constants;
    vm->functions = functions;
    vm->eval();
    delete vm;
    return;
}

void BaskVM::read_consts() {
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

void BaskVM::read_labels() {
    WORD const_num = GET_WORD();
    for (int i = 0; i < const_num; i++) {
        string str = read_string();
        WORD location = GET_WORD();
        if (labels.find(str) != labels.end()) {
            FAIL << "The label " << str << " is defined multiple times" << endl;
        } 
        labels[str] = location;
    }
}

void BaskVM::read_funcs() {
    int ip_reset = ip;
    for (; ip < code.size();) {
        string str = read_string();
        WORD length = GET_WORD();
        vector<BYTE> test = vector<BYTE> (code.begin() + ip, code.begin() + ip + length);
        functions[str] = test;
        ip += length;
    }
    ip = ip_reset;
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
    read_labels();
    code_start = ip;
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
                if (symbol_table.find(name) != symbol_table.end()) {
                    FAIL << "Stack '" << name << "' already exists";
                }
                symbol_table[name] = new Stack(this->name + ":" + name);
            }
            break;
        case OP_SWITCH:
            {
                string name = read_string();
                if (symbol_table.find(name) == symbol_table.end()) {
                    FAIL << "Invalid stack identifier: " << name;
                }
                current_stack = (symbol_table[name]);
            }
            break;
        case OP_GOTO:
            {
                string name = read_string();
                if (labels.find(name) == labels.end()) {
                    FAIL << "Invalid label identifier: " << name;
                }
                ip = code_start + labels[name];
            }
            break;
        case OP_BRANCH:
            {
                string name = read_string();
                if (!(current_stack->pop("Cannot pop branch conditional value")->b())) continue;

                if (labels.find(name) == labels.end()) {
                    FAIL << "Invalid label identifier: " << name;
                }
                ip = code_start + labels[name];
            }
            break;
        case OP_FUNC_CALL:
            switch (read_byte())
            {
            case FUNC_CALL_MODE_INBUILT:
                call_inbuilt(read_string(), current_stack);
                break;
            case FUNC_CALL_MODE_LOCAL:
                string name = read_string();
                BaskVM* vm = new BaskVM();
                vm->code = functions[name];
                vm->name = name;
                vm->default_stack->name = name + ":default";
                int stack_size = current_stack->size();
                current_stack->reverse();
                for (int i = 0; i <= stack_size - 1; i++) {
                    Value* val = current_stack->pop();
                    vm->default_stack->push(new Value{val->type, val->data});
                }

                current_stack->clear();

                vm->constants = constants;
                vm->current_stack = vm->default_stack;
                vm->eval();
                vm->default_stack->reverse();
                for (int i = 0; i < vm->default_stack->size(); i++) {
                    Value* val = vm->default_stack->pop();
                    current_stack->push(new Value{val->type, val->data});
                }
                for (std::map<std::string, Stack*>::iterator i = vm->symbol_table.begin(); i != vm->symbol_table.end(); i++) {
                    if (i->second == vm->default_stack) continue;
                    i->second->clear();
                    delete i->second;
                }
                delete vm;
            }
            break;
        case OP_BRING:
            {
                string name = read_string();
                if (symbol_table.find(name) == symbol_table.end()) {
                    FAIL << "Invalid stack identifier: " << name;
                }
                Value* val = symbol_table[name]->pop("Cannot bring from stack '" + name + "' to stack '" + current_stack->name + "' as '" + name + "' is empty");
                current_stack->push(val);
            }
            break;
        case OP_COPY:
            {
                string name = read_string();
                if (symbol_table.find(name) == symbol_table.end()) {
                    FAIL << "Invalid stack identifier: " << name;
                }
                Stack* stack = symbol_table[name];
                Value* val = stack->pop("Cannot copy from stack '" + name + "' to stack '" + current_stack->name + "' as '" + name + "' is empty");
                current_stack->push(val);
                stack->push(val);
            }
            break;
        case OP_POP:
            current_stack->pop("Cannot pop from stack '" + current_stack->name + "'");
            break;
        case OP_DUP:
            {
                Value* val = current_stack->pop("Cannot pop to duplicate from stack '" + current_stack->name + "'");
                current_stack->push(val);
                current_stack->push(val);
            }
            break;
        default:
            FAIL << "Unknown Opcode: " << std::hex << opcode;
        }
    }
}