#ifndef STACK_H
#define STACK_H

#include <vector>
#include "values.h"

class Stack {
private:
    std::vector<Value*> stack;

public:
    void push(Value* value) {
        stack.push_back(value);
    }

    Value* pop() { 
        Value* val = stack.back();
        stack.pop_back();
        return val;
    }
};

#endif
