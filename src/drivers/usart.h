#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"

/****************************************************************************************************
                                    Address Definitions
****************************************************************************************************/

typedef enum {
    USART1_BASE_ADDR = ((APB2_BASE_ADDR) + (0x1000u)),
    USART6_BASE_ADDR = ((APB2_BASE_ADDR) + (0x1400u)),
    USART2_BASE_ADDR = ((APB1_BASE_ADDR) + (0x4400u)),
    USART3_BASE_ADDR = ((APB1_BASE_ADDR) + (0x4400u)),
    UART4_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x4400u)),
    UART5_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x4400u)),
} usart_base_addr_e;

#ifdef MACRO
#define USART1_BASE_ADDR ((APB2_BASE_ADDR) + (0x1000u))
#define USART6_BASE_ADDR ((APB2_BASE_ADDR) + (0x1400u))
#define USART2_BASE_ADDR ((APB1_BASE_ADDR) + (0x4400u))
#define USART3_BASE_ADDR ((APB1_BASE_ADDR) + (0x4400u))
#define UART4_BASE_ADDR  ((APB1_BASE_ADDR) + (0x4400u))
#define UART5_BASE_ADDR  ((APB1_BASE_ADDR) + (0x4400u))
#endif

/****************************************************************************************************
                                    Macros and Other Enums
****************************************************************************************************/
// Init check enum
typedef enum {
    USART1_INIT_NUM = 0,
    USART2_INIT_NUM = 1,
    USART3_INIT_NUM = 2,
    UART4_INIT_NUM  = 3,
    UART5_INIT_NUM  = 4,
    USART6_INIT_NUM = 5,
} usart_init_port_num_e;

// Register bits
typedef enum {
    USART_SR_PE_POS   = 0,
    USART_SR_FE_POS   = 1,
    USART_SR_NF_POS   = 2,
    USART_SR_ORE_POS  = 3,
    USART_SR_IDLE_POS = 4,
    USART_SR_RXNE_POS = 5,
    USART_SR_TC_POS   = 6,
    USART_SR_TXE_POS  = 7,
    USART_SR_LBD_POS  = 8,
    USART_SR_CTS_POS  = 9,
} usart_sr_pos_e;

typedef enum {
    USART_SR_PE   = (1 << USART_SR_PE_POS),
    USART_SR_FE   = (1 << USART_SR_FE_POS),
    USART_SR_NF   = (1 << USART_SR_NF_POS),
    USART_SR_ORE  = (1 << USART_SR_ORE_POS),
    USART_SR_IDLE = (1 << USART_SR_IDLE_POS),
    USART_SR_RXNE = (1 << USART_SR_RXNE_POS),
    USART_SR_TC   = (1 << USART_SR_TC_POS),
    USART_SR_TXE  = (1 << USART_SR_TXE_POS),
    USART_SR_LBD  = (1 << USART_SR_LBD_POS),
    USART_SR_CTS  = (1 << USART_SR_CTS_POS),
} usart_sr_mask_e;

typedef enum {
    USART_CR1_SBK_POS    = 0,
    USART_CR1_RWU_POS    = 1,
    USART_CR1_RE_POS     = 2,
    USART_CR1_TE_POS     = 3,
    USART_CR1_IDLEIE_POS = 4,
    USART_CR1_RXNEIE_POS = 5,
    USART_CR1_TCIE_POS   = 6,
    USART_CR1_TXEIE_POS  = 7,
    USART_CR1_PEIE_POS   = 8,
    USART_CR1_PS_POS     = 9,
    USART_CR1_PCE_POS    = 10,
    USART_CR1_WAKE_POS   = 11,
    USART_CR1_M_POS      = 12,
    USART_CR1_UE_POS     = 13,
    USART_CR1_OVER8_POS  = 15,
} usart_cr1_pos_e;

typedef enum {
    USART_CR1_SBK    = (1 << USART_CR1_SBK_POS),
    USART_CR1_RWU    = (1 << USART_CR1_RWU_POS),
    USART_CR1_RE     = (1 << USART_CR1_RE_POS),
    USART_CR1_TE     = (1 << USART_CR1_TE_POS),
    USART_CR1_IDLEIE = (1 << USART_CR1_IDLEIE_POS),
    USART_CR1_RXNEIE = (1 << USART_CR1_RXNEIE_POS),
    USART_CR1_TCIE   = (1 << USART_CR1_TCIE_POS),
    USART_CR1_TXEIE  = (1 << USART_CR1_TXEIE_POS),
    USART_CR1_PEIE   = (1 << USART_CR1_PEIE_POS),
    USART_CR1_PS     = (1 << USART_CR1_PS_POS),
    USART_CR1_PCE    = (1 << USART_CR1_PCE_POS),
    USART_CR1_WAKE   = (1 << USART_CR1_WAKE_POS),
    USART_CR1_M      = (1 << USART_CR1_M_POS),
    USART_CR1_UE     = (1 << USART_CR1_UE_POS),
    USART_CR1_OVER8  = (1 << USART_CR1_OVER8_POS),
} usart_cr1_e;

typedef enum {
    USART_CR2_ADD_POS   = 0,
    USART_CR2_LBDL_POS  = 5,
    USART_CR2_LBDIE_POS = 6,
    USART_CR2_LBCL_POS  = 8,
    USART_CR2_CPHA_POS  = 9,
    USART_CR2_CPOL_POS  = 10,
    USART_CR2_CLKEN_POS = 11,
    USART_CR2_STOP_POS  = 12,
    USART_CR2_LINEN_POS = 14,
} usart_cr2_pos_e;

typedef enum {
    USART_CR2_ADD   = (1 << USART_CR2_ADD_POS),
    USART_CR2_LBDL  = (1 << USART_CR2_LBDL_POS),
    USART_CR2_LBDIE = (1 << USART_CR2_LBDIE_POS),
    USART_CR2_LBCL  = (1 << USART_CR2_LBCL_POS),
    USART_CR2_CPHA  = (1 << USART_CR2_CPHA_POS),
    USART_CR2_CPOL  = (1 << USART_CR2_CPOL_POS),
    USART_CR2_CLKEN = (1 << USART_CR2_CLKEN_POS),
    USART_CR2_STOP  = (1 << USART_CR2_STOP_POS),
    USART_CR2_LINEN = (1 << USART_CR2_LINEN_POS),
} usart_cr2_e;

typedef enum {
    USART_BRR_FRACTION_POS      = 0,
    USART_BRR_MANTISSA_POS      = 4,
    USART_BRR_OVER8EN_CLEAR_POS = 3,
} usart_brr_e;

// Configuration settings
typedef enum {
    USART_OVERSAMPLING_16 = 0, // Default
    USART_OVERSAMPLING_8  = 1,
} usart_oversampling_e;

typedef enum {
    USART_WORD_LENGTH_8_DATA_BITS = 0, // Default
    USART_WORD_LENGTH_9_DATA_BITS = 1,
} usart_word_length_e;

typedef enum {
    USART_PARITY_CONTROL_DISABLE = 0, // Default
    USART_PARITY_CONTROL_ENABLE  = 1,
} usart_parity_control_e;

typedef enum {
    USART_PARITY_SEL_EVEN = 0, // Default
    USART_PARITY_SEL_ODD  = 1,
} usart_parity_select_e;

typedef enum {
    USART_STOP_BITS_1   = 0, // Default
    USART_STOP_BITS_0_5 = 1,
    USART_STOP_BITS_2   = 2,
    USART_STOP_BITS_1_5 = 3,
} usart_stop_bits_e;

typedef enum {
    USART_BAUDRATE_1200   = 1200,
    USART_BAUDRATE_2400   = 2400,
    USART_BAUDRATE_9600   = 9600,
    USART_BAUDRATE_19200  = 19200,
    USART_BAUDRATE_38400  = 38400,
    USART_BAUDRATE_57600  = 57600,
    USART_BAUDRATE_115200 = 115200,
    USART_BAUDRATE_230400 = 230400,
    USART_BAUDRATE_460800 = 460800,
    USART_BAUDRATE_921600 = 921600,
    USART_BAUDRATE_2MB    = 2000000,
    USART_BAUDRATE_3MB    = 3000000,
} usart_baudrate_e;

typedef enum {
    USART_MODE_NONE,
    USART_MODE_TX,
    USART_MODE_RX,
} usart_mode_e;

typedef enum {
    USART_STATUS_READY = 0, // Default
    USART_STATUS_RUNNING,
} usart_status_e;

/****************************************************************************************************
                                    Structure Definitions
****************************************************************************************************/

typedef __vo struct {
    uint32_t SR;   // Status register                      Offset: 0x00
    uint32_t DR;   // Data register                        Offset: 0x04
    uint32_t BRR;  // Baud rate register                   Offset: 0x08
    uint32_t CR1;  // Control register 1                   Offset: 0x0C
    uint32_t CR2;  // Control register 2                   Offset: 0x10
    uint32_t CR3;  // Control register 3                   Offset: 0x14
    uint32_t GTPR; // Guard time and prescaler register    Offset: 0x18
} usart_reg_def;

typedef struct {
    usart_oversampling_e oversampling_mode;
    usart_word_length_e word_length;
    usart_parity_control_e parity_control;
    usart_parity_select_e parity_select;
    usart_stop_bits_e stop_bits;
    usart_baudrate_e baudrate;
    togglable_e it_config;
} usart_config;

typedef struct {
    usart_reg_def *p_usartx;
    usart_config usart_conf;
    uint8_t *rx_buffer;
    uint8_t *tx_buffer;
    uint32_t txrx_length;
    usart_mode_e mode;
    __vo usart_status_e status;
} usart_handle;

/****************************************************************************************************
                                    Peripheral Structure Definitions
****************************************************************************************************/

#define USART1 ((usart_reg_def *)USART1_BASE_ADDR)
#define USART2 ((usart_reg_def *)USART2_BASE_ADDR)
#define USART3 ((usart_reg_def *)USART3_BASE_ADDR)
#define UART4  ((usart_reg_def *)UART4_BASE_ADDR)
#define UART5  ((usart_reg_def *)UART5_BASE_ADDR)
#define USART6 ((usart_reg_def *)USART6_BASE_ADDR)

/****************************************************************************************************
                                    Peripheral Function APIs
****************************************************************************************************/
// Init and reset
void usart_init(usart_handle *const p_usart_handle);
void usart_reset(usart_reg_def const *const p_usartx);

// Write/read blocking or polling
void usart_write(usart_reg_def *const p_usartx, uint8_t const *p_data, uint32_t length);
void usart_read(usart_reg_def *const p_usartx, uint8_t *p_data, uint32_t length);

// Write/read interrupt
void usart_write_it(usart_handle *p_usart_handle, uint8_t *p_data, uint32_t const length);
void usart_read_it(usart_handle *p_usart_handle, uint8_t *p_data, uint32_t const length);

// Interrupt handling
void usart_it_config(usart_reg_def const *const p_usartx, togglable_e toggle);
void usart_it_handler(usart_handle *const p_usart_handle);

#endif