#ifndef OPCODE_H
#define OPCODE_H

#define OP_HALT 0x00

// push const onto stack
#define OP_CONST 0x01

// call func
#define OP_FUNC_CALL 0x02

#define OP_CREATE 0x03

#define OP_SWITCH 0x04

#define SWITCH_MODE_DEFAULT 0x00
#define SWITCH_MODE_IDENTIFIER 0x01

#define FUNC_CALL_MODE_INBUILT 0x00

#endif