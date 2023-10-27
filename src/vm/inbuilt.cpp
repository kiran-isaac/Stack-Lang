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
            typedef std::numeric_limits< double > dbl;
            cout.precision(dbl::max_digits10);
            cout << stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->d();
            break;
        case DT_CHAR:
            cout << stack->pop("[Inbuilt Function : print] Cannot pop parameter 1: print value")->c();
            break;
        case DT_STRING:
            cout << pop_string(stack, "[Inbuilt Function : print] Cannot pop string");
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

void start_timer(Stack* stack) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    double start = ts.tv_sec;
    stack->push(new NUMBER(start));
}

void end_timer(Stack* stack) {
    double start = stack->pop("[Inbuilt Function : end_timer] Cannot pop the start time")->d();
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    double end = ts.tv_sec;
    double delta = double(end - start);
    stack->push(new NUMBER(delta));
}

void add(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : +] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : +] Cannot pop parameter 1: operand")->d();
    stack->push(new NUMBER(op1 + op2));
}

void sub(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : -] Cannot pop parameter 1: operand")->d();
    stack->push(new NUMBER(op1 - op2));
}

void mul(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : *] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : *] Cannot pop parameter 1: operand")->d();
    stack->push(new NUMBER(op1 * op2));
}

void div(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : /] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : /] Cannot pop parameter 1: operand")->d();
    stack->push(new NUMBER(op1 / op2));
}

void pow(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : ^] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : ^] Cannot pop parameter 1: operand")->d();
    stack->push(new NUMBER((double)pow(op1, op2)));
}

void equal(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : ==] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : ==] Cannot pop parameter 1: operand")->d();
    stack->push(new BOOL(op1 == op2));
}

void not_equal(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : !=] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : !=] Cannot pop parameter 1: operand")->d();
    stack->push(new BOOL(op1 != op2));
}

void less_than(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : <] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : <] Cannot pop parameter 1: operand")->d();
    stack->push(new BOOL(op1 < op2));
}

void greater_than(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : >] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : >] Cannot pop parameter 1: operand")->d();
    stack->push(new BOOL(op1 > op2));
}

void greater_than_or_equal(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : >=] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : >=] Cannot pop parameter 1: operand")->d();
    stack->push(new BOOL(op1 >= op2));
}

void less_than_or_equal(Stack* stack) {
    double op1 = stack->pop("[Inbuilt Function : <=] Cannot pop parameter 0: operand")->d();
    double op2 = stack->pop("[Inbuilt Function : <=] Cannot pop parameter 1: operand")->d();
    stack->push(new BOOL(op1 <= op2));
}

void bool_and(Stack* stack) {
    bool op1 = stack->pop("[Inbuilt Function : &&] Cannot pop parameter 0: operand")->b();
    bool op2 = stack->pop("[Inbuilt Function : &&] Cannot pop parameter 1: operand")->b();
    stack->push(new BOOL(op1 && op2));
}

void bool_or(Stack* stack) {
    bool op1 = stack->pop("[Inbuilt Function : ||] Cannot pop parameter 0: operand")->b();
    bool op2 = stack->pop("[Inbuilt Function : ||] Cannot pop parameter 1: operand")->b();
    stack->push(new BOOL(op1 || op2));
}

void bool_xor(Stack* stack) {
    bool op1 = stack->pop("[Inbuilt Function : ==] Cannot pop parameter 0: operand")->b();
    bool op2 = stack->pop("[Inbuilt Function : ==] Cannot pop parameter 1: operand")->b();
    stack->push(new BOOL((op1 && !op2) || (op2 && !op1)));
}

void bool_not(Stack* stack) {
    bool op1 = stack->pop("[Inbuilt Function : !] Cannot pop parameter 0: operand")->b();
    stack->push(new BOOL(!op1));
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
    // Mathmatical
    funcMap["add"]        = &add;
    funcMap["sub"]        = &sub;
    funcMap["mul"]        = &mul;
    funcMap["div"]        = &div;
    funcMap["pow"]        = &pow;

    // Comparisons
    funcMap["equal"]      = &equal;
    funcMap["not_equal"]  = &not_equal;
    funcMap["lt"]         = &less_than;
    funcMap["gt"]         = &greater_than;
    funcMap["lte"]        = &less_than_or_equal;
    funcMap["gte"]        = &greater_than_or_equal;

    // Boolean
    funcMap["not"]        = &bool_not;
    funcMap["and"]        = &bool_and;
    funcMap["xor"]        = &bool_xor;
    funcMap["or"]         = &bool_or;

    // Other
    funcMap["print"]      = &print;
    funcMap["exit"]       = &exit;
    funcMap["stack_len"]  = &stack_len;
    funcMap["reverse"]    = &reverse;
    funcMap["stack_id"]    = &StackID;
    funcMap["str_concat"] = &str_concat;
    funcMap["start_timer"] = &start_timer;
    funcMap["end_timer"]   = &end_timer;
}