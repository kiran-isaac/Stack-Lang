#include "inbuilt.h"

using namespace std;

typedef void (*func_ptr)(Stack* stack);
map<string , func_ptr> funcMap;

void call_inbuilt(vector<uint8_t> name, Stack* stack) {
    string str = bytes_to_string(name);
    func_ptr ptr = funcMap[str];
    if (ptr == nullptr) {
        FAIL << "Cannot locate inbuilt: " << str;
    }
    funcMap[str](stack);
}

void print(Stack* stack) {
    uint32_t mode = stack->pop("[Inbuilt Function : print] Cannot pop parameter 0: print datatype")->number;
    switch (mode) {
        case 0:
            cout << stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->number << endl;
            break;
        default:
            FAIL << "[Inbuilt Function : print]: Invalid value for print datatype. Push the print datatype before calling print";
    }
}

void exit(Stack* stack) {
    uint32_t code = stack->pop("[Inbuilt Function : exit] Cannot pop parameter 0: exit code")->number;
    exit(code);
}

void init_inbuilts() {
    funcMap["print"] = &print;
    funcMap["exit"] = &exit;
}