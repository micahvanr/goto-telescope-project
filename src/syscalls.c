// Dummy functions for compiler warnings

#include "common/assert_handler.h"
#include <stdbool.h>

void _close(void) {
    ASSERT(false);
}

void _close_r(void) {
    ASSERT(false);
}

void _lseek(void) {
    ASSERT(false);
}

void _lseek_r(void) {
    ASSERT(false);
}

void _read(void) {
    ASSERT(false);
}

void _read_r(void) {
    ASSERT(false);
}

void _write(void) {
    ASSERT(false);
}

void _write_r(void) {
    ASSERT(false);
}
