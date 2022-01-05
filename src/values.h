#ifndef VALUE_H
#define VALUE_H

enum class ValueType {
    NUMBER
};

struct Value {
    ValueType type;
    union {
        double number;
    };
};

#define NUMBER(value) Value{ValueType::NUMBER, value}

#endif