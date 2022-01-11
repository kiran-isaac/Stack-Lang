#include "stack.h"

using namespace std;

Stack::Stack(string name) {
    this->name = name;
}

Stack::Stack(string name, Stack clone) {
    this->name = name;
}

int Stack::size() {
    return stack.size();
}

void Stack::reverse() {
    std::reverse(stack.begin(), stack.end());
}

void Stack::clear() {
    for (Value* val : stack) {
        delete val;
    }
    stack.clear();
}

void Stack::push(Value* value) {
    stack.push_back(value);
}

Value* Stack::pop(string error)  {
    if (stack.size() < 1) {
        FAIL << error;
    }
    Value* val = stack.back();;
    
    stack.pop_back();
    return val;
}