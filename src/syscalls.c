// Dummy functions for compiler warnings

#include "common/assert_handler.h"

void _close(void) {
    ASSERT(FALSE);
}

void _close_r(void) {
    ASSERT(FALSE);
}

void _lseek(void) {
    ASSERT(FALSE);
}

void _lseek_r(void) {
    ASSERT(FALSE);
}

void _read(void) {
    ASSERT(FALSE);
}

void _read_r(void) {
    ASSERT(FALSE);
}

void _write(void) {
    ASSERT(FALSE);
}

void _write_r(void) {
    ASSERT(FALSE);
}
