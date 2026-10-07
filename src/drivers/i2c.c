/*****************************************************************************************
File: i2c.c
Overview: Provides functions for initializing, configuring, and using the I2C peripheral.
Features:
    Master transmit (w/ interrupt capability)
    Master receive (w/ interrupt capability)
    Slave transmit (w/ interrupt capability)
    Slave receive (w/ interrupt capability)
Notes: None
******************************************************************************************/

//} Include Statements
//=========================================//{

#include "i2c.h"
#include "assert_handler.h"
#include "nvic.h"
#include "rcc.h"

//======================================================================================//}
//                  Private Constants
//======================================================================================//{

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

//========================================================//}
//          Register Constants
//========================================================//{

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

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

// I2C1 = bit pos 0
// I2C2 = bit pos 1
// I2C3 = bit pos 2
uint8_t g_i2c_port_init = 0;
// If peripheral bit is 1, it is running. If 0, it is free.
uint8_t g_i2c_port_status = 0;

//======================================================================================//}
//                  Private Function Prototypes
//======================================================================================//{

// Assert helper functions
static inline void assert_i2c_handle_param(i2c_handle const *const p_i2c_handle);
static inline void set_i2c_init_status(i2c_reg_def const *const p_i2cx);
static inline i2c_init_check_e get_i2c_init_status(i2c_reg_def const *const p_i2cx);

// General helper functions
static inline i2c_init_port_num_e map_i2c_ports_to_num(i2c_reg_def const *const p_i2cx);
static void set_ccr_and_trise(i2c_handle const *const p_i2c_handle);

static inline void generate_start_condition(i2c_reg_def *const p_i2cx);
static inline void generate_stop_condition(i2c_reg_def *const p_i2cx);
static inline void clear_start_flag(i2c_reg_def const *const p_i2cx);
static inline void send_address(i2c_reg_def *const p_i2cx, uint8_t target_addr, i2c_status_e i2c_status);
static inline void clear_address_flag(i2c_reg_def const *const p_i2cx);
static inline void config_ack(i2c_reg_def *const p_i2cx, togglable_e toggle);
static inline void clear_stopf(i2c_reg_def *const p_i2cx);
static inline void clear_af(i2c_reg_def *const p_i2cx);

static inline void send_byte(i2c_handle *const p_i2c_handle);
static inline void receive_byte(i2c_handle *const p_i2c_handle);

static inline void close_com(i2c_handle *const p_i2c_handle);

static void master_sb(i2c_handle *const p_i2c_handle);
static void master_addr(i2c_handle *const p_i2c_handle);
static void slave_addr(i2c_handle const *const p_i2c_handle);

//======================================================================================//}
//                  Public Function Implementation
//======================================================================================//{

/***************************************************************************
Function: i2c_init
Overview: Initializes the I2C peripheral inside the handle with the settings in the configuration structure.
Parameters:
    p_i2c_handle: Structure with the corresponding settings to configure the I2C peripheral.
Return: 
    None
Note: 10-bit addressing not implemented.
***************************************************************************/
void i2c_init(i2c_handle *const p_i2c_handle)
{
    // Ensure handle structure values are valid.
    assert_i2c_handle_param(p_i2c_handle);

    // Initialize the port in the init variable.
    set_i2c_init_status(p_i2c_handle->p_i2cx);
    // Enable clock
    rcc_i2c_togglable_clock(p_i2c_handle->p_i2cx, ENABLE);

    // Set peripheral clock frequency
    p_i2c_handle->p_i2cx->CR2 &= ~(I2C_CR2_FREQ_MASK << I2C_CR2_FREQ_POS);
    p_i2c_handle->p_i2cx->CR2 |= (rcc_get_bus_clock_freq_hz(APB1_BUS) / 1000000);

    // Set own address
    p_i2c_handle->p_i2cx->OAR1 &= ~(I2C_OAR1_ADD7_MASK << I2C_OAR1_ADD7_POS);
    p_i2c_handle->p_i2cx->OAR1 |= (p_i2c_handle->i2c_conf.own_address << I2C_OAR1_ADD7_POS);

    // Configure CCR
    set_ccr_and_trise(p_i2c_handle);

    // Enable peripheral
    p_i2c_handle->p_i2cx->CR1 |= I2C_CR1_PE;
}

static inline void assert_i2c_handle_param(i2c_handle const *const p_i2c_handle)
{
    // clang-format off
    uint8_t found_setting = false;

    // Peripheral check
    found_setting = (p_i2c_handle->p_i2cx == I2C1) ? true
                  : (p_i2c_handle->p_i2cx == I2C2) ? true
                  : (p_i2c_handle->p_i2cx == I2C3) ? true
                                                   : false;
    ASSERT(found_setting);

    // Fast mode duty cycle check
    found_setting = false;
    switch (p_i2c_handle->i2c_conf.fm_duty_cycle) {
    case I2C_FM_DUTY_2:    found_setting = true; break;
    case I2C_FM_DUTY_16_9: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Speed mode check
    found_setting = false;
    switch (p_i2c_handle->i2c_conf.speed_mode) {
    case I2C_SPEED_MODE_STANDARD: found_setting = true; break;
    case I2C_SPEED_MODE_FAST:     found_setting = true; break;
    }
    ASSERT(found_setting);

    // Clock frequency check
    found_setting = false;
    switch (p_i2c_handle->i2c_conf.clock_freq_hz) {
    case I2C_CLK_FREQ_100KHZ: found_setting = true; break;
    case I2C_CLK_FREQ_200KHZ: found_setting = true; break;
    case I2C_CLK_FREQ_300KHZ: found_setting = true; break;
    case I2C_CLK_FREQ_400KHZ: found_setting = true; break;
    }
    ASSERT(found_setting);
    // clang-format on
}

/***************************************************************************
Function: i2c_reset
Overview: disables the clock for the given i2c peripheral, resetting it.
Parameters:
    p_i2cx: I2C register peripheral to be reset.
Return: 
    None
Note: None
***************************************************************************/
void i2c_reset(i2c_reg_def const *const p_i2cx)
{
    rcc_i2c_togglable_clock(p_i2cx, DISABLE);
}

// NOTE: Might need to return success/fail incase the device never receives and ack.

/***************************************************************************
Function: i2c_master_transmit
Overview: takes the role of master and transmits data.
Parameters:
	p_i2cx: I2C peripheral used to communicate.
	target_addr: Address of device to communicate to.
	p_data: Pointer to data that will be sent. Can be dereferenced variable or list.
	length: Length of data being sent.
	repeated_start: Whether the transmission will end with a stop signal.
		I2C_REPEATED_START_ENABLE
		I2C_REPEATED_START_DISABLE
Return: 
    None
Note: None
***************************************************************************/
void i2c_master_transmit(i2c_reg_def *const p_i2cx, uint8_t const target_addr, uint8_t const *p_data,
                         uint32_t const length, i2c_repeated_start_e const repeated_start)
{
    ASSERT(get_i2c_init_status(p_i2cx) == I2C_INITIALIZED);

    // Ensure peripheral is not busy.
    while (p_i2cx->SR2 & I2C_SR2_BUSY);

    config_ack(p_i2cx, ENABLE);

    generate_start_condition(p_i2cx);
    while (!(p_i2cx->SR1 & I2C_SR1_SB));

    clear_start_flag(p_i2cx);

    // Wait for address to be sent.
    send_address(p_i2cx, target_addr, I2C_STATUS_MASTER_TX);
    while (!(p_i2cx->SR1 & I2C_SR1_ADDR));
    clear_address_flag(p_i2cx);

    // Send data
    for (uint32_t i = length; i > 0; i--)
    {
        while (!(p_i2cx->SR1 & I2C_SR1_TXE));
        p_i2cx->DR = *p_data;
        p_data++;
        // If last byte send stop.
        if (i == 1)
        {
            while (!(p_i2cx->SR1 & I2C_SR1_TXE));
            while (!(p_i2cx->SR1 & I2C_SR1_BTF));
            if (repeated_start == I2C_REPEATED_START_DISABLE)
            {
                generate_stop_condition(p_i2cx);
            }
        }
    }
}

/***************************************************************************
Function: i2c_master_receive
Overview: takes the role of master and receives data.
Parameters:
	p_i2cx: I2C peripheral used to communicate.
	target_addr: Address of device to communicate to.
	p_data: Pointer to data that will be recieved. Can be dereferenced variable or list.
	length: Length of data being sent.
	repeated_start: Whether the transmission will end with a stop signal.
		I2C_REPEATED_START_ENABLE
		I2C_REPEATED_START_DISABLE
Return: 
    None
Note: None
***************************************************************************/
void i2c_master_receive(i2c_reg_def *const p_i2cx, uint8_t const target_addr, uint8_t *p_data, uint32_t const length,
                        i2c_repeated_start_e const repeated_start)
{
    ASSERT(get_i2c_init_status(p_i2cx) == I2C_INITIALIZED);

    // Ensure peripheral is not busy.
    while (p_i2cx->SR2 & I2C_SR2_BUSY);

    if (length != 1)
    {
        config_ack(p_i2cx, ENABLE);
    }
    else
    {
        config_ack(p_i2cx, DISABLE);
    }

    generate_start_condition(p_i2cx);

    while (!(p_i2cx->SR1 & I2C_SR1_SB));
    clear_start_flag(p_i2cx);

    send_address(p_i2cx, target_addr, I2C_STATUS_MASTER_RX);
    while (!(p_i2cx->SR1 & I2C_SR1_ADDR));

    clear_address_flag(p_i2cx);

    for (uint32_t i = length; i > 0; i--)
    {
        if (i == 1)
        {
            config_ack(p_i2cx, DISABLE);
        }
        while (!(p_i2cx->SR1 & I2C_SR1_RXNE));
        *p_data = p_i2cx->DR;
        p_data++;

        if ((i == 1) && (repeated_start == I2C_REPEATED_START_DISABLE))
        {
            generate_stop_condition(p_i2cx);
        }
    }
}

/***************************************************************************
Function: i2c_slave_transmit
Overview: takes the role of slave and transmits data.
Parameters:
	p_i2cx: I2C peripheral used to communicate.
	target_addr: Address of device to communicate to.
	p_data: Pointer to data that will be transmitted. Can be dereferenced variable or list.
	length: Length of data being sent.
Return: 
    None
Note: None
***************************************************************************/
void i2c_slave_transmit(i2c_reg_def *const p_i2cx, uint8_t const *p_data, uint32_t const length)
{
    ASSERT(get_i2c_init_status(p_i2cx) == I2C_INITIALIZED);

    // Ensure peripheral is not busy.
    while (p_i2cx->SR2 & I2C_SR2_BUSY);

    config_ack(p_i2cx, ENABLE);

    // Wait for ADDR flag and clear it.
    while (!(p_i2cx->SR1 & I2C_SR1_ADDR));
    clear_address_flag(p_i2cx);

    // Send data
    for (uint32_t i = length; i > 0; i--)
    {
        while (!(p_i2cx->SR1 & I2C_SR1_TXE));
        p_i2cx->DR = *p_data;
        p_data++;
    }
    // Clear ack failure
    clear_af(p_i2cx);

    // Turn acking off
    config_ack(p_i2cx, DISABLE);
}

/***************************************************************************
Function: i2c_slave_receive
Overview: takes the role of slave and receives data.
Parameters:
	p_i2cx: I2C peripheral used to communicate.
	target_addr: Address of device to communicate to.
	p_data: Pointer to data that will be recieved. Can be dereferenced variable or list.
	length: Length of data being sent.
Return: 
    None
Note: None
***************************************************************************/
void i2c_slave_receive(i2c_reg_def *const p_i2cx, uint8_t *p_data, uint32_t const length)
{
    ASSERT(get_i2c_init_status(p_i2cx) == I2C_INITIALIZED);

    // Ensure peripheral is not busy.
    while (p_i2cx->SR2 & I2C_SR2_BUSY);

    config_ack(p_i2cx, ENABLE);

    // Wait for ADDR flag and clear it.
    while (!(p_i2cx->SR1 & I2C_SR1_ADDR));
    clear_address_flag(p_i2cx);

    // Receive data
    for (uint32_t i = length; i > 0; i--)
    {
        while (!(p_i2cx->SR1 & I2C_SR1_RXNE));
        *p_data = p_i2cx->DR;
        p_data++;
    }

    // Wait until STOPF flag is set and clear it.
    while (!(p_i2cx->SR1 & I2C_SR1_STOPF));
    clear_stopf(p_i2cx);

    // Turn acking off.
    config_ack(p_i2cx, DISABLE);
}

/***************************************************************************
Function: i2c_master_transmit_it
Overview: takes the role of master and transmits data using interrupts.
Parameters:
    p_i2c_handle: Structure with the settings and data for it handling.
	target_addr: Address of device to communicate to.
	p_data: Pointer to data that will be sent. Can be dereferenced variable or list.
	length: Length of data being sent.
	repeated_start: Whether the transmission will end with a stop signal.
		I2C_REPEATED_START_ENABLE
		I2C_REPEATED_START_DISABLE
Return: 
    None
Note: None
***************************************************************************/
void i2c_master_transmit_it(i2c_handle *p_i2c_handle, uint8_t const target_addr, uint8_t *const p_data,
                            uint32_t const length, i2c_repeated_start_e const repeated_start)
{
    ASSERT(get_i2c_init_status(p_i2c_handle->p_i2cx) == I2C_INITIALIZED);

    // Wait until i2c is available (blocking).
    while (!(p_i2c_handle->i2c_it_data.status == I2C_STATUS_READY));
    p_i2c_handle->i2c_it_data.status = I2C_STATUS_MASTER_TX;

    // Set corresponding handle variables.
    p_i2c_handle->i2c_it_data.txrx_buffer    = p_data;
    p_i2c_handle->i2c_it_data.txrx_length    = length;
    p_i2c_handle->i2c_it_data.target_addr    = target_addr;
    p_i2c_handle->i2c_it_data.repeated_start = repeated_start;

    generate_start_condition(p_i2c_handle->p_i2cx);

    i2c_config_it(p_i2c_handle->p_i2cx, ENABLE);
}

/***************************************************************************
Function: i2c_master_recieve_it
Overview: takes the role of master and receives data using interrupts.
Parameters:
    p_i2c_handle: Structure with the settings and data for it handling.
	target_addr: Address of device to communicate to.
	p_data: Pointer to data that will be received. Can be dereferenced variable or list.
	length: Expected length of data being received.
	repeated_start: Whether the transmission will end with a stop signal.
		I2C_REPEATED_START_ENABLE
		I2C_REPEATED_START_DISABLE
Return: 
    None
Note: None
 ***************************************************************************/
void i2c_master_receive_it(i2c_handle *p_i2c_handle, uint8_t const target_addr, uint8_t *const p_data,
                           uint32_t const length, i2c_repeated_start_e const repeated_start)
{
    ASSERT(get_i2c_init_status(p_i2c_handle->p_i2cx) == I2C_INITIALIZED);

    // Wait until i2c is available (blocking).
    while (!(p_i2c_handle->i2c_it_data.status == I2C_STATUS_READY));
    p_i2c_handle->i2c_it_data.status = I2C_STATUS_MASTER_RX;

    // Set corresponding handle variables.
    p_i2c_handle->i2c_it_data.txrx_buffer    = p_data;
    p_i2c_handle->i2c_it_data.txrx_length    = length;
    p_i2c_handle->i2c_it_data.target_addr    = target_addr;
    p_i2c_handle->i2c_it_data.repeated_start = repeated_start;

    generate_start_condition(p_i2c_handle->p_i2cx);

    config_ack(p_i2c_handle->p_i2cx, ENABLE);

    i2c_config_it(p_i2c_handle->p_i2cx, ENABLE);
}

void i2c_slave_transmit_it(i2c_handle *const p_i2c_handle, uint8_t *const p_data, uint32_t const length)
{
    ASSERT(get_i2c_init_status(p_i2c_handle->p_i2cx) == I2C_INITIALIZED);

    // Wait until i2c is available (blocking).
    while (!(p_i2c_handle->i2c_it_data.status == I2C_STATUS_READY));
    p_i2c_handle->i2c_it_data.status = I2C_STATUS_SLAVE_TX;

    // Set corresponding handle variables.
    p_i2c_handle->i2c_it_data.txrx_buffer = p_data;
    p_i2c_handle->i2c_it_data.txrx_length = length;

    config_ack(p_i2c_handle->p_i2cx, ENABLE);

    i2c_config_it(p_i2c_handle->p_i2cx, ENABLE);
}

void i2c_slave_receive_it(i2c_handle *const p_i2c_handle, uint8_t *const p_data, uint32_t const length)
{
    ASSERT(get_i2c_init_status(p_i2c_handle->p_i2cx) == I2C_INITIALIZED);

    // Wait until i2c is available (blocking).
    while (!(p_i2c_handle->i2c_it_data.status == I2C_STATUS_READY));
    p_i2c_handle->i2c_it_data.status = I2C_STATUS_SLAVE_RX;

    // Set corresponding handle variables.
    p_i2c_handle->i2c_it_data.txrx_buffer = p_data;
    p_i2c_handle->i2c_it_data.txrx_length = length;

    config_ack(p_i2c_handle->p_i2cx, ENABLE);

    i2c_config_it(p_i2c_handle->p_i2cx, ENABLE);
}

/***************************************************************************
Function: i2c_config_it
Overview: Controls interrupts for I2C peripheral.
Parameters:
	p_i2cx: I2C peripheral to be modified.
    toggle: 
        ENABLE (1)
        DISABLE (0)
Return: 
    None
Note: None
 ***************************************************************************/
void i2c_config_it(i2c_reg_def *const p_i2cx, togglable_e const toggle)
{
    uint32_t temp_cr2 = p_i2cx->CR2;

    switch (toggle)
    {
        case ENABLE:
            temp_cr2 |= I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN;
            p_i2cx->CR2 = temp_cr2;
            break;
        case DISABLE:
            temp_cr2 &= ~(I2C_CR2_ITBUFEN | I2C_CR2_ITERREN | I2C_CR2_ITEVTEN);
            p_i2cx->CR2 = temp_cr2;
            break;
    }

    if (p_i2cx == I2C1)
    {
        nvic_irq_config(I2C1_EV_IRQ_NUM_31, toggle);
    }
    else if (p_i2cx == I2C2)
    {
        nvic_irq_config(I2C2_EV_IRQ_NUM_33, toggle);
    }
    else if (p_i2cx == I2C3)
    {
        nvic_irq_config(I2C3_EV_IRQ_NUM_72, toggle);
    }
}

/***************************************************************************
Function: i2c_it_handler
Overview: Handles interrupts for I2C peripheral.
Parameters:
	p_i2cx: I2C peripheral to be modified.
    toggle: 
        ENABLE (1)
        DISABLE (0)
Return: 
    None
Note: Should be called from the ISR function.
 ***************************************************************************/
void i2c_it_handler(i2c_handle *const p_i2c_handle)
{
    uint32_t volatile dummy_read = 0;

    switch (p_i2c_handle->i2c_it_data.status)
    {
        case I2C_STATUS_MASTER_TX:
        case I2C_STATUS_MASTER_RX:
            if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_SB)
            {
                master_sb(p_i2c_handle);
            }

            if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_ADDR)
            {
                master_addr(p_i2c_handle);
            }

            switch (p_i2c_handle->i2c_it_data.status)
            {
                case I2C_STATUS_MASTER_TX:
                    if ((p_i2c_handle->i2c_it_data.txrx_length == 0) && (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_BTF)
                        && (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_TXE))
                    {
                        close_com(p_i2c_handle);
                        i2c_callback(p_i2c_handle, I2C_EVENT_CMPLT);
                    }

                    else if ((p_i2c_handle->p_i2cx->SR1 & I2C_SR1_TXE) && (p_i2c_handle->i2c_it_data.txrx_length != 0))
                    {
                        send_byte(p_i2c_handle);
                    }
                    break;

                case I2C_STATUS_MASTER_RX:
                    if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_RXNE)
                    {
                        if (p_i2c_handle->i2c_it_data.txrx_length == 2)
                        {
                            config_ack(p_i2c_handle->p_i2cx, DISABLE);
                        }
                        receive_byte(p_i2c_handle);

                        if (p_i2c_handle->i2c_it_data.txrx_length == 0)
                        {
                            close_com(p_i2c_handle);
                            i2c_callback(p_i2c_handle, I2C_EVENT_CMPLT);
                        }
                    }
                    break;

                default:
                    ASSERT(false);
                    break;
            }
            break;

        case I2C_STATUS_SLAVE_TX:
        case I2C_STATUS_SLAVE_RX:

            if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_ADDR)
            {
                slave_addr(p_i2c_handle);
            }

            switch (p_i2c_handle->i2c_it_data.status)
            {
                case I2C_STATUS_SLAVE_TX:

                    if ((p_i2c_handle->p_i2cx->SR1 & I2C_SR1_TXE) && (p_i2c_handle->i2c_it_data.txrx_length != 0))
                    {
                        send_byte(p_i2c_handle);
                    }

                    if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_AF)
                    {
                        clear_af(p_i2c_handle->p_i2cx);
                        close_com(p_i2c_handle);
                    }

                    break;
                case I2C_STATUS_SLAVE_RX:
                    if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_RXNE)
                    {
                        receive_byte(p_i2c_handle);
                    }

                    if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_STOPF)
                    {
                        clear_stopf(p_i2c_handle->p_i2cx);
                        close_com(p_i2c_handle);
                    }
                    break;
                default:
                    ASSERT(false);
                    break;
            }
            break;

        default:
            ASSERT(false);
            break;
    }
    UNUSED(dummy_read);
}

//======================================================================================//}
//                  Private Function Implementation
//======================================================================================//{

static inline i2c_init_port_num_e map_i2c_ports_to_num(i2c_reg_def const *const p_i2cx)
{
    if (p_i2cx == I2C1)
    {
        return I2C1_INIT_NUM;
    }
    else if (p_i2cx == I2C2)
    {
        return I2C2_INIT_NUM;
    }
    else if (p_i2cx == I2C3)
    {
        return I2C3_INIT_NUM;
    }
    else
    {
        ASSERT(0);
    }
    return 0;
}

static inline void set_i2c_init_status(i2c_reg_def const *const p_i2cx)
{

    g_i2c_port_init |= (1 << (map_i2c_ports_to_num(p_i2cx)));
}

static inline i2c_init_check_e get_i2c_init_status(i2c_reg_def const *const p_i2cx)
{
    return 0b1 & (g_i2c_port_init >> (map_i2c_ports_to_num(p_i2cx)));
}

static void set_ccr_and_trise(i2c_handle const *const p_i2c_handle)
{
    uint32_t const I2C_MAX_RISE_SM_1000_NS = 1000;
    uint32_t const I2C_MAX_RISE_FM_300_NS  = 300;

    uint32_t ccr_value            = 0;
    uint32_t trise_value          = 0;
    uint32_t desired_clock_khz    = p_i2c_handle->i2c_conf.clock_freq_hz / 1000;
    uint32_t peripheral_clock_khz = (rcc_get_bus_clock_freq_hz(APB1_BUS) / 1000);

    // Speed configuration
    p_i2c_handle->p_i2cx->CCR |= (p_i2c_handle->i2c_conf.speed_mode << I2C_CCR_F_S_POS);

    switch (p_i2c_handle->i2c_conf.speed_mode)
    {
        case I2C_SPEED_MODE_STANDARD:
            // Peripheral clock speed must be at leats 2Mhz in standard mode.
            if (peripheral_clock_khz < 2000)
            {
                ASSERT(false);
            }

            // Set CCR
            ccr_value = ((peripheral_clock_khz) / (2 * (desired_clock_khz)));

            // Set time rise value
            trise_value |= ((I2C_MAX_RISE_SM_1000_NS / (1000000 / (peripheral_clock_khz))) + 1);
            break;

        case I2C_SPEED_MODE_FAST:
            // Peripheral clock speed must be at leats 4Mhz in fast mode.
            if (peripheral_clock_khz < 4000)
            {
                ASSERT(false);
            }

            // Set duty cycle
            p_i2c_handle->p_i2cx->CCR |= (p_i2c_handle->i2c_conf.fm_duty_cycle << I2C_CCR_DUTY_POS);

            // Set CCR
            switch (p_i2c_handle->i2c_conf.fm_duty_cycle)
            {
                case I2C_FM_DUTY_2:
                    ccr_value = ((peripheral_clock_khz) / (3 * (desired_clock_khz)));
                    break;
                case I2C_FM_DUTY_16_9:
                    ccr_value = ((peripheral_clock_khz) / (25 * (desired_clock_khz)));
                    break;
            }
            break;

            // Set TRISE
            trise_value |= ((I2C_MAX_RISE_FM_300_NS / (1000000 / (peripheral_clock_khz))) + 1);
    }
    p_i2c_handle->p_i2cx->TRISE &= ~(I2C_TRISE_TRISE_MASK << I2C_TRISE_TRISE_POS);
    p_i2c_handle->p_i2cx->TRISE = (trise_value & I2C_TRISE_TRISE_MASK);

    p_i2c_handle->p_i2cx->CCR &= ~(I2C_CCR_CCR_MASK << I2C_CCR_CCR_POS);
    p_i2c_handle->p_i2cx->CCR |= ccr_value;
}

static inline void generate_start_condition(i2c_reg_def *const p_i2cx)
{
    // Sends start and waits until SB is set.
    p_i2cx->CR1 |= I2C_CR1_START;
}

static inline void generate_stop_condition(i2c_reg_def *const p_i2cx)
{
    p_i2cx->CR1 |= I2C_CR1_STOP;
}
static inline void clear_start_flag(i2c_reg_def const *const p_i2cx)
{
    uint32_t volatile dummy_read;
    dummy_read = p_i2cx->SR1;
    UNUSED(dummy_read);
}

static inline void send_address(i2c_reg_def *const p_i2cx, uint8_t const target_addr, i2c_status_e const i2c_status)
{
    // Clears SB and writes address.
    switch (i2c_status)
    {
        case I2C_STATUS_MASTER_TX:
        case I2C_STATUS_SLAVE_TX:
            p_i2cx->DR = ((target_addr << 1) & ~I2C_READ_WRITE_BIT);
            break;
        case I2C_STATUS_MASTER_RX:
        case I2C_STATUS_SLAVE_RX:
            p_i2cx->DR = ((target_addr << 1) | I2C_READ_WRITE_BIT);
            break;

        default:
            ASSERT(false);
    }
}

static inline void clear_address_flag(i2c_reg_def const *const p_i2cx)
{
    uint32_t volatile dummy_read;
    dummy_read = p_i2cx->SR1;
    dummy_read = p_i2cx->SR2;
    UNUSED(dummy_read);
}

static inline void config_ack(i2c_reg_def *const p_i2cx, togglable_e toggle)
{
    switch (toggle)
    {
        case ENABLE:
            p_i2cx->CR1 |= I2C_CR1_ACK;
            break;
        case DISABLE:
            p_i2cx->CR1 &= ~I2C_CR1_ACK;
            break;
    }
}

static inline void send_byte(i2c_handle *const p_i2c_handle)
{
    p_i2c_handle->p_i2cx->DR = *p_i2c_handle->i2c_it_data.txrx_buffer;
    p_i2c_handle->i2c_it_data.txrx_buffer++;
    p_i2c_handle->i2c_it_data.txrx_length--;
}

static inline void receive_byte(i2c_handle *const p_i2c_handle)
{
    *p_i2c_handle->i2c_it_data.txrx_buffer = p_i2c_handle->p_i2cx->DR;
    p_i2c_handle->i2c_it_data.txrx_buffer++;
    p_i2c_handle->i2c_it_data.txrx_length--;
}

static inline void close_com(i2c_handle *const p_i2c_handle)
{
    if (p_i2c_handle->i2c_it_data.repeated_start == I2C_REPEATED_START_DISABLE)
    {
        generate_stop_condition(p_i2c_handle->p_i2cx);
    }
    i2c_config_it(p_i2c_handle->p_i2cx, DISABLE);
    switch (p_i2c_handle->i2c_it_data.status)
    {
        case I2C_STATUS_SLAVE_RX:
        case I2C_STATUS_SLAVE_TX:
            config_ack(p_i2c_handle->p_i2cx, DISABLE);
            break;
        default:;
    }
    p_i2c_handle->i2c_it_data.status      = I2C_STATUS_READY;
    p_i2c_handle->i2c_it_data.txrx_length = 0;
    p_i2c_handle->i2c_it_data.txrx_buffer = 0;
    p_i2c_handle->i2c_it_data.target_addr = 0;
}

static inline void clear_stopf(i2c_reg_def *const p_i2cx)
{
    // cppcheck-suppress-begin redundantAssignment
    uint32_t dummy_action;
    dummy_action = p_i2cx->SR1;
    dummy_action = 0;
    p_i2cx->CR1 |= dummy_action;
    // cppcheck-suppress-end redundantAssignment
}

static inline void clear_af(i2c_reg_def *const p_i2cx)
{
    p_i2cx->SR1 &= ~I2C_SR1_AF;
}

static void master_sb(i2c_handle *const p_i2c_handle)
{
    clear_start_flag(p_i2c_handle->p_i2cx);
    send_address(p_i2c_handle->p_i2cx, p_i2c_handle->i2c_it_data.target_addr, p_i2c_handle->i2c_it_data.status);
}

static void master_addr(i2c_handle *const p_i2c_handle)
{
    // Disable ack before clearing address flag in 1 byte reception.
    if ((p_i2c_handle->i2c_it_data.status == I2C_STATUS_MASTER_RX) && (p_i2c_handle->i2c_it_data.txrx_length == 1))
    {
        config_ack(p_i2c_handle->p_i2cx, DISABLE);
    }
    clear_address_flag(p_i2c_handle->p_i2cx);
}
static void slave_addr(i2c_handle const *const p_i2c_handle)
{
    clear_address_flag(p_i2c_handle->p_i2cx);
}

// *End of file*
