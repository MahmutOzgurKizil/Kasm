#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>

typedef enum {
    // Arithmetic and Logic
    OP_ADD  = 0b00000,  // 0x00
    OP_SUB  = 0b00001,  // 0x01
    OP_MUL  = 0b00010,  // 0x02
    OP_DIV  = 0b00011,  // 0x03
    OP_NEG  = 0b00100,  // 0x04
    OP_NOT  = 0b00101,  // 0x05
    OP_AND  = 0b00110,  // 0x06
    OP_OR   = 0b00111,  // 0x07
    OP_XOR  = 0b01000,  // 0x08
    OP_SHL  = 0b01001,  // 0x09
    OP_SHR  = 0b01010,  // 0x0A
    OP_ADDI = 0b01011,  // 0x0B
    OP_SUBI = 0b01100,  // 0x0C
    OP_CMP  = 0b01101,  // 0x0D

    // Memory and Data Transfer
    OP_LOAD  = 0b01110, // 0x0E
    OP_STORE = 0b01111, // 0x0F
    OP_MOVE  = 0b10000, // 0x10
    OP_SET   = 0b10001, // 0x11
    OP_PUSH  = 0b10010, // 0x12
    OP_POP   = 0b10011, // 0x13
    OP_MVO   = 0b10100, // 0x14

    // Control Flow
    OP_JUMP  = 0b10101, // 0x15
    OP_JUMPR = 0b10110, // 0x16
    OP_CALL  = 0b10111, // 0x17
    OP_RET   = 0b11000, // 0x18
    OP_JEQ   = 0b11001, // 0x19
    OP_JLT   = 0b11010, // 0x1A
    OP_JOV   = 0b11011, // 0x1B

    // I/O and System
    OP_OUT  = 0b11100,  // 0x1C
    OP_IN   = 0b11101,  // 0x1D
    OP_HALT = 0b11110,  // 0x1E
} OpCode;


#define SR_Z_BIT  0
#define SR_N_BIT  1
#define SR_O_BIT  2

#define SR_Z  (1 << SR_Z_BIT)
#define SR_N  (1 << SR_N_BIT)
#define SR_O  (1 << SR_O_BIT)

#define NUM_REGISTERS  8
#define MEMORY_SIZE    256
#define STACK_INIT     0xFF

#endif
