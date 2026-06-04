#ifndef I2C_TEST_H
#define I2C_TEST_H

#include "printf.h"
#include "test_types.h"

typedef enum {
    COMMAND_LEN  = 0x51,
    COMMAND_READ = 0x52,
    TARGET_ADDR  = 0x68,
} i2c_test_enums;

void i2c_tests(test_type_e test);

#endif
