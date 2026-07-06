#ifndef I2C_TEST_H
#define I2C_TEST_H

#include "printf.h"
#include "test_types.h"

typedef enum {
    TARGET_ADDR = 0x68,
} i2c_test_enums;

void i2c_tests(test_type_e test);

#endif
