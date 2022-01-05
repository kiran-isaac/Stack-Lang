#ifndef STACK_H
#define STACK_H

#include <vector>
#include <string>
#include "values.h"
#include "logger.h"

class Stack {
private:
    std::vector<Value*> stack;

public:
    void push(Value* value) {
        stack.push_back(value);
    }

    Value* pop(std::string error = "Nothing to pop from stack") {
        if (stack.size() < 1) {
            FAIL << error;
        }
        Value* val = stack.back();;
        
        stack.pop_back();
        return val;
    }
};

#endif
