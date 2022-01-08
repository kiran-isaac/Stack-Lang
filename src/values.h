#ifndef VALUE_H
#define VALUE_H

#include "byte_ops.h"
#include "datatypes.h"

struct Value {
    int type;
    uint32_t data;

    char c() {
        char chr;
        memcpy(&chr, &data, 1);
        return chr;
    };

    float f() {
        float f;
        memcpy(&f, &data, 4);
        return f;
    }
};

#define NUMBER(value) Value{DT_NUM, float_to_word(value)}
#define CHAR(value) Value{DT_CHAR, char_to_word(value)}

#endif