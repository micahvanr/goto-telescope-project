//======================================================================================//{
//  File: i2c.h
//  Overview: Provides functions for initializing, configuring, and using the I2C peripheral.
//  Features:
//      Master transmit (w/ interrupt capability)
//      Master receive (w/ interrupt capability)
//      Slave transmit (w/ interrupt capability)
//      Slave receive (w/ interrupt capability)
//  Notes: None
//======================================================================================//}

#ifndef I2C_H
#define I2C_H

//} Include Statements
//=========================================//{

#include "common.h"
#include "stm32f4xx.h"
#include <stdint.h>

//======================================================================================//}
//                  Public Constants
//======================================================================================//{

//} Address Definitions
//=========================================//{

typedef enum : uint32_t {
    I2C1_BASE_ADDR = ((APB1_BASE_ADDR) + (0x5400U)),
    I2C2_BASE_ADDR = ((APB1_BASE_ADDR) + (0x5800U)),
    I2C3_BASE_ADDR = ((APB1_BASE_ADDR) + (0x5C00U)),
} i2c_base_addr_e;

//} Config Options
//=========================================//{

typedef enum : uint32_t {
    I2C_SPEED_MODE_STANDARD = 0b0,
    I2C_SPEED_MODE_FAST     = 0b1,
} i2c_speed_mode_e;

typedef enum : uint32_t {
    I2C_FM_DUTY_2    = 0b0,
    I2C_FM_DUTY_16_9 = 0b1,
} i2c_fm_duty_cycle;

typedef enum : uint32_t {
    I2C_CLK_FREQ_100KHZ = 100000,
    I2C_CLK_FREQ_200KHZ = 200000,
    I2C_CLK_FREQ_300KHZ = 300000,
    I2C_CLK_FREQ_400KHZ = 400000,
} i2c_clk_freq_hz_e;

//} Callback Options
//=========================================//{

typedef enum : uint8_t {
    I2C_STATUS_READY   = 0, // Default
    I2C_STATUS_RUNNING = 1, // Only to be used in setting/getting of global status variable
    I2C_STATUS_MASTER_TX,
    I2C_STATUS_MASTER_RX,
    I2C_STATUS_SLAVE_TX,
    I2C_STATUS_SLAVE_RX,
} i2c_status_e;

typedef enum : uint8_t {
    I2C_REPEATED_START_DISABLE = 0,
    I2C_REPEATED_START_ENABLE,
} i2c_repeated_start_e;

//} Misc. Constants
//=========================================//{

// Used for callbacks
typedef enum : uint8_t {
    I2C_EVENT_CMPLT,
    I2C_EVENT_ERROR,
} i2c_event_e;

// Write = 0, read = 1
typedef enum : uint8_t {
    I2C_READ_WRITE_BIT = (1 << 0),
} i2c_read_write_bit_e;

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

// Register definition
typedef volatile struct {
    uint32_t CR1;   // Control register 1         Offset: 0x0
    uint32_t CR2;   // Control register 2         Offset: 0x4
    uint32_t OAR1;  // Own address register 1     Offset: 0x8
    uint32_t OAR2;  // Own address register 2     Offset: 0xC
    uint32_t DR;    // Data register              Offset: 0x10
    uint32_t SR1;   // Status register 1          Offset: 0x14
    uint32_t SR2;   // Status register 2          Offset: 0x18
    uint32_t CCR;   // Clock control register     Offset: 0x1C
    uint32_t TRISE; // TRISE register             Offset: 0x20
} i2c_reg_def;

// Configuration definition (used to initialize and configure I2C port settings)
typedef struct {
    uint8_t own_address;
    i2c_speed_mode_e speed_mode;     // Default: I2C_SPEED_MODE_STANDARD
    i2c_fm_duty_cycle fm_duty_cycle; // Default: I2C_FM_DUTY_2
    i2c_clk_freq_hz_e clock_freq_hz; // Default: I2C_CLK_FREQ_100KHZ
} i2c_config;

// Interrupt handle definition (used to handle the I2C interrupts)
typedef struct {
    uint8_t *txrx_buffer;
    uint32_t txrx_length;
    uint8_t target_addr;
    volatile i2c_status_e status;
    i2c_repeated_start_e repeated_start; // Default: I2C_REPEATED_START_DISABLE
} i2c_it_data;

// Handle definition (used to configure and handle the I2C features)
typedef struct {
    i2c_reg_def *p_i2cx;
    i2c_config i2c_conf;
    i2c_it_data i2c_it_data;
} i2c_handle;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define I2C1 ((i2c_reg_def *) I2C1_BASE_ADDR)
#define I2C2 ((i2c_reg_def *) I2C2_BASE_ADDR)
#define I2C3 ((i2c_reg_def *) I2C3_BASE_ADDR)

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

// Init and reset
void i2c_init(i2c_handle *const p_i2c_handle);
void i2c_reset(i2c_reg_def const *const p_i2cx);

// Write/read blocking or polling
void i2c_master_transmit(i2c_reg_def *const p_i2cx, uint8_t target_addr, uint8_t const *p_data, uint32_t length,
                         i2c_repeated_start_e repeated_start);
void i2c_master_receive(i2c_reg_def *const p_i2cx, uint8_t target_addr, uint8_t *p_data, uint32_t length,
                        i2c_repeated_start_e repeated_start);

void i2c_slave_transmit(i2c_reg_def *const p_i2cx, uint8_t const *p_data, uint32_t length);
void i2c_slave_receive(i2c_reg_def *const p_i2cx, uint8_t *p_data, uint32_t length);

// Write/read interrupt
void i2c_master_transmit_it(i2c_handle *p_i2c_handle, uint8_t target_addr, uint8_t *const p_data, uint32_t length,
                            i2c_repeated_start_e repeated_start);
void i2c_master_receive_it(i2c_handle *p_i2c_handle, uint8_t target_addr, uint8_t *const p_data, uint32_t length,
                           i2c_repeated_start_e repeated_start);

void i2c_slave_transmit_it(i2c_handle *const p_i2c_handle, uint8_t *const p_data, uint32_t length);
void i2c_slave_receive_it(i2c_handle *const p_i2c_handle, uint8_t *const p_data, uint32_t length);

// Interrupt handling
void i2c_toggable_it(i2c_reg_def *const p_i2cx, togglable_e toggle);
void i2c_it_handler(i2c_handle *const p_i2c_handle);

// User implement callback function to handle
void __WEAK i2c_callback(i2c_handle *p_i2c_handle, i2c_event_e event);

#endif // I2C_H

// *End of file*
