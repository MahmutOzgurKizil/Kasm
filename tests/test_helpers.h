#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <stdio.h>

#define ASSERT(condition, message) \
    if (condition) { printf("PASS: %s\n", message); } \
    else { printf("FAIL: %s\n", message); }

#endif
