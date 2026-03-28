#include "assembler.h"
#include <string.h>


static void trim(char *s) {
    int start = 0;
    while (s[start] == ' ' || s[start] == '\t') start++;
    if (start > 0) memmove(s, s + start, strlen(s) - start + 1);

    int end = strlen(s) - 1;
    while (end >= 0 && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r')) {
        s[end] = '\0';
        end--;
    }
}

void assembler_init(Assembler *a) {
    memset(a->labels, 0, sizeof(a->labels));
    a->label_count = 0;
    a->current_address = 0;
    a->error = 0;
    memset(a->error_msg, 0, sizeof(a->error_msg));
}

void assembler_pass1(Assembler *a, const char *source) {
    char buffer[MAX_SOURCE_LENGTH];
    strncpy(buffer, source, sizeof(buffer));
    buffer[MAX_SOURCE_LENGTH - 1] = '\0';

    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        char *comment = strchr(line, ';');
        if (comment != NULL) *comment = '\0';
        trim(line);

        int line_length = strlen(line);

        if (line_length == 0) {
            line = strtok(NULL, "\n");
            continue;
        }

        if (line[line_length - 1] == ':') {
            if (a->label_count >= MAX_LABEL_COUNT) {
                a->error = 1;
                snprintf(a->error_msg, sizeof(a->error_msg), "Too many labels");
                return;
            }
            if (line_length - 1 > MAX_LABEL_LENGTH) {
                a->error = 1;
                snprintf(a->error_msg, sizeof(a->error_msg), "Label too long: %s", line);
                return;
            }
            strncpy(a->labels[a->label_count].name, line, line_length - 1);
            a->labels[a->label_count].name[line_length - 1] = '\0';
            a->labels[a->label_count].address = a->current_address;
            a->label_count++;
        } else {
            a->current_address += 2;
        }

        line = strtok(NULL, "\n");
    }
}
