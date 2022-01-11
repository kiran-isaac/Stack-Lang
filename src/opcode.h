#ifndef OPCODE_H
#define OPCODE_H

#define OP_HALT 0x00

// push const onto stack
#define OP_CONST 0x01

// call func
#define OP_FUNC_CALL 0x02

// create new stack
#define OP_CREATE 0x03

// switch stacks
#define OP_SWITCH 0x04

// move between stacks
#define OP_BRING 0x05

// copy from stack
#define OP_COPY 0x06

#define OP_GOTO 0x07

#define OP_BRANCH 0x08

#define OP_POP 0x09

#define OP_DUP 0x0A

#define OP_NOOP 0x0B

#define OP_CLONE 0x0C

#define SWITCH_MODE_DEFAULT 0x00
#define SWITCH_MODE_IDENTIFIER 0x01

#define FUNC_CALL_MODE_LOCAL 0x00
#define FUNC_CALL_MODE_INBUILT 0x01
#define FUNC_CALL_MODE_MODULE 0x02

#endif