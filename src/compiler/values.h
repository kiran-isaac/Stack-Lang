#ifndef VALUE_H
#define VALUE_H

#include "../datatypes.h"

struct Value {
    uint8_t type;
    float number;
};

#define NUMBER(value) Value{DT_NUM, value}

#endif