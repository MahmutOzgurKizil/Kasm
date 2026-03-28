#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdint.h>
#include <stdio.h>
#define MAX_LABEL_LENGTH 20
#define MAX_LABEL_COUNT 13

typedef struct {
    char name[MAX_LABEL_LENGTH];
    uint8_t address;
} Label;

typedef struct {
    Label labels[MAX_LABEL_COUNT];
    uint8_t label_count;
    uint8_t current_address;
    uint8_t error;
    char error_msg[32];
} Assembler;

void assembler_init(Assembler *a);
void assembler_pass1 (Assembler *a, const char *source);
void assembler_pass2 (Assembler *a, const char *source, uint8_t *output);
uint8_t assembler_lookup_label(Assembler *a, const char *name);

#endif
