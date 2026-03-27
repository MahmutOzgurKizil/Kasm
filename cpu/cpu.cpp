#include "cpu.h"
#include <string.h>

void cpu_init(CPU *cpu) {
    memset(cpu->registers, 0, sizeof(cpu->registers));
    cpu->SR = 0;
    cpu->OV = 0;
    cpu->PC = 0;
    cpu->LR = 0;
    cpu->SP = STACK_INIT;
    cpu->IR = 0;

    memset(cpu->memory, 0, sizeof(cpu->memory));
    cpu->halted = 0;
}

void cpu_load(CPU *cpu, uint8_t *program, uint8_t size) {
    memcpy(cpu->memory, program, size);
}

void cpu_step(CPU *cpu) {
    if (cpu->halted) return;

    uint8_t high = cpu->memory[cpu->PC];
    uint8_t low = cpu->memory[cpu->PC + 1];
    uint16_t shifted = high << 8;
    cpu->IR = shifted | low;
    cpu->PC += 2;


    uint8_t opcode = (cpu->IR >> 11) & 0b00011111;
    uint8_t rd = (cpu->IR >> 8) & 0b00000111;
    uint8_t rs1 = (cpu->IR >> 5) & 0b00000111;
    uint8_t rs2 = (cpu->IR >> 2) & 0b00000111;

    uint8_t imm = cpu->IR & 0xFF;

    switch (opcode) {
        case OP_HALT:
            cpu->halted = 1;
            break;
        case OP_SET:
            cpu->registers[rd] = imm;
            break;
        case OP_ADD:
            uint16_t result = (uint16_t) cpu->registers[rs1] + (uint16_t) cpu->registers[rs2];
            cpu->registers[rd] = result & 0xFF;
            cpu->OV = (result >> 8) & 0xFF;

            cpu->SR = 0; 
            if (result == 0) cpu->SR |= SR_Z;
            if (result & 0x80) cpu->SR |= SR_N;
            if (result > 0xFF) cpu->SR |= SR_O;

            break;
    }

}
