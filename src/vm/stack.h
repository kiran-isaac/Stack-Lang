#ifndef STACK_H
#define STACK_H

#include <vector>
#include <algorithm>
#include <string>
#include "../values.h"
#include "logger.h"

class Stack {
private:
    std::vector<Value*> stack;

public:
    std::string name;

    Stack(std::string name);

    int size();

    void reverse();

    void push(Value* value);

    Value* pop(std::string error = "Nothing to pop from stack");
};

#endif
