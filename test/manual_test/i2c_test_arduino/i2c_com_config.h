#ifndef I2C_COM_CONFIG_H
#define I2C_COM_CONFIG_H

#include <stdint.h>

typedef enum : uint8_t {
    I2C_TEST_STM_MASTER_TX,
    I2C_TEST_STM_MASTER_RX,
    I2C_TEST_STM_SLAVE_TX,
    I2C_TEST_STM_SLAVE_RX,
} i2c_com_config_options_e;

typedef enum : uint32_t {
    COMMAND_LEN  = 0x51,
    COMMAND_READ = 0x52,
    I2C_ARDUINO_ADDR = 0x68,
    I2C_STM_ADDR = 0x42,
    // Change this enum to change the mode
    I2C_TEST_MODE = I2C_TEST_STM_MASTER_TX,

} i2c_com_config_sel_e;

#endif 
