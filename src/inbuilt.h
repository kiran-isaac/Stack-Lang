#ifndef INBUILT_H
#define INBUILT_H

#include <iostream>
#include <map>
#include "values.h"
#include "stack.h"
#include "byte_ops.h"

void print(Stack*);

void call_inbuilt(uint32_t id);

#endif