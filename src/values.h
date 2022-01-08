#ifndef VALUE_H
#define VALUE_H

#include "byte_ops.h"
#include "datatypes.h"

struct Value {
    int type;
    WORD data;

    char c() {
        char chr;
        if (type == DT_NUM) {
            int i = (int)d();
            chr = (char)i;
        } else if (type == DT_CHAR) {
            chr;
            memcpy(&chr, &data, 1);
        }
        return chr;
    };

    double d() {
        double f;
        if (type == DT_CHAR) {
            f = (double)(int)c();
        } else {
            memcpy(&f, &data, 8);
        }
        return f;
    }
};

#define NUMBER(value) Value{DT_NUM, double_to_word(value)}
#define CHAR(value) Value{DT_CHAR, char_to_word(value)}

#endif