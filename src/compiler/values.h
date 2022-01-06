#ifndef VALUE_H
#define VALUE_H

enum class ValueType {
    FLOAT,
    INT,
    POINTER
};

struct Value {
    ValueType type;
    float number;
};

#define NUMBER(value) Value{ValueType::FLOAT, value}

#endif