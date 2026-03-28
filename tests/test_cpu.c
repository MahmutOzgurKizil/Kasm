#include "../cpu/cpu.h"

#include "test_helpers.h"

void test_cpu_init() {
    CPU cpu;
    cpu_init(&cpu);
    ASSERT(cpu.PC == 0, "PC starts at 0");
    ASSERT(cpu.SP == 0xFF, "SP starts at 0xFF");
    ASSERT(cpu.halted == 0, "CPU not halted");
    ASSERT(cpu.registers[0] == 0, "R0 starts at 0");
}

void test_set_instruction() {
    CPU cpu;
    cpu_init(&cpu);
    cpu.memory[0] = 0x88;
    cpu.memory[1] = 0x2A;
    cpu_step(&cpu);
    ASSERT(cpu.registers[0] == 42, "R0 is 42");
}

void test_add_instruction() {
    CPU cpu;
    cpu_init(&cpu);
    cpu.memory[0] = 0x88; // SET R0 5
    cpu.memory[1] = 0x05;
    
    cpu.memory[2] = 0x89; // SET R1 3
    cpu.memory[3] = 0x03;

    cpu.memory[4] = 0x02; // ADD RD R0, R1
    cpu.memory[5] = 0x04;

    cpu.memory[6] = 0xF0; // HALT
    cpu.memory[7] = 0x00;

    cpu_step(&cpu);
    cpu_step(&cpu);
    cpu_step(&cpu);
    cpu_step(&cpu);
    ASSERT(cpu.registers[2] == 8, "R2 is 8 (R0 + R1)");
}

int main() {
    test_cpu_init();
    test_set_instruction();
    test_add_instruction();
    return 0;
}
