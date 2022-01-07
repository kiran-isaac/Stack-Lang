#ifndef VALUE_H
#define VALUE_H

#include "../datatypes.h"

struct Value {
    int type;
    float f;
    char c;
};

#define NUMBER(value) Value{DT_NUM, value}
#define CHAR(value) Value{DT_CHAR, 0, value}

#endif