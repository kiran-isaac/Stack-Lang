#include "inbuilt.h"

using namespace std;

typedef void (*func_ptr)(Stack* stack);
map<string, func_ptr> funcMap;

void call_inbuilt(std::string name, Stack* stack) {
    func_ptr ptr = funcMap[name];
    if (ptr == nullptr) {
        FAIL << "Cannot locate inbuilt: " << name;
    }
    funcMap[name](stack);
}

void StackID(Stack* stack) {
    string n = stack->name;
    reverse(n.begin(), n.end());
    stack->push(new CHAR(0x00));
    for (char c : n) {
        stack->push(new CHAR(c));
    }
}

void print(Stack* stack) {
    uint32_t mode = stack->pop("[Inbuilt Function : print] Cannot pop parameter 0: print datatype")->f();
    switch (mode) {
        case DT_NUM:
            cout << stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->f();
            break;
        case DT_CHAR:
            cout << stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->c();
            break;
        case DT_STRING:
            {
                char val = stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->c();
                while (val != 0x00) {
                    cout << val;
                    val = stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->c();
                }
            }
            break;
        default:
            FAIL << "[Inbuilt Function : print]: Invalid value for print datatype. Push the print datatype before calling print";
    }
}

void stack_len(Stack* stack) {
    stack->push(new NUMBER(stack->size() * 1.0f));
}

void reverse(Stack* stack) {
    stack->reverse();
}

void add(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : +] Cannot pop parameter 0: operand")->f();
    float op2 = stack->pop("[Inbuilt Function : +] Cannot pop parameter 1: operand")->f();
    stack->push(new NUMBER(op1 + op2));
}

void sub(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 0: operand")->f();
    float op2 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 1: operand")->f();
    stack->push(new NUMBER(op2 - op1));
}

void mul(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : *] Cannot pop parameter 0: operand")->f();
    float op2 = stack->pop("[Inbuilt Function : *] Cannot pop parameter 1: operand")->f();
    stack->push(new NUMBER(op2 * op1));
}

void div(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 0: operand")->f();
    float op2 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 1: operand")->f();
    stack->push(new NUMBER(op2 / op1));
}

void exit(Stack* stack) {
    uint32_t code = stack->pop("[Inbuilt Function : exit] Cannot pop parameter 0: exit code")->f();
    cout << "Exited with code: " << code << endl;
    if (stack->size() != 0) {
        cout << "Exiting with non empty stack " << stack->name << endl;
    }
    exit(code);
}

void init_inbuilts() {
    funcMap["print"]     = &print;
    funcMap["exit"]      = &exit;
    funcMap["add"]       = &add;
    funcMap["sub"]       = &sub;
    funcMap["mul"]       = &mul;
    funcMap["div"]       = &div;
    funcMap["stack_len"] = &stack_len;
    funcMap["reverse"]   = &reverse;
    funcMap["stackID"]   = &StackID;
}