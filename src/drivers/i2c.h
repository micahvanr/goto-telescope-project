#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"

//======================================================================================//
//                  Address Definitions
//======================================================================================//

typedef enum {
    I2C1_BASE_ADDR = ((APB1_BASE_ADDR) + (0x5400U)),
    I2C2_BASE_ADDR = ((APB1_BASE_ADDR) + (0x5800U)),
    I2C3_BASE_ADDR = ((APB1_BASE_ADDR) + (0x5C00U)),
} i2c_base_addr_e;

//======================================================================================//
//                  Peripheral Constants
//======================================================================================//

//} Misc. Constants
//=========================================//{

// Used for callbacks
typedef enum {
    I2C_EVENT_CMPLT,
    I2C_EVENT_ERROR,
} i2c_event_e;

// Write = 0, read = 1
typedef enum {
    I2C_READ_WRITE_BIT = (1 << 0),
} i2c_read_write_bit_e;

//} Initialization Handler Constants
//=========================================//{

// Init check enum
typedef enum {
    I2C1_INIT_NUM = 0,
    I2C2_INIT_NUM = 1,
    I2C3_INIT_NUM = 2,
} i2c_init_port_num_e;

typedef enum {
    I2C_NOT_INITIALIZED = 0,
    I2C_INITIALIZED     = 1,
} i2c_init_check_e;

//} Interrupt Handler Constants
//=========================================//{

typedef enum {
    I2C_STATUS_READY   = 0, // Default
    I2C_STATUS_RUNNING = 1, // Only to be used in setting/getting of global status variable
    I2C_STATUS_MASTER_TX,
    I2C_STATUS_MASTER_RX,
    I2C_STATUS_SLAVE_TX,
    I2C_STATUS_SLAVE_RX,
} i2c_status_e;

typedef enum {
    I2C_REPEATED_START_DISABLE = 0,
    I2C_REPEATED_START_ENABLE,
} i2c_repeated_start_e;

//} Configuration Options
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

//======================================================================================//
//                  Register Constants
//======================================================================================//

// clang-format off
typedef enum : uint32_t {
    I2C_CR1_SWRST_POS     = 15U,   // Software reset
    I2C_CR1_ALERT_POS     = 13U,   // SMBus alert
    I2C_CR1_PEC_POS       = 12U,   // Packet error checking
    I2C_CR1_POS_POS       = 11U,   // Acknowledge/PEC Position (for data reception)
    I2C_CR1_ACK_POS       = 10U,   // Acknowledge enable
    I2C_CR1_STOP_POS      = 9U,    // Stop generation
    I2C_CR1_START_POS     = 8U,    // Start generation
    I2C_CR1_NOSTRETCH_POS = 7U,    // Clock stretching disable (Slave mode)
    I2C_CR1_ENGC_POS      = 6U,    // General call enable
    I2C_CR1_ENPEC_POS     = 5U,    // PEC enable
    I2C_CR1_ENARP_POS     = 4U,    // ARP enable
    I2C_CR1_SMBTYPE_POS   = 3U,    // SMBus type
    I2C_CR1_SMBUS_POS     = 1U,    // SMBus mode
    I2C_CR1_PE_POS        = 0U,    // Peripheral enable
} i2c_cr1_pos_e;

typedef enum : uint32_t {
    I2C_CR1_SWRST     = (1U << I2C_CR1_SWRST_POS),       // Software reset
    I2C_CR1_ALERT     = (1U << I2C_CR1_ALERT_POS),       // SMBus alert
    I2C_CR1_PEC       = (1U << I2C_CR1_PEC_POS),         // Packet error checking
    I2C_CR1_POS       = (1U << I2C_CR1_POS_POS),         // Acknowledge/PEC Position (for data reception)
    I2C_CR1_ACK       = (1U << I2C_CR1_ACK_POS),         // Acknowledge enable
    I2C_CR1_STOP      = (1U << I2C_CR1_STOP_POS),        // Stop generation
    I2C_CR1_START     = (1U << I2C_CR1_START_POS),       // Start generation
    I2C_CR1_NOSTRETCH = (1U << I2C_CR1_NOSTRETCH_POS),   // Clock stretching disable (Slave mode)
    I2C_CR1_ENGC      = (1U << I2C_CR1_ENGC_POS),        // General call enable
    I2C_CR1_ENPEC     = (1U << I2C_CR1_ENPEC_POS),       // PEC enable
    I2C_CR1_ENARP     = (1U << I2C_CR1_ENARP_POS),       // ARP enable
    I2C_CR1_SMBTYPE   = (1U << I2C_CR1_SMBTYPE_POS),     // SMBus type
    I2C_CR1_SMBUS     = (1U << I2C_CR1_SMBUS_POS),       // SMBus mode
    I2C_CR1_PE        = (1U << I2C_CR1_PE_POS),          // Peripheral enable
} i2c_cr1_e;

typedef enum : uint32_t {
    I2C_CR1_SWRST_MASK     = 0b1U,   // 1 bit(s)
    I2C_CR1_ALERT_MASK     = 0b1U,   // 1 bit(s)
    I2C_CR1_PEC_MASK       = 0b1U,   // 1 bit(s)
    I2C_CR1_POS_MASK       = 0b1U,   // 1 bit(s)
    I2C_CR1_ACK_MASK       = 0b1U,   // 1 bit(s)
    I2C_CR1_STOP_MASK      = 0b1U,   // 1 bit(s)
    I2C_CR1_START_MASK     = 0b1U,   // 1 bit(s)
    I2C_CR1_NOSTRETCH_MASK = 0b1U,   // 1 bit(s)
    I2C_CR1_ENGC_MASK      = 0b1U,   // 1 bit(s)
    I2C_CR1_ENPEC_MASK     = 0b1U,   // 1 bit(s)
    I2C_CR1_ENARP_MASK     = 0b1U,   // 1 bit(s)
    I2C_CR1_SMBTYPE_MASK   = 0b1U,   // 1 bit(s)
    I2C_CR1_SMBUS_MASK     = 0b1U,   // 1 bit(s)
    I2C_CR1_PE_MASK        = 0b1U,   // 1 bit(s)
} i2c_cr1_mask_e;

typedef enum : uint32_t {
    I2C_CR2_LAST_POS    = 12U,   // DMA last transfer
    I2C_CR2_DMAEN_POS   = 11U,   // DMA requests enable
    I2C_CR2_ITBUFEN_POS = 10U,   // Buffer interrupt enable
    I2C_CR2_ITEVTEN_POS = 9U,    // Event interrupt enable
    I2C_CR2_ITERREN_POS = 8U,    // Error interrupt enable
    I2C_CR2_FREQ_POS    = 0U,    // Peripheral clock frequency
} i2c_cr2_pos_e;

typedef enum : uint32_t {
    I2C_CR2_LAST    = (1U << I2C_CR2_LAST_POS),      // DMA last transfer
    I2C_CR2_DMAEN   = (1U << I2C_CR2_DMAEN_POS),     // DMA requests enable
    I2C_CR2_ITBUFEN = (1U << I2C_CR2_ITBUFEN_POS),   // Buffer interrupt enable
    I2C_CR2_ITEVTEN = (1U << I2C_CR2_ITEVTEN_POS),   // Event interrupt enable
    I2C_CR2_ITERREN = (1U << I2C_CR2_ITERREN_POS),   // Error interrupt enable
    I2C_CR2_FREQ    = (1U << I2C_CR2_FREQ_POS),      // Peripheral clock frequency
} i2c_cr2_e;

typedef enum : uint32_t {
    I2C_CR2_LAST_MASK    = 0b1U,        // 1 bit(s)
    I2C_CR2_DMAEN_MASK   = 0b1U,        // 1 bit(s)
    I2C_CR2_ITBUFEN_MASK = 0b1U,        // 1 bit(s)
    I2C_CR2_ITEVTEN_MASK = 0b1U,        // 1 bit(s)
    I2C_CR2_ITERREN_MASK = 0b1U,        // 1 bit(s)
    I2C_CR2_FREQ_MASK    = 0b111111U,   // 6 bit(s)
} i2c_cr2_mask_e;

typedef enum : uint32_t {
    I2C_OAR1_ADDMODE_POS = 15U,   // Addressing mode (slave mode)
    I2C_OAR1_ADD10_POS   = 8U,    // Interface address
    I2C_OAR1_ADD7_POS    = 1U,    // Interface address
    I2C_OAR1_ADD0_POS    = 0U,    // Interface address
} i2c_oar1_pos_e;

typedef enum : uint32_t {
    I2C_OAR1_ADDMODE = (1U << I2C_OAR1_ADDMODE_POS),   // Addressing mode (slave mode)
    I2C_OAR1_ADD10   = (1U << I2C_OAR1_ADD10_POS),     // Interface address
    I2C_OAR1_ADD7    = (1U << I2C_OAR1_ADD7_POS),      // Interface address
    I2C_OAR1_ADD0    = (1U << I2C_OAR1_ADD0_POS),      // Interface address
} i2c_oar1_e;

typedef enum : uint32_t {
    I2C_OAR1_ADDMODE_MASK = 0b1U,         // 1 bit(s)
    I2C_OAR1_ADD10_MASK   = 0b11U,        // 2 bit(s)
    I2C_OAR1_ADD7_MASK    = 0b1111111U,   // 7 bit(s)
    I2C_OAR1_ADD0_MASK    = 0b1U,         // 1 bit(s)
} i2c_oar1_mask_e;

typedef enum : uint32_t {
    I2C_OAR2_ADD2_POS   = 1U,   // Interface address
    I2C_OAR2_ENDUAL_POS = 0U,   // Dual addressing mode enable
} i2c_oar2_pos_e;

typedef enum : uint32_t {
    I2C_OAR2_ADD2   = (1U << I2C_OAR2_ADD2_POS),     // Interface address
    I2C_OAR2_ENDUAL = (1U << I2C_OAR2_ENDUAL_POS),   // Dual addressing mode enable
} i2c_oar2_e;

typedef enum : uint32_t {
    I2C_OAR2_ADD2_MASK   = 0b1111111U,   // 7 bit(s)
    I2C_OAR2_ENDUAL_MASK = 0b1U,         // 1 bit(s)
} i2c_oar2_mask_e;

typedef enum : uint32_t {
    I2C_DR_DR_POS = 0U,   // 8-bit data register
} i2c_dr_pos_e;

typedef enum : uint32_t {
    I2C_DR_DR = (1U << I2C_DR_DR_POS),   // 8-bit data register
} i2c_dr_e;

typedef enum : uint32_t {
    I2C_DR_DR_MASK = 0b11111111U,   // 8 bit(s)
} i2c_dr_mask_e;

typedef enum : uint32_t {
    I2C_SR1_SMBALERT_POS = 15U,   // SMBus alert
    I2C_SR1_TIMEOUT_POS  = 14U,   // Timeout or Tlow error
    I2C_SR1_PECERR_POS   = 12U,   // PEC Error in reception
    I2C_SR1_OVR_POS      = 11U,   // Overrun/Underrun
    I2C_SR1_AF_POS       = 10U,   // Acknowledge failure
    I2C_SR1_ARLO_POS     = 9U,    // Arbitration lost (master mode)
    I2C_SR1_BERR_POS     = 8U,    // Bus error
    I2C_SR1_TXE_POS      = 7U,    // Data register empty (transmitters)
    I2C_SR1_RXNE_POS     = 6U,    // Data register not empty (receivers)
    I2C_SR1_STOPF_POS    = 4U,    // Stop detection (slave mode)
    I2C_SR1_ADD10_POS    = 3U,    // 10-bit header sent (Master mode)
    I2C_SR1_BTF_POS      = 2U,    // Byte transfer finished
    I2C_SR1_ADDR_POS     = 1U,    // Address sent (master mode)/matched (slave mode)
    I2C_SR1_SB_POS       = 0U,    // Start bit (Master mode)
} i2c_sr1_pos_e;

typedef enum : uint32_t {
    I2C_SR1_SMBALERT = (1U << I2C_SR1_SMBALERT_POS),   // SMBus alert
    I2C_SR1_TIMEOUT  = (1U << I2C_SR1_TIMEOUT_POS),    // Timeout or Tlow error
    I2C_SR1_PECERR   = (1U << I2C_SR1_PECERR_POS),     // PEC Error in reception
    I2C_SR1_OVR      = (1U << I2C_SR1_OVR_POS),        // Overrun/Underrun
    I2C_SR1_AF       = (1U << I2C_SR1_AF_POS),         // Acknowledge failure
    I2C_SR1_ARLO     = (1U << I2C_SR1_ARLO_POS),       // Arbitration lost (master mode)
    I2C_SR1_BERR     = (1U << I2C_SR1_BERR_POS),       // Bus error
    I2C_SR1_TXE      = (1U << I2C_SR1_TXE_POS),        // Data register empty (transmitters)
    I2C_SR1_RXNE     = (1U << I2C_SR1_RXNE_POS),       // Data register not empty (receivers)
    I2C_SR1_STOPF    = (1U << I2C_SR1_STOPF_POS),      // Stop detection (slave mode)
    I2C_SR1_ADD10    = (1U << I2C_SR1_ADD10_POS),      // 10-bit header sent (Master mode)
    I2C_SR1_BTF      = (1U << I2C_SR1_BTF_POS),        // Byte transfer finished
    I2C_SR1_ADDR     = (1U << I2C_SR1_ADDR_POS),       // Address sent (master mode)/matched (slave mode)
    I2C_SR1_SB       = (1U << I2C_SR1_SB_POS),         // Start bit (Master mode)
} i2c_sr1_e;

typedef enum : uint32_t {
    I2C_SR1_SMBALERT_MASK = 0b1U,   // 1 bit(s)
    I2C_SR1_TIMEOUT_MASK  = 0b1U,   // 1 bit(s)
    I2C_SR1_PECERR_MASK   = 0b1U,   // 1 bit(s)
    I2C_SR1_OVR_MASK      = 0b1U,   // 1 bit(s)
    I2C_SR1_AF_MASK       = 0b1U,   // 1 bit(s)
    I2C_SR1_ARLO_MASK     = 0b1U,   // 1 bit(s)
    I2C_SR1_BERR_MASK     = 0b1U,   // 1 bit(s)
    I2C_SR1_TXE_MASK      = 0b1U,   // 1 bit(s)
    I2C_SR1_RXNE_MASK     = 0b1U,   // 1 bit(s)
    I2C_SR1_STOPF_MASK    = 0b1U,   // 1 bit(s)
    I2C_SR1_ADD10_MASK    = 0b1U,   // 1 bit(s)
    I2C_SR1_BTF_MASK      = 0b1U,   // 1 bit(s)
    I2C_SR1_ADDR_MASK     = 0b1U,   // 1 bit(s)
    I2C_SR1_SB_MASK       = 0b1U,   // 1 bit(s)
} i2c_sr1_mask_e;

typedef enum : uint32_t {
    I2C_SR2_PEC_POS        = 8U,   // acket error checking register
    I2C_SR2_DUALF_POS      = 7U,   // Dual flag (Slave mode)
    I2C_SR2_SMBHOST_POS    = 6U,   // SMBus host header (Slave mode)
    I2C_SR2_SMBDEFAULT_POS = 5U,   // SMBus device default address (Slave mode)
    I2C_SR2_GENCALL_POS    = 4U,   // General call address (Slave mode)
    I2C_SR2_TRA_POS        = 2U,   // Transmitter/receiver
    I2C_SR2_BUSY_POS       = 1U,   // Bus busy
    I2C_SR2_MSL_POS        = 0U,   // Master/slave
} i2c_sr2_pos_e;

typedef enum : uint32_t {
    I2C_SR2_PEC        = (1U << I2C_SR2_PEC_POS),          // acket error checking register
    I2C_SR2_DUALF      = (1U << I2C_SR2_DUALF_POS),        // Dual flag (Slave mode)
    I2C_SR2_SMBHOST    = (1U << I2C_SR2_SMBHOST_POS),      // SMBus host header (Slave mode)
    I2C_SR2_SMBDEFAULT = (1U << I2C_SR2_SMBDEFAULT_POS),   // SMBus device default address (Slave mode)
    I2C_SR2_GENCALL    = (1U << I2C_SR2_GENCALL_POS),      // General call address (Slave mode)
    I2C_SR2_TRA        = (1U << I2C_SR2_TRA_POS),          // Transmitter/receiver
    I2C_SR2_BUSY       = (1U << I2C_SR2_BUSY_POS),         // Bus busy
    I2C_SR2_MSL        = (1U << I2C_SR2_MSL_POS),          // Master/slave
} i2c_sr2_e;

typedef enum : uint32_t {
    I2C_SR2_PEC_MASK        = 0b11111111U,   // 8 bit(s)
    I2C_SR2_DUALF_MASK      = 0b1U,          // 1 bit(s)
    I2C_SR2_SMBHOST_MASK    = 0b1U,          // 1 bit(s)
    I2C_SR2_SMBDEFAULT_MASK = 0b1U,          // 1 bit(s)
    I2C_SR2_GENCALL_MASK    = 0b1U,          // 1 bit(s)
    I2C_SR2_TRA_MASK        = 0b1U,          // 1 bit(s)
    I2C_SR2_BUSY_MASK       = 0b1U,          // 1 bit(s)
    I2C_SR2_MSL_MASK        = 0b1U,          // 1 bit(s)
} i2c_sr2_mask_e;

typedef enum : uint32_t {
    I2C_CCR_F_S_POS  = 15U,   // I2C master mode selection
    I2C_CCR_DUTY_POS = 14U,   // Fast mode duty cycle
    I2C_CCR_CCR_POS  = 0U,    // Clock control register in Fast/Standard mode (Master mode)
} i2c_ccr_pos_e;

typedef enum : uint32_t {
    I2C_CCR_F_S  = (1U << I2C_CCR_F_S_POS),    // I2C master mode selection
    I2C_CCR_DUTY = (1U << I2C_CCR_DUTY_POS),   // Fast mode duty cycle
    I2C_CCR_CCR  = (1U << I2C_CCR_CCR_POS),    // Clock control register in Fast/Standard mode (Master mode)
} i2c_ccr_e;

typedef enum : uint32_t {
    I2C_CCR_F_S_MASK  = 0b1U,              // 1 bit(s)
    I2C_CCR_DUTY_MASK = 0b1U,              // 1 bit(s)
    I2C_CCR_CCR_MASK  = 0b111111111111U,   // 12 bit(s)
} i2c_ccr_mask_e;

typedef enum : uint32_t {
    I2C_TRISE_TRISE_POS = 0U,   // Maximum rise time in Fast/Standard mode (Master mode)
} i2c_trise_pos_e;

typedef enum : uint32_t {
    I2C_TRISE_TRISE = (1U << I2C_TRISE_TRISE_POS),   // Maximum rise time in Fast/Standard mode (Master mode)
} i2c_trise_e;

typedef enum : uint32_t {
    I2C_TRISE_TRISE_MASK = 0b111111U,   // 6 bit(s)
} i2c_trise_mask_e;

// clang-format on

//======================================================================================//
//                  Structure Definitions
//======================================================================================//

// Register definition
typedef __vo struct {
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
    __vo i2c_status_e status;
    i2c_repeated_start_e repeated_start; // Default: I2C_REPEATED_START_DISABLE
} i2c_it_data;

// Handle definition (used to configure and handle the I2C features)
typedef struct {
    i2c_reg_def *p_i2cx;
    i2c_config i2c_conf;
    i2c_it_data i2c_it_data;
} i2c_handle;

//======================================================================================//
//                  Peripheral Structure Macros
//======================================================================================//

#define I2C1 ((i2c_reg_def *)I2C1_BASE_ADDR)
#define I2C2 ((i2c_reg_def *)I2C2_BASE_ADDR)
#define I2C3 ((i2c_reg_def *)I2C3_BASE_ADDR)

//======================================================================================//
//                  Function API Prototypes
//======================================================================================//

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
void i2c_it_config(i2c_reg_def const *const p_i2cx, togglable_e toggle);
void i2c_it_handler(i2c_handle *const p_i2c_handle);

// User implement callback function to handle
void __weak i2c_callback(i2c_handle *p_i2c_handle, i2c_event_e event);

#endif
