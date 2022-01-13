#ifndef INBUILT_H
#define INBUILT_H

#include <iostream>
#include <map>
#include <cmath>
#include <limits>
#include <ctime>
#include <time.h>
#include "values.h"
#include "stack.h"
#include "../byte_ops.h"
#include "logger.h"
#include "../datatypes.h"

void init_inbuilts();

void call_inbuilt(std::string name, Stack* stack);

#endif