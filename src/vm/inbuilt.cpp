#include "inbuilt.h"

using namespace std;

typedef void (*func_ptr)(Stack* stack);
map<string, func_ptr> funcMap;

void call_inbuilt(vector<uint8_t> name, Stack* stack) {
    string str = bytes_to_string(name);
    func_ptr ptr = funcMap[str];
    if (ptr == nullptr) {
        FAIL << "Cannot locate inbuilt: " << str;
    }
    funcMap[str](stack);
}

void print(Stack* stack) {
    uint32_t mode = stack->pop("[Inbuilt Function : print] Cannot pop parameter 0: print datatype")->f;
    switch (mode) {
        case DT_NUM:
            cout << stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->f << endl;
            break;
        case DT_CHAR:
            cout << stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->c << endl;
            break;
        default:
            FAIL << "[Inbuilt Function : print]: Invalid value for print datatype. Push the print datatype before calling print";
    }
}

void add(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : +] Cannot pop parameter 0: operand")->f;
    float op2 = stack->pop("[Inbuilt Function : +] Cannot pop parameter 1: operand")->f;
    stack->push(new NUMBER(op1 + op2));
}

void sub(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 0: operand")->f;
    float op2 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 1: operand")->f;
    stack->push(new NUMBER(op2 - op1));
}

void mul(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : *] Cannot pop parameter 0: operand")->f;
    float op2 = stack->pop("[Inbuilt Function : *] Cannot pop parameter 1: operand")->f;
    stack->push(new NUMBER(op2 * op1));
}

void div(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 0: operand")->f;
    float op2 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 1: operand")->f;
    stack->push(new NUMBER(op2 / op1));
}

void exit(Stack* stack) {
    uint32_t code = stack->pop("[Inbuilt Function : exit] Cannot pop parameter 0: exit code")->f;
    cout << "Exited with code: " << code << endl;
    exit(code);
}

void init_inbuilts() {
    funcMap["print"]    = &print;
    funcMap["exit"]     = &exit;
    funcMap["add"]      = &add;
    funcMap["sub"]      = &sub;
    funcMap["mul"]      = &mul;
    funcMap["div"]      = &div;
}