#ifndef STACK_H
#define STACK_H

#include <vector>
#include <algorithm>
#include <string>
#include "values.h"
#include "logger.h"

class Stack {
public:
    std::vector<Value*> stack;

    std::string name;

    Stack(std::string name);
    Stack(std::string name, Stack clone);

    int size();

    void reverse();
    void clear();
    void push(Value* value);

    Value* pop(std::string error = "Nothing to pop from stack");
};

#endif
