#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "instructions.h"

typedef struct {
    uint8_t registers[NUM_REGISTERS]; 
    uint8_t SR;
    uint8_t OV;
    uint8_t PC;
    uint8_t LR;
    uint8_t SP;
    uint16_t IR;

    uint8_t memory[MEMORY_SIZE];
    uint8_t halted;
} CPU;


void cpu_init(CPU *cpu);
void cpu_step(CPU *cpu);
void cpu_load(CPU *cpu, uint8_t *program, uint8_t size);
#endif
