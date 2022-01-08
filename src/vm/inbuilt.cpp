#include "inbuilt.h"

using namespace std;

typedef void (*func_ptr)(Stack* stack);
map<string, func_ptr> funcMap;

string pop_string(Stack* stack, string error) {
    string str;
    char val = stack->pop(error)->c();
    while (val != 0x00) {
        str += val;
        val = stack->pop(error)->c();
    }
    return str;
}

void push_string(Stack* stack, string str) {
    reverse(str.begin(), str.end());
    stack->push(new CHAR(0x00));
    for (char c : str) {
        stack->push(new CHAR(c));
    }
}

void call_inbuilt(std::string name, Stack* stack) {
    func_ptr ptr = funcMap[name];
    if (ptr == nullptr) {
        FAIL << "Cannot locate inbuilt: " << name;
    }
    funcMap[name](stack);
}

void StackID(Stack* stack) {
    push_string(stack, stack->name);
}

void print(Stack* stack) {
    uint32_t mode = stack->pop("[Inbuilt Function : print] Cannot pop parameter 0: print datatype")->d();
    switch (mode) {
        case DT_NUM:
            cout << stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->d();
            break;
        case DT_CHAR:
            cout << stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->c();
            break;
        case DT_STRING:
            {
                cout << pop_string(stack, "[Inbuilt Function : print] Cannot pop string");
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

void str_concat(Stack* stack) {
    string str1 = pop_string(stack, "[Inbuilt Function : str_concat] Cannot get first string");
    string str2 = pop_string(stack, "[Inbuilt Function : str_concat] Cannot get second string");
    push_string(stack, str1 + str2);
    
}

void add(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : +] Cannot pop parameter 0: operand")->d();
    float op2 = stack->pop("[Inbuilt Function : +] Cannot pop parameter 1: operand")->d();
    stack->push(new NUMBER(op1 + op2));
}

void sub(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 0: operand")->d();
    float op2 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 1: operand")->d();
    stack->push(new NUMBER(op2 - op1));
}

void mul(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : *] Cannot pop parameter 0: operand")->d();
    float op2 = stack->pop("[Inbuilt Function : *] Cannot pop parameter 1: operand")->d();
    stack->push(new NUMBER(op2 * op1));
}

void div(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 0: operand")->d();
    float op2 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 1: operand")->d();
    stack->push(new NUMBER(op2 / op1));
}

void pow(Stack* stack) {
    float op1 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 0: operand")->d();
    float op2 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 1: operand")->d();
    stack->push(new NUMBER((float)pow(op2, op1)));
}

void exit(Stack* stack) {
    uint32_t code = stack->pop("[Inbuilt Function : exit] Cannot pop parameter 0: exit code")->d();
    cout << "Exited with code: " << code << endl;
    if (stack->size() != 0) {
        cout << "Exiting with non empty stack " << stack->name << endl;
    }
    exit(code);
}

void init_inbuilts() {
    funcMap["print"]      = &print;
    funcMap["exit"]       = &exit;
    funcMap["add"]        = &add;
    funcMap["sub"]        = &sub;
    funcMap["mul"]        = &mul;
    funcMap["div"]        = &div;
    funcMap["pow"]        = &pow;
    funcMap["stack_len"]  = &stack_len;
    funcMap["reverse"]    = &reverse;
    funcMap["stackID"]    = &StackID;
    funcMap["str_concat"] = &str_concat;
}