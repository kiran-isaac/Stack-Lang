#ifndef VALUE_H
#define VALUE_H

enum class ValueType {
    NUMBER,
    POINTER
};

struct Value {
    ValueType type;
    float number;
};

#define NUMBER(value) Value{ValueType::NUMBER, value}

#endif