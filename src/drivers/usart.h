#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"

//======================================================================================//
//                  Address Definitions
//======================================================================================//

typedef enum {
    USART1_BASE_ADDR = ((APB2_BASE_ADDR) + (0x1000u)),
    USART6_BASE_ADDR = ((APB2_BASE_ADDR) + (0x1400u)),
    USART2_BASE_ADDR = ((APB1_BASE_ADDR) + (0x4400u)),
    USART3_BASE_ADDR = ((APB1_BASE_ADDR) + (0x4400u)),
    UART4_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x4400u)),
    UART5_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x4400u)),
} usart_base_addr_e;

//======================================================================================//
//                  Peripheral Constants
//======================================================================================//

// Used for callbacks
typedef enum {
    USART_EVENT_CMPLT,
    USART_EVENT_ERROR,
} usart_event_e;

// Init check enum
typedef enum {
    USART1_INIT_NUM = 0,
    USART2_INIT_NUM = 1,
    USART3_INIT_NUM = 2,
    UART4_INIT_NUM  = 3,
    UART5_INIT_NUM  = 4,
    USART6_INIT_NUM = 5,
} usart_init_port_num_e;

typedef enum {
    USART_NOT_INITIALIZED = 0,
    USART_INITIALIZED     = 0,
} usart_init_check_e;

//======================================================================================//
//                  Register Constants
//======================================================================================//

typedef enum {
    USART_BRR_OVER8EN_CLEAR_POS = 3,
} usart_reg_misc_e;

typedef enum {
    USART_SR_CTS_POS  = 9, // CTS flag
    USART_SR_LBD_POS  = 8, // LIN break detection flag
    USART_SR_TXE_POS  = 7, // Transmit data register empty
    USART_SR_TC_POS   = 6, // Transmission complete
    USART_SR_RXNE_POS = 5, // Read data register not empty
    USART_SR_IDLE_POS = 4, // IDLE line detected
    USART_SR_ORE_POS  = 3, // Overrun error
    USART_SR_NF_POS   = 2, // Noise detected flag
    USART_SR_FE_POS   = 1, // Framing error
    USART_SR_PE_POS   = 0, // Parity error
} usart_sr_pos_e;

typedef enum {
    USART_SR_CTS  = (1 << USART_SR_CTS_POS),  // CTS flag
    USART_SR_LBD  = (1 << USART_SR_LBD_POS),  // LIN break detection flag
    USART_SR_TXE  = (1 << USART_SR_TXE_POS),  // Transmit data register empty
    USART_SR_TC   = (1 << USART_SR_TC_POS),   // Transmission complete
    USART_SR_RXNE = (1 << USART_SR_RXNE_POS), // Read data register not empty
    USART_SR_IDLE = (1 << USART_SR_IDLE_POS), // IDLE line detected
    USART_SR_ORE  = (1 << USART_SR_ORE_POS),  // Overrun error
    USART_SR_NF   = (1 << USART_SR_NF_POS),   // Noise detected flag
    USART_SR_FE   = (1 << USART_SR_FE_POS),   // Framing error
    USART_SR_PE   = (1 << USART_SR_PE_POS),   // Parity error
} usart_sr_e;

typedef enum {
    USART_SR_CTS_MASK  = 0b1, // 1 bit(s)
    USART_SR_LBD_MASK  = 0b1, // 1 bit(s)
    USART_SR_TXE_MASK  = 0b1, // 1 bit(s)
    USART_SR_TC_MASK   = 0b1, // 1 bit(s)
    USART_SR_RXNE_MASK = 0b1, // 1 bit(s)
    USART_SR_IDLE_MASK = 0b1, // 1 bit(s)
    USART_SR_ORE_MASK  = 0b1, // 1 bit(s)
    USART_SR_NF_MASK   = 0b1, // 1 bit(s)
    USART_SR_FE_MASK   = 0b1, // 1 bit(s)
    USART_SR_PE_MASK   = 0b1, // 1 bit(s)
} usart_sr_mask_e;

typedef enum {
    USART_DR_DR_POS = 0, // Data value
} usart_dr_pos_e;

typedef enum {
    USART_DR_DR = (1 << USART_DR_DR_POS), // Data value
} usart_dr_e;

typedef enum {
    USART_DR_DR_MASK = 0b111111111, // 9 bit(s)
} usart_dr_mask_e;

typedef enum {
    USART_BRR_DIV_MANTISSA_POS = 4, // mantissa of USARTDIV
    USART_BRR_DIV_FRACTION_POS = 0, // fraction of USARTDIV
} usart_brr_pos_e;

typedef enum {
    USART_BRR_DIV_MANTISSA = (1 << USART_BRR_DIV_MANTISSA_POS), // mantissa of USARTDIV
    USART_BRR_DIV_FRACTION = (1 << USART_BRR_DIV_FRACTION_POS), // fraction of USARTDIV
} usart_brr_e;

typedef enum {
    USART_BRR_DIV_MANTISSA_MASK = 0b111111111111, // 12 bit(s)
    USART_BRR_DIV_FRACTION_MASK = 0b1111,         // 4 bit(s)
} usart_brr_mask_e;

typedef enum {
    USART_CR1_OVER8_POS  = 15, // Oversampling mode
    USART_CR1_UE_POS     = 13, // USART enable
    USART_CR1_M_POS      = 12, // Word length
    USART_CR1_WAKE_POS   = 11, // Wakeup method
    USART_CR1_PCE_POS    = 10, // Parity control enable
    USART_CR1_PS_POS     = 9,  // Parity selection
    USART_CR1_PEIE_POS   = 8,  // PE interrupt enable
    USART_CR1_TXEIE_POS  = 7,  // TXE interrupt enable
    USART_CR1_TCIE_POS   = 6,  // Transmission complete interrupt enable
    USART_CR1_RXNEIE_POS = 5,  // RXNE interrupt enable
    USART_CR1_IDLEIE_POS = 4,  // IDLE interrupt enable
    USART_CR1_TE_POS     = 3,  // Transmitter enable
    USART_CR1_RE_POS     = 2,  // Receiver enable
    USART_CR1_RWU_POS    = 1,  // Receiver wakeup
    USART_CR1_SBK_POS    = 0,  // Send break
} usart_cr1_pos_e;

typedef enum {
    USART_CR1_OVER8  = (1 << USART_CR1_OVER8_POS),  // Oversampling mode
    USART_CR1_UE     = (1 << USART_CR1_UE_POS),     // USART enable
    USART_CR1_M      = (1 << USART_CR1_M_POS),      // Word length
    USART_CR1_WAKE   = (1 << USART_CR1_WAKE_POS),   // Wakeup method
    USART_CR1_PCE    = (1 << USART_CR1_PCE_POS),    // Parity control enable
    USART_CR1_PS     = (1 << USART_CR1_PS_POS),     // Parity selection
    USART_CR1_PEIE   = (1 << USART_CR1_PEIE_POS),   // PE interrupt enable
    USART_CR1_TXEIE  = (1 << USART_CR1_TXEIE_POS),  // TXE interrupt enable
    USART_CR1_TCIE   = (1 << USART_CR1_TCIE_POS),   // Transmission complete interrupt enable
    USART_CR1_RXNEIE = (1 << USART_CR1_RXNEIE_POS), // RXNE interrupt enable
    USART_CR1_IDLEIE = (1 << USART_CR1_IDLEIE_POS), // IDLE interrupt enable
    USART_CR1_TE     = (1 << USART_CR1_TE_POS),     // Transmitter enable
    USART_CR1_RE     = (1 << USART_CR1_RE_POS),     // Receiver enable
    USART_CR1_RWU    = (1 << USART_CR1_RWU_POS),    // Receiver wakeup
    USART_CR1_SBK    = (1 << USART_CR1_SBK_POS),    // Send break
} usart_cr1_e;

typedef enum {
    USART_CR1_OVER8_MASK  = 0b1, // 1 bit(s)
    USART_CR1_UE_MASK     = 0b1, // 1 bit(s)
    USART_CR1_M_MASK      = 0b1, // 1 bit(s)
    USART_CR1_WAKE_MASK   = 0b1, // 1 bit(s)
    USART_CR1_PCE_MASK    = 0b1, // 1 bit(s)
    USART_CR1_PS_MASK     = 0b1, // 1 bit(s)
    USART_CR1_PEIE_MASK   = 0b1, // 1 bit(s)
    USART_CR1_TXEIE_MASK  = 0b1, // 1 bit(s)
    USART_CR1_TCIE_MASK   = 0b1, // 1 bit(s)
    USART_CR1_RXNEIE_MASK = 0b1, // 1 bit(s)
    USART_CR1_IDLEIE_MASK = 0b1, // 1 bit(s)
    USART_CR1_TE_MASK     = 0b1, // 1 bit(s)
    USART_CR1_RE_MASK     = 0b1, // 1 bit(s)
    USART_CR1_RWU_MASK    = 0b1, // 1 bit(s)
    USART_CR1_SBK_MASK    = 0b1, // 1 bit(s)
} usart_cr1_mask_e;

typedef enum {
    USART_CR2_LINEN_POS = 14, // LIN mode enable
    USART_CR2_STOP_POS  = 12, // STOP bits
    USART_CR2_CLKEN_POS = 11, // Clock enable
    USART_CR2_CPOL_POS  = 10, // Clock polarity
    USART_CR2_CPHA_POS  = 9,  // Clock phase
    USART_CR2_LBCL_POS  = 8,  // Last bit clock pulse
    USART_CR2_LBDIE_POS = 6,  // LIN break detection interrupt enable
    USART_CR2_LBDL_POS  = 5,  // lin break detection length
    USART_CR2_ADD_POS   = 0,  // Address of the USART node
} usart_cr2_pos_e;

typedef enum {
    USART_CR2_LINEN = (1 << USART_CR2_LINEN_POS), // LIN mode enable
    USART_CR2_STOP  = (1 << USART_CR2_STOP_POS),  // STOP bits
    USART_CR2_CLKEN = (1 << USART_CR2_CLKEN_POS), // Clock enable
    USART_CR2_CPOL  = (1 << USART_CR2_CPOL_POS),  // Clock polarity
    USART_CR2_CPHA  = (1 << USART_CR2_CPHA_POS),  // Clock phase
    USART_CR2_LBCL  = (1 << USART_CR2_LBCL_POS),  // Last bit clock pulse
    USART_CR2_LBDIE = (1 << USART_CR2_LBDIE_POS), // LIN break detection interrupt enable
    USART_CR2_LBDL  = (1 << USART_CR2_LBDL_POS),  // lin break detection length
    USART_CR2_ADD   = (1 << USART_CR2_ADD_POS),   // Address of the USART node
} usart_cr2_e;

typedef enum {
    USART_CR2_LINEN_MASK = 0b1,    // 1 bit(s)
    USART_CR2_STOP_MASK  = 0b11,   // 2 bit(s)
    USART_CR2_CLKEN_MASK = 0b1,    // 1 bit(s)
    USART_CR2_CPOL_MASK  = 0b1,    // 1 bit(s)
    USART_CR2_CPHA_MASK  = 0b1,    // 1 bit(s)
    USART_CR2_LBCL_MASK  = 0b1,    // 1 bit(s)
    USART_CR2_LBDIE_MASK = 0b1,    // 1 bit(s)
    USART_CR2_LBDL_MASK  = 0b1,    // 1 bit(s)
    USART_CR2_ADD_MASK   = 0b1111, // 4 bit(s)
} usart_cr2_mask_e;

typedef enum {
    USART_CR3_ONEBIT_POS = 11, // One sample bit method enable
    USART_CR3_CTSIE_POS  = 10, // CTS interrupt enable
    USART_CR3_CTSE_POS   = 9,  // CTS enable
    USART_CR3_RTSE_POS   = 8,  // RTS enable
    USART_CR3_DMAT_POS   = 7,  // DMA enable transmitter
    USART_CR3_DMAR_POS   = 6,  // DMA enable receiver
    USART_CR3_SCEN_POS   = 5,  // Smartcard mode enable
    USART_CR3_NACK_POS   = 4,  // Smartcard NACK enable
    USART_CR3_HDSEL_POS  = 3,  // Half-duplex selection
    USART_CR3_IRLP_POS   = 2,  // IrDA low-power
    USART_CR3_IREN_POS   = 1,  // IrDA mode enable
    USART_CR3_EIE_POS    = 0,  // Error interrupt enable
} usart_cr3_pos_e;

typedef enum {
    USART_CR3_ONEBIT = (1 << USART_CR3_ONEBIT_POS), // One sample bit method enable
    USART_CR3_CTSIE  = (1 << USART_CR3_CTSIE_POS),  // CTS interrupt enable
    USART_CR3_CTSE   = (1 << USART_CR3_CTSE_POS),   // CTS enable
    USART_CR3_RTSE   = (1 << USART_CR3_RTSE_POS),   // RTS enable
    USART_CR3_DMAT   = (1 << USART_CR3_DMAT_POS),   // DMA enable transmitter
    USART_CR3_DMAR   = (1 << USART_CR3_DMAR_POS),   // DMA enable receiver
    USART_CR3_SCEN   = (1 << USART_CR3_SCEN_POS),   // Smartcard mode enable
    USART_CR3_NACK   = (1 << USART_CR3_NACK_POS),   // Smartcard NACK enable
    USART_CR3_HDSEL  = (1 << USART_CR3_HDSEL_POS),  // Half-duplex selection
    USART_CR3_IRLP   = (1 << USART_CR3_IRLP_POS),   // IrDA low-power
    USART_CR3_IREN   = (1 << USART_CR3_IREN_POS),   // IrDA mode enable
    USART_CR3_EIE    = (1 << USART_CR3_EIE_POS),    // Error interrupt enable
} usart_cr3_e;

typedef enum {
    USART_CR3_ONEBIT_MASK = 0b1, // 1 bit(s)
    USART_CR3_CTSIE_MASK  = 0b1, // 1 bit(s)
    USART_CR3_CTSE_MASK   = 0b1, // 1 bit(s)
    USART_CR3_RTSE_MASK   = 0b1, // 1 bit(s)
    USART_CR3_DMAT_MASK   = 0b1, // 1 bit(s)
    USART_CR3_DMAR_MASK   = 0b1, // 1 bit(s)
    USART_CR3_SCEN_MASK   = 0b1, // 1 bit(s)
    USART_CR3_NACK_MASK   = 0b1, // 1 bit(s)
    USART_CR3_HDSEL_MASK  = 0b1, // 1 bit(s)
    USART_CR3_IRLP_MASK   = 0b1, // 1 bit(s)
    USART_CR3_IREN_MASK   = 0b1, // 1 bit(s)
    USART_CR3_EIE_MASK    = 0b1, // 1 bit(s)
} usart_cr3_mask_e;

typedef enum {
    USART_GTPR_GT_POS  = 8, // Guard time value
    USART_GTPR_PSC_POS = 0, // Prescaler value
} usart_gtpr_pos_e;

typedef enum {
    USART_GTPR_GT  = (1 << USART_GTPR_GT_POS),  // Guard time value
    USART_GTPR_PSC = (1 << USART_GTPR_PSC_POS), // Prescaler value
} usart_gtpr_e;

typedef enum {
    USART_GTPR_GT_MASK  = 0b11111111, // 8 bit(s)
    USART_GTPR_PSC_MASK = 0b11111111, // 8 bit(s)
} usart_gtpr_mask_e;

// Configuration settings
typedef enum {
    USART_OVERSAMPLING_16 = 0,
    USART_OVERSAMPLING_8  = 1,
} usart_oversampling_e;

typedef enum {
    USART_WORD_LENGTH_8_DATA_BITS = 0,
    USART_WORD_LENGTH_9_DATA_BITS = 1,
} usart_word_length_e;

typedef enum {
    USART_PARITY_CONTROL_DISABLE = 0,
    USART_PARITY_CONTROL_ENABLE  = 1,
} usart_parity_control_e;

typedef enum {
    USART_PARITY_SEL_EVEN = 0,
    USART_PARITY_SEL_ODD  = 1,
} usart_parity_select_e;

typedef enum {
    USART_STOP_BITS_1   = 0,
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
    USART_STATUS_READY = 0,
    USART_STATUS_RUNNING,
} usart_status_e;

//======================================================================================//
//                  Structure Definitions
//======================================================================================//

typedef __vo struct {
    uint32_t SR;   // Status register                        Offset: 0x0
    uint32_t DR;   // Data register                          Offset: 0x4
    uint32_t BRR;  // Baud rate register                     Offset: 0x8
    uint32_t CR1;  // Control register 1                     Offset: 0xC
    uint32_t CR2;  // Control register 2                     Offset: 0x10
    uint32_t CR3;  // Control register 3                     Offset: 0x14
    uint32_t GTPR; // Guard time and prescaler register      Offset: 0x18
} usart_reg_def;

// Configuration definition (used to initialize and configure USART port settings)
typedef struct {
    usart_oversampling_e oversampling_mode; // Default: USART_OVERSAMPLING_16
    usart_word_length_e word_length;        // Default: USART_WORD_LENGTH_8_DATA_BITS
    usart_parity_control_e parity_control;  // Default: USART_PARITY_CONTROL_DISABLE
    usart_parity_select_e parity_select;    // Default: USART_PARITY_SEL_EVEN
    usart_stop_bits_e stop_bits;            // Default: USART_STOP_BITS_1
    usart_baudrate_e baudrate;              // Default: NA
} usart_config;

// Interrupt handle definition (used to handle the USART interrupts)
typedef struct {
    uint8_t *txrx_buffer;
    uint32_t txrx_length;
    usart_mode_e mode;
    __vo usart_status_e status; // Default: USART_STATUS_READY
} usart_it_data;

// Handle definition (used to configure and handle the USART features)
typedef struct {
    usart_reg_def *p_usartx;
    usart_config usart_conf;
    usart_it_data usart_it_data;
} usart_handle;

//======================================================================================//
//                  Peripheral Structure Macros
//======================================================================================//

#define USART1 ((usart_reg_def *)USART1_BASE_ADDR)
#define USART2 ((usart_reg_def *)USART2_BASE_ADDR)
#define USART3 ((usart_reg_def *)USART3_BASE_ADDR)
#define UART4  ((usart_reg_def *)UART4_BASE_ADDR)
#define UART5  ((usart_reg_def *)UART5_BASE_ADDR)
#define USART6 ((usart_reg_def *)USART6_BASE_ADDR)

//======================================================================================//
//                  Function API Prototypes
//======================================================================================//

// Init and reset
void usart_init(usart_handle *const p_usart_handle);
void usart_reset(usart_reg_def const *const p_usartx);

// Write/read blocking or polling
void usart_transmit(usart_reg_def *const p_usartx, uint8_t const *p_data, uint32_t length);
void usart_receive(usart_reg_def *const p_usartx, uint8_t *p_data, uint32_t length);
// Used for printf
void usart_transmit_single_byte(usart_reg_def *const p_usartx, uint8_t const p_data);

// Write/read interrupt
void usart_transmit_it(usart_handle *p_usart_handle, uint8_t *p_data, uint32_t const length);
void usart_receive_it(usart_handle *p_usart_handle, uint8_t *p_data, uint32_t const length);

// Interrupt handling
void usart_it_config(usart_reg_def const *const p_usartx, togglable_e toggle);
void usart_it_handler(usart_handle *const p_usart_handle);

// User implement callback function to handle
void __weak usart_callback(usart_handle *p_usart_handle, usart_event_e event);

#endif
