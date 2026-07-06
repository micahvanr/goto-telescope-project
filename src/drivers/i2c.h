#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"

/****************************************************************************************************
                                    Address Definitions
****************************************************************************************************/
typedef enum {
    I2C1_BASE_ADDR = ((APB1_BASE_ADDR) + (0x5400u)),
    I2C2_BASE_ADDR = ((APB1_BASE_ADDR) + (0x5800u)),
    I2C3_BASE_ADDR = ((APB1_BASE_ADDR) + (0x5C00u)),
} i2c_base_addr_e;

/****************************************************************************************************
                                    Macros and Other Enums
****************************************************************************************************/

// Used for callbacks
typedef enum {
    I2C_EVENT_CMPLT,
    I2C_EVENT_ERROR,
} i2c_event_e;

// Init check enum
typedef enum {
    I2C1_INIT_NUM = 0,
    I2C2_INIT_NUM = 1,
    I2C3_INIT_NUM = 2,
} i2c_init_port_num_e;

typedef enum {
    I2C1_STATUS_NUM = 0,
    I2C2_STATUS_NUM = 1,
    I2C3_STATUS_NUM = 2,
} i2c_status_port_num_e;

typedef enum {
    I2C_REPEATED_START_DISABLE,
    I2C_REPEATED_START_ENABLE,
} i2c_repeated_start_e;

// Write = 0, read = 1
typedef enum {
    I2C_READ_WRITE_BIT = (1 << 0),
} i2c_read_write_bit_e;

typedef enum {
    I2C_READ_SEL,
    I2C_WRITE_SEL,
} i2c_read_write_sel_e;

typedef enum {
    I2C_STATUS_READY   = 0, // Default
    I2C_STATUS_RUNNING = 1, // Only to be used in setting/getting of global status variable
    I2C_STATUS_MASTER_TX,
    I2C_STATUS_MASTER_RX,
    I2C_STATUS_SLAVE_TX,
    I2C_STATUS_SLAVE_RX,
} i2c_status_e;

// TRISE
typedef enum {
    I2C_MAX_RISE_SM_1000_NS = 1000,
    I2C_MAX_RISE_FM_300_NS  = 300,
} i2c_max_rise_time_e;

// Register bits
typedef enum {
    I2C_CR1_PE_POS         = 0,
    I2C_CR1_SMBUS_POS      = 1,
    I2C_CR1_SMBTYPE_POS    = 3,
    I2C_CR1_ENARP_POS      = 4,
    I2C_CR1_ENPEC_POS      = 5,
    I2C_CR1_ENGC_POS       = 6,
    I2C_CR1_NO_STRETCH_POS = 7,
    I2C_CR1_START_POS      = 8,
    I2C_CR1_STOP_POS       = 9,
    I2C_CR1_ACK_POS        = 10,
    I2C_CR1_POS_POS        = 11,
    I2C_CR1_PEC_POS        = 12,
    I2C_CR1_ALERT_POS      = 13,
    I2C_CR1_SWRST_POS      = 15,
} i2c_cr1_pos_e;

typedef enum {
    I2C_CR1_PE         = (1 << I2C_CR1_PE_POS),
    I2C_CR1_SMBUS      = (1 << I2C_CR1_SMBUS_POS),
    I2C_CR1_SMBTYPE    = (1 << I2C_CR1_SMBTYPE_POS),
    I2C_CR1_ENARP      = (1 << I2C_CR1_ENARP_POS),
    I2C_CR1_ENPEC      = (1 << I2C_CR1_ENPEC_POS),
    I2C_CR1_ENGC       = (1 << I2C_CR1_ENGC_POS),
    I2C_CR1_NO_STRETCH = (1 << I2C_CR1_NO_STRETCH_POS),
    I2C_CR1_START      = (1 << I2C_CR1_START_POS),
    I2C_CR1_STOP       = (1 << I2C_CR1_STOP_POS),
    I2C_CR1_ACK        = (1 << I2C_CR1_ACK_POS),
    I2C_CR1_POS        = (1 << I2C_CR1_POS_POS),
    I2C_CR1_PEC        = (1 << I2C_CR1_PEC_POS),
    I2C_CR1_ALERT      = (1 << I2C_CR1_ALERT_POS),
    I2C_CR1_SWRST      = (1 << I2C_CR1_SWRST_POS),
} i2c_cr1_e;

typedef enum {
    I2C_CR2_FREQ_MASK = 0b11111, // 5 bits
} i2c_cr2_mask_e;

typedef enum {
    I2C_CR2_FREQ_POS    = 0,
    I2C_CR2_ITERREN_POS = 8,
    I2C_CR2_ITEVTEN_POS = 9,
    I2C_CR2_ITBUFEN_POS = 10,
    I2C_CR2_DMAEN_POS   = 11,
    I2C_CR2_LAST_POS    = 12,
} i2c_cr2_pos_e;

typedef enum {
    I2C_CR2_FREQ    = (1 << I2C_CR2_FREQ_POS),
    I2C_CR2_ITERREN = (1 << I2C_CR2_ITERREN_POS),
    I2C_CR2_ITEVTEN = (1 << I2C_CR2_ITEVTEN_POS),
    I2C_CR2_ITBUFEN = (1 << I2C_CR2_ITBUFEN_POS),
    I2C_CR2_DMAEN   = (1 << I2C_CR2_DMAEN_POS),
    I2C_CR2_LAST    = (1 << I2C_CR2_LAST_POS),
} i2c_cr2_e;

typedef enum {
    I2C_OAR1_ADD0_POS    = 0,
    I2C_OAR1_ADD1_POS    = 1,
    I2C_OAR1_ADDMODE_POS = 15,
} i2c_oar1_pos_e;

typedef enum {
    I2C_OAR2_ENDUAL_POS = 0,
    I2C_OAR2_ADD2_POS   = 1,
} i2c_oar2_pos_e;

typedef enum {
    I2C_SR1_SB_POS       = 0,
    I2C_SR1_ADDR_POS     = 1,
    I2C_SR1_BTF_POS      = 2,
    I2C_SR1_ADD10_POS    = 3,
    I2C_SR1_STOPF_POS    = 4,
    I2C_SR1_RxNE_POS     = 6,
    I2C_SR1_TxE_POS      = 7,
    I2C_SR1_BERR_POS     = 8,
    I2C_SR1_ARLO_POS     = 9,
    I2C_SR1_AF_POS       = 10,
    I2C_SR1_OVR_POS      = 11,
    I2C_SR1_PECERR_POS   = 12,
    I2C_SR1_TIMEOUT_POS  = 14,
    I2C_SR1_SMBALERT_POS = 15,
} i2c_sr1_pos_e;

typedef enum {
    I2C_SR1_SB       = (1 << I2C_SR1_SB_POS),
    I2C_SR1_ADDR     = (1 << I2C_SR1_ADDR_POS),
    I2C_SR1_BTF      = (1 << I2C_SR1_BTF_POS),
    I2C_SR1_ADD10    = (1 << I2C_SR1_ADD10_POS),
    I2C_SR1_STOPF    = (1 << I2C_SR1_STOPF_POS),
    I2C_SR1_TxE      = (1 << I2C_SR1_TxE_POS),
    I2C_SR1_RxNE     = (1 << I2C_SR1_RxNE_POS),
    I2C_SR1_BERR     = (1 << I2C_SR1_BERR_POS),
    I2C_SR1_ARLO     = (1 << I2C_SR1_ARLO_POS),
    I2C_SR1_AF       = (1 << I2C_SR1_AF_POS),
    I2C_SR1_OVR      = (1 << I2C_SR1_OVR_POS),
    I2C_SR1_PECERR   = (1 << I2C_SR1_PECERR_POS),
    I2C_SR1_TIMEOUT  = (1 << I2C_SR1_TIMEOUT_POS),
    I2C_SR1_SMBALERT = (1 << I2C_SR1_SMBALERT_POS),
} i2c_sr1_e;

typedef enum {
    I2C_SR2_MSL_POS        = 0,
    I2C_SR2_BUSY_POS       = 1,
    I2C_SR2_TRA_POS        = 2,
    I2C_SR2_GENCALL_POS    = 4,
    I2C_SR2_SMBDEFAULT_POS = 5,
    I2C_SR2_SMBHOST_POS    = 6,
    I2C_SR2_DUALF_POS      = 7,
    I2C_SR2_PEC_POS        = 8,
} i2c_sr2_pos_e;

typedef enum {
    I2C_SR2_MSL        = (1 << I2C_SR2_MSL_POS),
    I2C_SR2_BUSY       = (1 << I2C_SR2_BUSY_POS),
    I2C_SR2_TRA        = (1 << I2C_SR2_TRA_POS),
    I2C_SR2_GENCALL    = (1 << I2C_SR2_GENCALL_POS),
    I2C_SR2_SMBDEFAULT = (1 << I2C_SR2_SMBDEFAULT_POS),
    I2C_SR2_SMBHOST    = (1 << I2C_SR2_SMBHOST_POS),
    I2C_SR2_DUALF      = (1 << I2C_SR2_DUALF_POS),
    I2C_SR2_PEC        = (1 << I2C_SR2_PEC_POS),
} i2c_sr2_e;

typedef enum {
    I2C_CCR_CCR_POS  = 0,
    I2C_CCR_DUTY_POS = 14,
    I2C_CCR_FS_POS   = 15,
} i2c_ccr_pos_e;

typedef enum {
    I2C_CCR_CCR  = (1 << I2C_CCR_CCR_POS),
    I2C_CCR_DUTY = (1 << I2C_CCR_DUTY_POS),
    I2C_CCR_FS   = (1 << I2C_CCR_FS_POS),
} i2c_ccr_e;

typedef enum {
    I2C_CCR_MASK = 0xFFF, // 12 bits
} i2c_ccr_mask_e;

typedef enum {
    I2C_TRISE_MASK = 0b111111, // 6 bits
} i2c_trise_mask_e;

// Configuration settings
typedef enum {
    I2C_SPEED_MODE_STANDARD = 0, // Default
    I2C_SPEED_MODE_FAST     = 1,
} i2c_speed_mode_e;

typedef enum {
    I2C_FM_DUTY_2    = 0, // Default
    I2C_FM_DUTY_16_9 = 1,
} i2c_fm_duty_cycle;

typedef enum {
    I2C_CLK_FREQ_100KHZ = 100000, // Default
    I2C_CLK_FREQ_200KHZ = 200000,
    I2C_CLK_FREQ_300KHZ = 300000,
    I2C_CLK_FREQ_400KHZ = 400000,
} i2c_clk_freq_hz_e;

// Structure Definitions
typedef struct {
    uint32_t CR1;   // Control register 1           Offset: 0x00
    uint32_t CR2;   // Control register 2           Offset: 0x04
    uint32_t OAR1;  // Own address register 1       Offset: 0x08
    uint32_t OAR2;  // Own address register 2       Offset: 0x0C
    uint32_t DR;    // Data register                Offset: 0x10
    uint32_t SR1;   // Status register 1            Offset: 0x14
    uint32_t SR2;   // Status register 2            Offset: 0x18
    uint32_t CCR;   // Clock control register       Offset: 0x1C
    uint32_t TRISE; // TRISE register               Offset: 0x20
    uint32_t FLTR;  // FLTR register                Offset: 0x24
} i2c_reg_def;

typedef struct {
    // Modes (slave tx/rx || master tx/rx)
    // peripheral frequency
    uint8_t own_address;
    i2c_speed_mode_e speed_mode;
    i2c_fm_duty_cycle fm_duty_cycle;
    i2c_clk_freq_hz_e clock_freq_hz;
} i2c_config;

typedef struct {
    uint8_t *txrx_buffer;
    uint32_t txrx_length;
    uint8_t target_addr;
    __vo i2c_status_e status;
    i2c_repeated_start_e repeated_start;
} i2c_it_data;

typedef struct {
    i2c_reg_def *p_i2cx;
    i2c_config i2c_conf;
    i2c_it_data i2c_it_data;
} i2c_handle;

/****************************************************************************************************
                                    Peripheral Structure Definitions
****************************************************************************************************/

#define I2C1 ((i2c_reg_def *)I2C1_BASE_ADDR)
#define I2C2 ((i2c_reg_def *)I2C2_BASE_ADDR)
#define I2C3 ((i2c_reg_def *)I2C3_BASE_ADDR)

/****************************************************************************************************
                                    Peripheral Function APIs
****************************************************************************************************/
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
void i2c_master_transmit_it(i2c_handle *p_i2c_handle, uint8_t target_addr, uint8_t *p_data, uint32_t length,
                            i2c_repeated_start_e repeated_start);
void i2c_master_receive_it(i2c_handle *p_i2c_handle, uint8_t target_addr, uint8_t *p_data, uint32_t length,
                           i2c_repeated_start_e repeated_start);

void i2c_slave_transmit_it(i2c_handle *p_i2c_handle, uint8_t *p_data, uint32_t length);
void i2c_slave_receive_it(i2c_handle *p_i2c_handle, uint8_t *p_data, uint32_t length);

// Interrupt handling
void i2c_it_config(i2c_reg_def const *const p_i2cx, togglable_e toggle);
void i2c_it_handler(i2c_handle *const p_i2c_handle);

// User implement callback function to handle
void __weak i2c_callback(i2c_handle *p_i2c_handle, i2c_event_e event);

#endif
