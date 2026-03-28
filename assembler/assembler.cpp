#include "assembler.h"
#include <string.h>

void assembler_init(Assembler *a) {
    memset(a->labels, 0, sizeof(a->labels));
    a->label_count = 0;
    a->current_address = 0;
    a->error = 0;
    memset(a->error_msg, 0, sizeof(a->error_msg));
}
