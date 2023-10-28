#include "../include/vm.h"

using namespace std;

#include <cassert>

BYTE VM::read_byte() {
  ip++;
  return code[ip - 1];
}

VM::VM() {
  ip = 0;
  default_stack = new Stack("default");
  current_stack = default_stack;
  symbol_table["default"] = default_stack;
  code = std::vector<BYTE>();
  init_inbuilts();
}

vector<BYTE> VM::load(const char* fname) {
  // load file into vector of bytes
  ifstream file(fname, ios::binary | ios::ate);
  if (!file.is_open()) {
    VM_FAIL << "Unable to open file '" << fname << "'";
  }
  streampos size = file.tellg();
  vector<BYTE> buffer(size);
  file.seekg(0, ios::beg);
  file.read((char*)&buffer[0], size);
  file.close();
  return buffer;
}

void VM::exec(BSKConfig* config) {
  assert(config->mode == Mode::RUN);

  WORD size = 0;

  vector<vector<BYTE>> files = vector<vector<BYTE>>();

  if (!filesystem::exists(filesystem::path(config->lib))) {
    VM_FAIL << "Unable to find stdlib at '" << config->lib
            << "'. Please build the standard library and move it to this "
               "location or locate it using the -std flag";
  }

  // add stdlib to inputs so it can be loaded
  config->inputs.push_back(new File(config->lib.c_str(), true));

  for (File* input : config->inputs) {
    vector<BYTE> code = load(input->name);

    size += COMBINE_8_BYTES(code[0], code[1], code[2], code[3], code[4],
                            code[5], code[6], code[7]);

    code = vector<BYTE>(code.begin() + 8, code.end());

    files.push_back(code);
  }

  for (vector<BYTE> file : files) {
    for (BYTE byte : file) {
      this->code.push_back(byte);
    }
  }

  read_funcs(size);
  if (functions.find("main") == functions.end()) {
    VM_FAIL << "Cannot locate main function";
  }
  VM* vm = new VM();
  vm->code = functions["main"];
  vm->name = "main";
  vm->constants = constants;
  vm->functions = functions;
  vm->run();
  delete vm;
  return;
}

void VM::read_consts() {
  WORD const_num = GET_WORD();
  for (size_t i = 0; i < const_num; i++) {
    switch (read_byte()) {
      case DT_NUM: {
        WORD value = GET_WORD();
        constants.push_back(new Value({DT_NUM, value}));
      } break;
      case DT_CHAR: {
        char value = (char)read_byte();
        constants.push_back(new CHAR(value));
      } break;
    }
  }
}

void VM::read_labels() {
  WORD const_num = GET_WORD();
  for (size_t i = 0; i < const_num; i++) {
    string str = read_string();
    WORD location = GET_WORD();
    if (labels.find(str) != labels.end()) {
      VM_FAIL << "The label " << str << " is defined multiple times" << endl;
    }
    labels[str] = location;
  }
}

void VM::read_funcs(int num) {
  for (int i = 0; i < num; i++) {
    string str = read_string();
    WORD length = GET_WORD();
    vector<BYTE> func_contents =
        vector<BYTE>(code.begin() + ip, code.begin() + ip + length);
    functions[str] = func_contents;
    ip += length;
  }
  ip = 0;
}

string VM::read_string() {
  BYTE byte;
  vector<BYTE> bytes;
  do {
    byte = read_byte();
    bytes.push_back(byte);
  } while (byte != 0x00);
  bytes.pop_back();
  return bytes_to_string(bytes);
}

void VM::run() {
  read_consts();
  read_labels();
  code_start = ip;
  while (true) {
    auto opcode = read_byte();
    switch (opcode) {
      case OP_HALT:
        return;
      case OP_CONST:
        current_stack->push(GET_CONSTANT());
        break;
      case OP_CREATE: {
        string name = read_string();
        if (symbol_table.find(name) != symbol_table.end()) {
          VM_FAIL << "Stack '" << name << "' already exists";
        }
        symbol_table[name] = new Stack(name);
      } break;
      case OP_SWITCH: {
        string name = read_string();
        if (symbol_table.find(name) == symbol_table.end()) {
          VM_FAIL << "Invalid stack identifier: " << name;
        }
        current_stack = (symbol_table[name]);
      } break;
      case OP_GOTO: {
        string name = read_string();
        if (labels.find(name) == labels.end()) {
          VM_FAIL << "Invalid label identifier: " << name;
        }
        ip = code_start + labels[name];
      } break;
      case OP_BRANCH: {
        string name = read_string();

        // If the top value on the stack is 0, do nothing, otherwise branch
        if (!(current_stack->pop("Cannot pop branch conditional value")->b()))
          continue;

        if (labels.find(name) == labels.end()) {
          VM_FAIL << "Invalid label identifier: " << name;
        }
        ip = code_start + labels[name];
      } break;
      case OP_FUNC_CALL:
        switch (read_byte()) {
          case FUNC_CALL_MODE_INBUILT:
            call_inbuilt(read_string(), current_stack);
            break;
          case FUNC_CALL_MODE_LOCAL:
            string name = read_string();
            if (functions.find(name) == functions.end()) {
              VM_FAIL << "Could not find function with identifier: " << name;
            }
            VM* vm = new VM();
            vm->code = functions[name];
            vm->functions = functions;
            vm->name = name;
            vm->default_stack = current_stack;
            vm->symbol_table["default"] = current_stack;

            vm->current_stack = vm->default_stack;
            vm->run();
            for (std::map<std::string, Stack*>::iterator i =
                     vm->symbol_table.begin();
                 i != vm->symbol_table.end(); i++) {
              if (i->second == vm->default_stack) continue;
              i->second->clear();
              delete i->second;
            }
            delete vm;
            break;
        }
        break;
      case OP_BRING: {
        string name = read_string();
        if (symbol_table.find(name) == symbol_table.end()) {
          VM_FAIL << "Invalid stack identifier: " << name;
        }
        Value* val = symbol_table[name]->pop(
            "Cannot bring from stack '" + name + "' to stack '" +
            current_stack->name + "' as '" + name + "' is empty");
        current_stack->push(val);
      } break;
      case OP_COPY: {
        string name = read_string();
        if (symbol_table.find(name) == symbol_table.end()) {
          VM_FAIL << "Invalid stack identifier for copy: " << name;
        }
        Stack* stack = symbol_table[name];
        Value* val =
            stack->pop("Cannot copy from stack '" + name + "' to stack '" +
                       current_stack->name + "' as '" + name + "' is empty");
        current_stack->push(val);
        stack->push(val);
      } break;
      case OP_CLONE: {
        string name = read_string();
        if (symbol_table.find(name) == symbol_table.end()) {
          VM_FAIL << "Invalid stack identifier for clone: " << name;
        }
        Stack* original = symbol_table[name];
        for (Value* val : original->stack) {
          current_stack->push(new Value{val->type, val->data});
        }
      } break;
      case OP_POP:
        current_stack->pop("Cannot pop from stack '" + current_stack->name +
                           "'");
        break;
      case OP_DUP: {
        Value* val = current_stack->pop("Cannot pop to duplicate from stack '" +
                                        current_stack->name + "'");
        current_stack->push(val);
        current_stack->push(val);
      } break;
      default:
        VM_FAIL << "Unknown Opcode: " << std::hex << opcode;
    }
  }
}