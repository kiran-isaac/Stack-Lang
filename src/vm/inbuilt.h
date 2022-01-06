#ifndef INBUILT_H
#define INBUILT_H

#include <iostream>
#include <map>
#include "values.h"
#include "stack.h"
#include "byte_ops.h"
#include "logger.h"
#include "../datatypes.h"

void init_inbuilts();

void call_inbuilt(std::vector<uint8_t> name, Stack* stack);

#endif