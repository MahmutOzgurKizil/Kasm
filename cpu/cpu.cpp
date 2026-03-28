#include "cpu.h"
#include "instructions.h"
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
        case OP_ADD: {
            uint8_t a = cpu->registers[rs1];
            uint8_t b = cpu->registers[rs2];
            uint16_t result = (uint16_t)a + (uint16_t)b;
            cpu->registers[rd] = result & 0xFF;
            cpu->OV = (result >> 8) & 0xFF;

            cpu->SR = 0; 
            if (result == 0) cpu->SR |= SR_Z;
            if (result & 0x80) cpu->SR |= SR_N;
            if (result > 0xFF) cpu->SR |= SR_O;
            break;
        }
        case OP_SUB: {
            uint8_t a = cpu->registers[rs1];
            uint8_t b = cpu->registers[rs2];
            uint16_t result = (uint16_t)a - (uint16_t)b;
            cpu->registers[rd] = result & 0xFF;

            int8_t signed_result = (int8_t)(result & 0xFF);
            cpu->SR = 0;
            if (signed_result == 0) cpu->SR |= SR_Z;
            if (signed_result < 0) cpu->SR |= SR_N;
            if ((a ^ b) & 0x80 && (a ^ (uint8_t)result) & 0x80) cpu->SR |= SR_O;
            break;
        }
        case OP_MUL: {
            uint8_t a = cpu->registers[rs1];
            uint8_t b = cpu->registers[rs2];
            uint16_t result = (uint16_t)a * (uint16_t)b;
            cpu->registers[rd] = result & 0xFF;
            cpu->OV = (result >> 8) & 0xFF;

            cpu->SR = 0; 
            if (result == 0) cpu->SR |= SR_Z;
            if (result & 0x80) cpu->SR |= SR_N;
            if (result > 0xFF) cpu->SR |= SR_O;
            break;
        }
        case OP_DIV: {
            uint8_t a = cpu->registers[rs1];
            uint8_t b = cpu->registers[rs2];
            if (b == 0) {
                cpu->halted = 1;
                break;
            }
            uint8_t result = a / b;
            cpu->registers[rd] = result & 0xFF;

            cpu->SR = 0; 
            if (result == 0) cpu->SR |= SR_Z;
            if (result & 0x80) cpu->SR |= SR_N;
            break;
        }
        case OP_NEG: {
            cpu->registers[rd] = (uint8_t)(-(int8_t)cpu->registers[rs1]);
            break;
        }
        case OP_NOT: {
            cpu->registers[rd] = ~cpu->registers[rs1];
            break;
        }
        case OP_AND: {
            cpu->registers[rd] = cpu->registers[rs1] & cpu->registers[rs2];
            break;
        }
        case OP_OR: {
            cpu->registers[rd] = cpu->registers[rs1] | cpu->registers[rs2];
            break;
        }
        case OP_XOR: {
            cpu->registers[rd] = cpu->registers[rs1] ^ cpu->registers[rs2];
            break;
        }
        case OP_SHL: {
            cpu->registers[rd] = cpu->registers[rs1] << imm; 
            break;
        }
        case OP_SHR: {
            cpu->registers[rd] = cpu->registers[rs1] >> imm; 
            break;
        }
        case OP_ADDI: {
            uint8_t a = cpu->registers[rs1];
            uint16_t result = (uint16_t)a + imm;
            cpu->registers[rd] = result & 0xFF;
            cpu->OV = (result >> 8) & 0xFF;

            cpu->SR = 0; 
            if (result == 0) cpu->SR |= SR_Z;
            if (result & 0x80) cpu->SR |= SR_N;
            if (result > 0xFF) cpu->SR |= SR_O;
            break;
        }
        case OP_SUBI: {
            uint8_t a = cpu->registers[rs1];
            uint16_t result = (uint16_t)a - imm;
            cpu->registers[rd] = result & 0xFF;

            int8_t signed_result = (int8_t)(result & 0xFF);
            cpu->SR = 0;
            if (signed_result == 0) cpu->SR |= SR_Z;
            if (signed_result < 0) cpu->SR |= SR_N;
            if ((a ^ imm) & 0x80 && (a ^ (uint8_t)result) & 0x80) cpu->SR |= SR_O;
            break;
        }
        case OP_CMP: {
            uint8_t a = cpu->registers[rs1];
            uint8_t b = cpu->registers[rs2];
            uint16_t result = (uint16_t)a - (uint16_t)b;

            int8_t signed_result = (int8_t)(result & 0xFF);
            cpu->SR = 0;
            if (signed_result == 0) cpu->SR |= SR_Z;
            if (signed_result < 0) cpu->SR |= SR_N;
            if ((a ^ b) & 0x80 && (a ^ (uint8_t)result) & 0x80) cpu->SR |= SR_O;
            break;
        }
        case OP_LOAD: {
            cpu->registers[rd] = cpu->memory[cpu->registers[rs1]];
            break;
        }
        case OP_STORE: {
            cpu->memory[cpu->registers[rd]] = cpu->registers[rs1];
            break;
        }
        case OP_MOVE: {
            cpu->registers[rd] = cpu->registers[rs1];
            break;
        }
        case OP_PUSH: {
            cpu->memory[cpu->SP] = cpu->registers[rs1];
            if (cpu->SP == 0) {
                cpu->halted = 1;
                break;
            }
            cpu->SP -= 1;
            break;
        }
        case OP_POP: {
            if (cpu->SP == 0xFF) {
                cpu->halted = 1;
                break;
            }
            cpu->SP += 1;
            cpu->registers[rd] = cpu->memory[cpu->SP];
            break;
        }
        case OP_MVO: {
            cpu->registers[rd] = cpu->OV;
            break;
        }
        case OP_JUMP: {
            if (imm % 2 != 0) {
                cpu->halted = 1;
                break;
            }
            cpu->PC = imm;
            break;
        }
        case OP_JUMPR: {
            if (cpu->registers[rd] % 2 != 0) {
                cpu->halted = 1;
                break;
            }
            cpu->PC = cpu->registers[rd];
            break;
        }
        case OP_CALL: {
            if (imm % 2 != 0) {
                cpu->halted = 1;
                break;
            }
            cpu->LR = cpu->PC;
            cpu->PC = imm;
            break;
        }
        case OP_RET: {
            cpu->PC = cpu->LR;
            break;
        }
        case OP_JEQ: {
            if (imm % 2 != 0) {
                cpu->halted = 1;
                break;
            }
            if (cpu->SR & SR_Z) cpu->PC = imm;
            
            break;
        }
        case OP_JLT: {
            if (imm % 2 != 0) {
                cpu->halted = 1;
                break;
            }
            if (cpu->SR & SR_N) cpu->PC = imm;
            
            break;
        }
        case OP_JOV: {
            if (imm % 2 != 0) {
                cpu->halted = 1;
                break;
            }
            if (cpu->SR & SR_O) cpu->PC = imm;
            
            break;
        }
        case OP_OUT: {
            // TODO
            break;
        }
        case OP_IN: {
            // TODO
            break;
        }
        
        


    }

}
