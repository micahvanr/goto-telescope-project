//======================================================================================//{
//  File: usart.c
//  Overview: Provides functions for initializing, configuring, and using the USART peripheral.
//  Features:
//      Transmit (w/ interrupt capability)
//      Receive (w/ interrupt capability)
//  Notes: None
//======================================================================================//}

//} Includes
//=========================================//{

#include "usart.h"
#include "assert_handler.h"
#include "common.h"
#include "nvic.h"
#include "rcc.h"
#include "stm32f4xx.h"

//======================================================================================//}
//                  Private Constants
//======================================================================================//{

//} Initialization Handler Constants
//=========================================//{

typedef enum : uint8_t {
    USART1_INIT_NUM = 0,
    USART2_INIT_NUM = 1,
    USART3_INIT_NUM = 2,
    UART4_INIT_NUM  = 3,
    UART5_INIT_NUM  = 4,
    USART6_INIT_NUM = 5,
} usart_init_port_num_e;

//========================================================//}
//          Register Constants
//========================================================//{

typedef enum : uint32_t {
    USART_BRR_OVER8EN_CLEAR_POS = 3U,
} usart_reg_misc_e;

// clang-format off
typedef enum : uint32_t {
    USART_SR_CTS_POS  = 9U,   // CTS flag
    USART_SR_LBD_POS  = 8U,   // LIN break detection flag
    USART_SR_TXE_POS  = 7U,   // Transmit data register empty
    USART_SR_TC_POS   = 6U,   // Transmission complete
    USART_SR_RXNE_POS = 5U,   // Read data register not empty
    USART_SR_IDLE_POS = 4U,   // IDLE line detected
    USART_SR_ORE_POS  = 3U,   // Overrun error
    USART_SR_NF_POS   = 2U,   // Noise detected flag
    USART_SR_FE_POS   = 1U,   // Framing error
    USART_SR_PE_POS   = 0U,   // Parity error
} usart_sr_pos_e;

typedef enum : uint32_t {
    USART_SR_CTS  = (1U << USART_SR_CTS_POS),    // CTS flag
    USART_SR_LBD  = (1U << USART_SR_LBD_POS),    // LIN break detection flag
    USART_SR_TXE  = (1U << USART_SR_TXE_POS),    // Transmit data register empty
    USART_SR_TC   = (1U << USART_SR_TC_POS),     // Transmission complete
    USART_SR_RXNE = (1U << USART_SR_RXNE_POS),   // Read data register not empty
    USART_SR_IDLE = (1U << USART_SR_IDLE_POS),   // IDLE line detected
    USART_SR_ORE  = (1U << USART_SR_ORE_POS),    // Overrun error
    USART_SR_NF   = (1U << USART_SR_NF_POS),     // Noise detected flag
    USART_SR_FE   = (1U << USART_SR_FE_POS),     // Framing error
    USART_SR_PE   = (1U << USART_SR_PE_POS),     // Parity error
} usart_sr_e;

typedef enum : uint32_t {
    USART_SR_CTS_MASK  = 0b1U,   // 1 bit(s)
    USART_SR_LBD_MASK  = 0b1U,   // 1 bit(s)
    USART_SR_TXE_MASK  = 0b1U,   // 1 bit(s)
    USART_SR_TC_MASK   = 0b1U,   // 1 bit(s)
    USART_SR_RXNE_MASK = 0b1U,   // 1 bit(s)
    USART_SR_IDLE_MASK = 0b1U,   // 1 bit(s)
    USART_SR_ORE_MASK  = 0b1U,   // 1 bit(s)
    USART_SR_NF_MASK   = 0b1U,   // 1 bit(s)
    USART_SR_FE_MASK   = 0b1U,   // 1 bit(s)
    USART_SR_PE_MASK   = 0b1U,   // 1 bit(s)
} usart_sr_mask_e;

typedef enum : uint32_t {
    USART_DR_DR_POS = 0U,   // Data value
} usart_dr_pos_e;

typedef enum : uint32_t {
    USART_DR_DR = (1U << USART_DR_DR_POS),   // Data value
} usart_dr_e;

typedef enum : uint32_t {
    USART_DR_DR_MASK = 0b111111111U,   // 9 bit(s)
} usart_dr_mask_e;

typedef enum : uint32_t {
    USART_BRR_DIV_MANTISSA_POS = 4U,   // mantissa of USARTDIV
    USART_BRR_DIV_FRACTION_POS = 0U,   // fraction of USARTDIV
} usart_brr_pos_e;

typedef enum : uint32_t {
    USART_BRR_DIV_MANTISSA = (1U << USART_BRR_DIV_MANTISSA_POS),   // mantissa of USARTDIV
    USART_BRR_DIV_FRACTION = (1U << USART_BRR_DIV_FRACTION_POS),   // fraction of USARTDIV
} usart_brr_e;

typedef enum : uint32_t {
    USART_BRR_DIV_MANTISSA_MASK = 0b111111111111U,   // 12 bit(s)
    USART_BRR_DIV_FRACTION_MASK = 0b1111U,           // 4 bit(s)
} usart_brr_mask_e;

typedef enum : uint32_t {
    USART_CR1_OVER8_POS  = 15U,   // Oversampling mode
    USART_CR1_UE_POS     = 13U,   // USART enable
    USART_CR1_M_POS      = 12U,   // Word length
    USART_CR1_WAKE_POS   = 11U,   // Wakeup method
    USART_CR1_PCE_POS    = 10U,   // Parity control enable
    USART_CR1_PS_POS     = 9U,    // Parity selection
    USART_CR1_PEIE_POS   = 8U,    // PE interrupt enable
    USART_CR1_TXEIE_POS  = 7U,    // TXE interrupt enable
    USART_CR1_TCIE_POS   = 6U,    // Transmission complete interrupt enable
    USART_CR1_RXNEIE_POS = 5U,    // RXNE interrupt enable
    USART_CR1_IDLEIE_POS = 4U,    // IDLE interrupt enable
    USART_CR1_TE_POS     = 3U,    // Transmitter enable
    USART_CR1_RE_POS     = 2U,    // Receiver enable
    USART_CR1_RWU_POS    = 1U,    // Receiver wakeup
    USART_CR1_SBK_POS    = 0U,    // Send break
} usart_cr1_pos_e;

typedef enum : uint32_t {
    USART_CR1_OVER8  = (1U << USART_CR1_OVER8_POS),    // Oversampling mode
    USART_CR1_UE     = (1U << USART_CR1_UE_POS),       // USART enable
    USART_CR1_M      = (1U << USART_CR1_M_POS),        // Word length
    USART_CR1_WAKE   = (1U << USART_CR1_WAKE_POS),     // Wakeup method
    USART_CR1_PCE    = (1U << USART_CR1_PCE_POS),      // Parity control enable
    USART_CR1_PS     = (1U << USART_CR1_PS_POS),       // Parity selection
    USART_CR1_PEIE   = (1U << USART_CR1_PEIE_POS),     // PE interrupt enable
    USART_CR1_TXEIE  = (1U << USART_CR1_TXEIE_POS),    // TXE interrupt enable
    USART_CR1_TCIE   = (1U << USART_CR1_TCIE_POS),     // Transmission complete interrupt enable
    USART_CR1_RXNEIE = (1U << USART_CR1_RXNEIE_POS),   // RXNE interrupt enable
    USART_CR1_IDLEIE = (1U << USART_CR1_IDLEIE_POS),   // IDLE interrupt enable
    USART_CR1_TE     = (1U << USART_CR1_TE_POS),       // Transmitter enable
    USART_CR1_RE     = (1U << USART_CR1_RE_POS),       // Receiver enable
    USART_CR1_RWU    = (1U << USART_CR1_RWU_POS),      // Receiver wakeup
    USART_CR1_SBK    = (1U << USART_CR1_SBK_POS),      // Send break
} usart_cr1_e;

typedef enum : uint32_t {
    USART_CR1_OVER8_MASK  = 0b1U,   // 1 bit(s)
    USART_CR1_UE_MASK     = 0b1U,   // 1 bit(s)
    USART_CR1_M_MASK      = 0b1U,   // 1 bit(s)
    USART_CR1_WAKE_MASK   = 0b1U,   // 1 bit(s)
    USART_CR1_PCE_MASK    = 0b1U,   // 1 bit(s)
    USART_CR1_PS_MASK     = 0b1U,   // 1 bit(s)
    USART_CR1_PEIE_MASK   = 0b1U,   // 1 bit(s)
    USART_CR1_TXEIE_MASK  = 0b1U,   // 1 bit(s)
    USART_CR1_TCIE_MASK   = 0b1U,   // 1 bit(s)
    USART_CR1_RXNEIE_MASK = 0b1U,   // 1 bit(s)
    USART_CR1_IDLEIE_MASK = 0b1U,   // 1 bit(s)
    USART_CR1_TE_MASK     = 0b1U,   // 1 bit(s)
    USART_CR1_RE_MASK     = 0b1U,   // 1 bit(s)
    USART_CR1_RWU_MASK    = 0b1U,   // 1 bit(s)
    USART_CR1_SBK_MASK    = 0b1U,   // 1 bit(s)
} usart_cr1_mask_e;

typedef enum : uint32_t {
    USART_CR2_LINEN_POS = 14U,   // LIN mode enable
    USART_CR2_STOP_POS  = 12U,   // STOP bits
    USART_CR2_CLKEN_POS = 11U,   // Clock enable
    USART_CR2_CPOL_POS  = 10U,   // Clock polarity
    USART_CR2_CPHA_POS  = 9U,    // Clock phase
    USART_CR2_LBCL_POS  = 8U,    // Last bit clock pulse
    USART_CR2_LBDIE_POS = 6U,    // LIN break detection interrupt enable
    USART_CR2_LBDL_POS  = 5U,    // lin break detection length
    USART_CR2_ADD_POS   = 0U,    // Address of the USART node
} usart_cr2_pos_e;

typedef enum : uint32_t {
    USART_CR2_LINEN = (1U << USART_CR2_LINEN_POS),   // LIN mode enable
    USART_CR2_STOP  = (1U << USART_CR2_STOP_POS),    // STOP bits
    USART_CR2_CLKEN = (1U << USART_CR2_CLKEN_POS),   // Clock enable
    USART_CR2_CPOL  = (1U << USART_CR2_CPOL_POS),    // Clock polarity
    USART_CR2_CPHA  = (1U << USART_CR2_CPHA_POS),    // Clock phase
    USART_CR2_LBCL  = (1U << USART_CR2_LBCL_POS),    // Last bit clock pulse
    USART_CR2_LBDIE = (1U << USART_CR2_LBDIE_POS),   // LIN break detection interrupt enable
    USART_CR2_LBDL  = (1U << USART_CR2_LBDL_POS),    // lin break detection length
    USART_CR2_ADD   = (1U << USART_CR2_ADD_POS),     // Address of the USART node
} usart_cr2_e;

typedef enum : uint32_t {
    USART_CR2_LINEN_MASK = 0b1U,      // 1 bit(s)
    USART_CR2_STOP_MASK  = 0b11U,     // 2 bit(s)
    USART_CR2_CLKEN_MASK = 0b1U,      // 1 bit(s)
    USART_CR2_CPOL_MASK  = 0b1U,      // 1 bit(s)
    USART_CR2_CPHA_MASK  = 0b1U,      // 1 bit(s)
    USART_CR2_LBCL_MASK  = 0b1U,      // 1 bit(s)
    USART_CR2_LBDIE_MASK = 0b1U,      // 1 bit(s)
    USART_CR2_LBDL_MASK  = 0b1U,      // 1 bit(s)
    USART_CR2_ADD_MASK   = 0b1111U,   // 4 bit(s)
} usart_cr2_mask_e;

typedef enum : uint32_t {
    USART_CR3_ONEBIT_POS = 11U,   // One sample bit method enable
    USART_CR3_CTSIE_POS  = 10U,   // CTS interrupt enable
    USART_CR3_CTSE_POS   = 9U,    // CTS enable
    USART_CR3_RTSE_POS   = 8U,    // RTS enable
    USART_CR3_DMAT_POS   = 7U,    // DMA enable transmitter
    USART_CR3_DMAR_POS   = 6U,    // DMA enable receiver
    USART_CR3_SCEN_POS   = 5U,    // Smartcard mode enable
    USART_CR3_NACK_POS   = 4U,    // Smartcard NACK enable
    USART_CR3_HDSEL_POS  = 3U,    // Half-duplex selection
    USART_CR3_IRLP_POS   = 2U,    // IrDA low-power
    USART_CR3_IREN_POS   = 1U,    // IrDA mode enable
    USART_CR3_EIE_POS    = 0U,    // Error interrupt enable
} usart_cr3_pos_e;

typedef enum : uint32_t {
    USART_CR3_ONEBIT = (1U << USART_CR3_ONEBIT_POS),   // One sample bit method enable
    USART_CR3_CTSIE  = (1U << USART_CR3_CTSIE_POS),    // CTS interrupt enable
    USART_CR3_CTSE   = (1U << USART_CR3_CTSE_POS),     // CTS enable
    USART_CR3_RTSE   = (1U << USART_CR3_RTSE_POS),     // RTS enable
    USART_CR3_DMAT   = (1U << USART_CR3_DMAT_POS),     // DMA enable transmitter
    USART_CR3_DMAR   = (1U << USART_CR3_DMAR_POS),     // DMA enable receiver
    USART_CR3_SCEN   = (1U << USART_CR3_SCEN_POS),     // Smartcard mode enable
    USART_CR3_NACK   = (1U << USART_CR3_NACK_POS),     // Smartcard NACK enable
    USART_CR3_HDSEL  = (1U << USART_CR3_HDSEL_POS),    // Half-duplex selection
    USART_CR3_IRLP   = (1U << USART_CR3_IRLP_POS),     // IrDA low-power
    USART_CR3_IREN   = (1U << USART_CR3_IREN_POS),     // IrDA mode enable
    USART_CR3_EIE    = (1U << USART_CR3_EIE_POS),      // Error interrupt enable
} usart_cr3_e;

typedef enum : uint32_t {
    USART_CR3_ONEBIT_MASK = 0b1U,   // 1 bit(s)
    USART_CR3_CTSIE_MASK  = 0b1U,   // 1 bit(s)
    USART_CR3_CTSE_MASK   = 0b1U,   // 1 bit(s)
    USART_CR3_RTSE_MASK   = 0b1U,   // 1 bit(s)
    USART_CR3_DMAT_MASK   = 0b1U,   // 1 bit(s)
    USART_CR3_DMAR_MASK   = 0b1U,   // 1 bit(s)
    USART_CR3_SCEN_MASK   = 0b1U,   // 1 bit(s)
    USART_CR3_NACK_MASK   = 0b1U,   // 1 bit(s)
    USART_CR3_HDSEL_MASK  = 0b1U,   // 1 bit(s)
    USART_CR3_IRLP_MASK   = 0b1U,   // 1 bit(s)
    USART_CR3_IREN_MASK   = 0b1U,   // 1 bit(s)
    USART_CR3_EIE_MASK    = 0b1U,   // 1 bit(s)
} usart_cr3_mask_e;

typedef enum : uint32_t {
    USART_GTPR_GT_POS  = 8U,   // Guard time value
    USART_GTPR_PSC_POS = 0U,   // Prescaler value
} usart_gtpr_pos_e;

typedef enum : uint32_t {
    USART_GTPR_GT  = (1U << USART_GTPR_GT_POS),    // Guard time value
    USART_GTPR_PSC = (1U << USART_GTPR_PSC_POS),   // Prescaler value
} usart_gtpr_e;

typedef enum : uint32_t {
    USART_GTPR_GT_MASK  = 0b11111111U,   // 8 bit(s)
    USART_GTPR_PSC_MASK = 0b11111111U,   // 8 bit(s)
} usart_gtpr_mask_e;

// clang-format on

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

// Same concept with GPIO init variable but with USART ports instead.
uint8_t g_usart_port_init = 0;

//======================================================================================//}
//                  Private Function Prototypes
//======================================================================================//{

// Assert helper functions
static void assert_usart_handle_param(usart_handle const *const p_usart_handle);
static inline void set_usart_init_status(usart_reg_def const *const p_usartx);
static inline bool is_usart_initialized(usart_reg_def const *const p_usartx);

// General helper functions
static inline usart_init_port_num_e map_usart_ports_to_num(usart_reg_def const *const p_usartx);
static void set_baudrate(usart_reg_def *p_usartx, usart_oversampling_e oversampling_mode, usart_baudrate_e baudrate);

static void transfer_data(usart_handle *const p_usart_handle);
static void recieve_data(usart_handle *const p_usart_handle);

//======================================================================================//}
//                  Public Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: usart_init
//  Overview: Initialize the specified USART peripheral according to settings in the handle.
//  Parameters:
//      p_usart_handle: Structure with the corresponding settings to configure the USART peripheral.
//  Return:
//      None
//  Note: None
//============================================================================//}
void usart_init(usart_handle *const p_usart_handle)
{
    uint32_t temp_cr1 = p_usart_handle->p_usartx->CR1;

    // Ensure handle structure values are valid.
    assert_usart_handle_param(p_usart_handle);

    // Initialize the port in the init variable.
    set_usart_init_status(p_usart_handle->p_usartx);

    // Enable clock of USART peripheral.
    rcc_usart_togglable_clock(p_usart_handle->p_usartx, ENABLE);

    // Enable USART peripheral
    temp_cr1 |= USART_CR1_UE;
    temp_cr1 |= USART_CR1_TE;
    temp_cr1 |= USART_CR1_RE;

    // Set oversampling mode
    temp_cr1 &= ~(USART_CR1_OVER8_MASK << USART_CR1_OVER8_POS);
    temp_cr1 |= (p_usart_handle->usart_conf.oversampling_mode << USART_CR1_OVER8_POS);

    // Set word length
    temp_cr1 &= ~(USART_CR1_M_MASK << USART_CR1_M_POS);
    temp_cr1 |= (p_usart_handle->usart_conf.word_length << USART_CR1_M_POS);

    // Set parity control
    temp_cr1 &= ~(USART_CR1_PCE_MASK << USART_CR1_PCE_POS);
    temp_cr1 |= (p_usart_handle->usart_conf.parity_control << USART_CR1_PCE_POS);

    // Set parity selection (even/odd)
    if (USART_PARITY_CONTROL_ENABLE == p_usart_handle->usart_conf.parity_control)
    {
        temp_cr1 &= ~(USART_CR1_PS_MASK << USART_CR1_PS_POS);
        temp_cr1 |= (p_usart_handle->usart_conf.parity_select << USART_CR1_PS_POS);
    }

    p_usart_handle->p_usartx->CR1 = temp_cr1;

    // Set stop bits
    p_usart_handle->p_usartx->CR2 &= ~(USART_CR2_STOP_MASK << USART_CR2_STOP_POS);
    p_usart_handle->p_usartx->CR2 |= (p_usart_handle->usart_conf.stop_bits << USART_CR2_STOP_POS);

    // Set baudrate
    set_baudrate(p_usart_handle->p_usartx, p_usart_handle->usart_conf.oversampling_mode,
                 p_usart_handle->usart_conf.baudrate);
}

// Check each setting of the handle and ensure it is one of the available enum values.
static void assert_usart_handle_param(usart_handle const *const p_usart_handle)
{
    // clang-format off
    uint8_t found_setting = false;

    // Peripheral check
    found_setting = (USART1==  p_usart_handle->p_usartx ) ? true
                  : (USART2==  p_usart_handle->p_usartx ) ? true
                  : (USART3==  p_usart_handle->p_usartx ) ? true
                  : (UART4==  p_usart_handle->p_usartx )  ? true
                  : (UART5==  p_usart_handle->p_usartx )  ? true
                  : (USART6==  p_usart_handle->p_usartx ) ? true
                                                         : false;
    ASSERT(found_setting);

    // Baudrate check
    found_setting = false;
    switch (p_usart_handle->usart_conf.baudrate) {
    case USART_BAUDRATE_1200:   found_setting = true; break;
    case USART_BAUDRATE_2400:   found_setting = true; break;
    case USART_BAUDRATE_9600:   found_setting = true; break;
    case USART_BAUDRATE_19200:  found_setting = true; break;
    case USART_BAUDRATE_38400:  found_setting = true; break;
    case USART_BAUDRATE_57600:  found_setting = true; break;
    case USART_BAUDRATE_115200: found_setting = true; break;
    case USART_BAUDRATE_230400: found_setting = true; break;
    case USART_BAUDRATE_460800: found_setting = true; break;
    case USART_BAUDRATE_921600: found_setting = true; break;
    case USART_BAUDRATE_2MB:    found_setting = true; break;
    case USART_BAUDRATE_3MB:    found_setting = true; break;
    }
    ASSERT(found_setting);

    // Oversampling check
    found_setting = false;
    switch (p_usart_handle->usart_conf.oversampling_mode) {
    case USART_OVERSAMPLING_16: found_setting = true; break;
    case USART_OVERSAMPLING_8:  found_setting = true; break;
    }
    ASSERT(found_setting);

    // Parity control check
    found_setting = false;
    switch (p_usart_handle->usart_conf.parity_control) {
    case USART_PARITY_CONTROL_DISABLE: found_setting = true; break;
    case USART_PARITY_CONTROL_ENABLE:  found_setting = true; break;
    }
    ASSERT(found_setting);

    // Parity selection check
    found_setting = false;
    switch (p_usart_handle->usart_conf.parity_select) {
    case USART_PARITY_SEL_EVEN: found_setting = true; break;
    case USART_PARITY_SEL_ODD:  found_setting = true; break;
    }
    ASSERT(found_setting);

    // Stop bit check
    found_setting = false;
    switch (p_usart_handle->usart_conf.stop_bits) {
    case USART_STOP_BITS_1:   found_setting = true; break;
    case USART_STOP_BITS_0_5: found_setting = true; break;
    case USART_STOP_BITS_2:   found_setting = true; break;
    case USART_STOP_BITS_1_5: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Word length check
    found_setting = false;
    switch (p_usart_handle->usart_conf.word_length) {
    case USART_WORD_LENGTH_8_DATA_BITS: found_setting = true; break;
    case USART_WORD_LENGTH_9_DATA_BITS: found_setting = true; break;
    }
    ASSERT(found_setting);
    // clang-format on
}

//============================================================================//{
//  Function: usart_reset
//  Overview: Reset the specified specified USART peripheral.
//  Parameters:
//      p_usartx: USART register structure macro.
//          USARTx (1, 2, 3, 6)
//          UARTx (4, 5)
//  Return:
//      None
//  Note: None
//============================================================================//}
void usart_reset(usart_reg_def const *const p_usartx)
{
    rcc_usart_togglable_clock(p_usartx, DISABLE);
}

//============================================================================//{
//  Function: usart_transmit
//  Overview: Transmit data with the specified usart peripheral.
//  Parameters:
//      p_usartx: USART register structure macro.
//          USARTx (1, 2, 3, 6)
//          UARTx (4, 5)
//      p_data: Array of data bytes to write to peripheral.
//      length: Length of data array.
//  Return:
//      None
//  Note: This function is blocking.
//      9 Bit write with no parity not implemented.
//      If sending string, ensure the size does not include the '\0' character.
//============================================================================//}
void usart_transmit(usart_reg_def *const p_usartx, uint8_t const *p_data, uint32_t const length)
{
    ASSERT(is_usart_initialized(p_usartx));
    p_usartx->CR1 |= USART_CR1_TE;

    for (uint32_t data_pos = 0; data_pos < length; data_pos++)
    {
        while (!(p_usartx->SR & USART_SR_TXE));
        p_usartx->DR = *p_data;
        p_data++;
    }

    UNUSED(length);
}

//============================================================================//{
//  Function: usart_receive (blocking)
//  Overview: Receive data with the specified usart peripheral.
//  Parameters:
//      p_usartx: USART register structure macro.
//          USARTx (1, 2, 3, 6)
//          UARTx (4, 5)
//      p_data: Array of data bytes to read from peripheral.
//      length: Length of data array.
//  Return:
//      None
//  Note: This function is blocking.
//      9 Bit write with no parity not implemented.
//============================================================================//}
void usart_receive(usart_reg_def *const p_usartx, uint8_t *p_data, uint32_t const length)
{
    ASSERT(is_usart_initialized(p_usartx));
    p_usartx->CR1 |= USART_CR1_RE;

    for (uint32_t data_pos = 0; data_pos < length; data_pos++)
    {
        while (!(p_usartx->SR & USART_SR_RXNE));
        *p_data = p_usartx->DR;
        p_data++;
    }
}

//============================================================================//{
//  Function: usart_transmit_it
//  Overview: Transmit data with interrupts with the specified usart peripheral.
//  Parameters:
//      p_usart_handle: Structure with the corresponding settings to configure the USART peripheral.
//      p_data: Pointer to where data should be written from.
//      length: Length in bytes of data.
//  Return:
//      None
//  Note:
//      This is non-blocking.
//      9 Bit write with no parity not implemented.
//      If sending string, ensure the size does not include the '\0' character.
//      If the USART peripheral is busy transmitting/receiving, it will poll until it is available.
//============================================================================//}
void usart_transmit_it(usart_handle *const p_usart_handle, uint8_t *const p_data, uint32_t const length)
{
    ASSERT(is_usart_initialized(p_usart_handle->p_usartx));

    // Wait until usart is available (blocking).
    while (USART_STATUS_RUNNING == p_usart_handle->usart_it_data.status);
    p_usart_handle->usart_it_data.status = USART_STATUS_RUNNING;

    // Configure transfer settings.
    p_usart_handle->usart_it_data.txrx_length = length;
    p_usart_handle->usart_it_data.txrx_buffer = p_data;
    p_usart_handle->usart_it_data.mode        = USART_MODE_TX;

    usart_toggable_it(p_usart_handle->p_usartx, ENABLE);
}

//============================================================================//{
//  Function: usart_receive_it (non-blocking)
//  Overview: Receive data with interrupts with the specified usart peripheral.
//  Parameters:
//      p_usart_handle: Structure with the corresponding settings to configure the USART peripheral.
//      p_data: Pointer to where data should be read from.
//      length: Length in bytes of data.
//  Return:
//      None
//  Note:
//      9 Bit write with no parity not implemented.
//      If the USART peripheral is busy transmitting/receiving, it will poll until it is available.
//============================================================================//}
void usart_receive_it(usart_handle *const p_usart_handle, uint8_t *const p_data, uint32_t const length)
{
    ASSERT(is_usart_initialized(p_usart_handle->p_usartx));

    // Wait until usart is available (blocking).
    while (USART_STATUS_RUNNING == p_usart_handle->usart_it_data.status);
    p_usart_handle->usart_it_data.status = USART_STATUS_RUNNING;

    // Configure read settings.
    p_usart_handle->usart_it_data.txrx_length = length;
    p_usart_handle->usart_it_data.txrx_buffer = p_data;
    p_usart_handle->usart_it_data.mode        = USART_MODE_RX;

    if (USART_PARITY_CONTROL_ENABLE == p_usart_handle->usart_conf.parity_control)
    {
        p_usart_handle->p_usartx->CR1 |= USART_CR1_PCE;
    }

    usart_toggable_it(p_usart_handle->p_usartx, ENABLE);
}

//============================================================================//{
//  Function: usart_toggable_it
//  Overview: Enable or disable the specified USART peripheral's interrupts.
//  Parameters:
//      p_usartx: USART register structure macro.
//          USARTx (1, 2, 3, 6)
//          UARTx (4, 5)
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void usart_toggable_it(usart_reg_def *const p_usartx, togglable_e const toggle)
{
    uint32_t temp_cr1 = p_usartx->CR1;
    switch (toggle)
    {
        case ENABLE:
            usart_toggable_it(p_usartx, ENABLE);
            temp_cr1 |= USART_CR1_UE | USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_PEIE;
            p_usartx->CR1 = temp_cr1;
            break;
        case DISABLE:
            temp_cr1 &=
                ~(USART_CR1_RXNEIE | USART_CR1_TCIE | USART_CR1_TXEIE | USART_CR1_PEIE | USART_CR1_PCE | USART_CR1_UE);
            p_usartx->CR1 = temp_cr1;
            break;
    }

    if (USART1 == p_usartx)
    {
        nvic_toggable_irq(USART1_IRQ_NUM_37, toggle);
    }
    else if (USART2 == p_usartx)
    {
        nvic_toggable_irq(USART2_IRQ_NUM_38, toggle);
    }
    else if (USART3 == p_usartx)
    {
        nvic_toggable_irq(USART3_IRQ_NUM_39, toggle);
    }
    else if (UART4 == p_usartx)
    {
        nvic_toggable_irq(UART4_IRQ_NUM_52, toggle);
    }
    else if (UART5 == p_usartx)
    {
        nvic_toggable_irq(UART5_IRQ_NUM_53, toggle);
    }
    else if (USART6 == p_usartx)
    {
        nvic_toggable_irq(USART6_IRQ_NUM_71, toggle);
    }
    else
    {
        ASSERT(false);
    }
}

//============================================================================//{
//  Function: usart_it_handler
//  Overview: Handle interrupts for USART peripheral.
//  Parameters:
//      p_usart_handle: Structure with the corresponding settings to configure the USART peripheral.
//  Return:
//      None
//  Note: This function should be called by the applications IT handler.
//============================================================================//}
void usart_it_handler(usart_handle *const p_usart_handle)
{
    // Check for flags.
    uint32_t status_reg = p_usart_handle->p_usartx->SR;

    switch (p_usart_handle->usart_it_data.mode)
    {

        // Receive data
        case USART_MODE_RX:
            if ((status_reg & USART_SR_PE) || (status_reg & USART_SR_FE) || (status_reg & USART_SR_NF)
                || (status_reg & USART_SR_ORE))
            {
                usart_callback(p_usart_handle, USART_EVENT_ERROR);
                ASSERT(false);
            }
            if (p_usart_handle->p_usartx->SR & USART_SR_RXNE)
            {
                recieve_data(p_usart_handle);
            }
            if (0 == p_usart_handle->usart_it_data.txrx_length)
            {
                usart_toggable_it(p_usart_handle->p_usartx, DISABLE);
                p_usart_handle->usart_it_data.status = USART_STATUS_READY;
                usart_callback(p_usart_handle, USART_EVENT_CMPLT);
            }
            break;

        // Tranfer data
        case USART_MODE_TX:
            //  Disable interrupts before reading transmit register so IT will not trigger again after it is empty.
            if ((p_usart_handle->p_usartx->SR & USART_SR_TXE) && (p_usart_handle->usart_it_data.txrx_length != 0))
            {
                transfer_data(p_usart_handle);
            }
            if ((0 == p_usart_handle->usart_it_data.txrx_length) && (status_reg & USART_SR_TC))
            {
                usart_toggable_it(p_usart_handle->p_usartx, DISABLE);
                p_usart_handle->p_usartx->CR1 &= ~USART_CR1_UE;
                p_usart_handle->usart_it_data.status = USART_STATUS_READY;
                usart_callback(p_usart_handle, USART_EVENT_CMPLT);
            }

            break;
        default:
            ASSERT(false);
    }
}

//============================================================================//}
//  Function: usart_transmit_single_byte
//  Overview: Transmit 1 byte of data into the given usart peripheral.
//  Parameters:
//      p_usartx: USART register structure macro.
//          USARTx (1, 2, 3, 6)
//          UARTx (4, 5)
//      data: Byte of data to be sent.
//  Return:
//      None
//  Note: Used for printf implementation.
//============================================================================//}
void usart_transmit_single_byte(usart_reg_def *const p_usartx, uint8_t const data)
{
    ASSERT(is_usart_initialized(p_usartx));
    p_usartx->CR1 |= USART_CR1_TE;

    while (!(p_usartx->SR & USART_SR_TXE));
    p_usartx->DR = data;
}

//============================================================================//{
//  Function: usart_get_bus
//  Overview: Get the specified USART peripheral's bus.
//  Parameters:
//      p_usartx: USART register structure macro.
//          USARTx (1, 2, 3, 6)
//          UARTx (4, 5)
//  Return:
//      bus_types:
//          AHB1_BUS
//          AHB2_BUS
//          APB1_BUS
//          APB2_BUS
//  Note: None
//============================================================================//}
bus_types get_usart_bus(usart_reg_def const *const p_usartx)
{
    bus_types usart_bus = 0;
    // If USART peripheral on APB1.
    if ((USART2 == p_usartx) || (USART3 == p_usartx) || (UART4 == p_usartx) || (UART5 == p_usartx))
    {
        usart_bus = APB1_BUS;
    }
    // If USART peripheral on APB2.
    else
    {
        usart_bus = APB2_BUS;
    }

    return usart_bus;
}

//======================================================================================//}
//                  Private Function Implementation
//======================================================================================//{

static inline usart_init_port_num_e map_usart_ports_to_num(usart_reg_def const *const p_usartx)
{
    usart_init_port_num_e port_num = 0;
    if (USART1 == p_usartx)
    {
        port_num = USART1_INIT_NUM;
    }
    else if (USART2 == p_usartx)
    {
        port_num = USART2_INIT_NUM;
    }
    else if (USART3 == p_usartx)
    {
        port_num = USART3_INIT_NUM;
    }
    else if (UART4 == p_usartx)
    {
        port_num = UART4_INIT_NUM;
    }
    else if (UART5 == p_usartx)
    {
        port_num = UART5_INIT_NUM;
    }
    else if (USART6 == p_usartx)
    {
        port_num = USART6_INIT_NUM;
    }
    else
    {
        ASSERT(0);
    }

    return port_num;
}

static inline void set_usart_init_status(usart_reg_def const *const p_usartx)
{
    g_usart_port_init |= (1 << map_usart_ports_to_num(p_usartx));
}

static inline bool is_usart_initialized(usart_reg_def const *const p_usartx)
{
    return 0b1 & (g_usart_port_init >> map_usart_ports_to_num(p_usartx));
}

// Sets the baudrate for the given USART peripheral.
static void set_baudrate(usart_reg_def *const p_usartx, usart_oversampling_e const oversampling_mode,
                         usart_baudrate_e const baudrate)
{
    uint32_t temp_brr;
    uint32_t clock_freq;
    bus_types usart_bus;

    uint32_t usart_div;
    uint32_t div_mantissa;
    uint32_t div_fraction;
    uint32_t over8;

    // If over8 enabled, it will be 8, otherwise it will be 16.
    over8 = (8 * (2 - oversampling_mode));

    // Get clock frequency
    usart_bus  = ((USART1 == p_usartx) || (USART6 == p_usartx)) ? APB2_BUS : APB1_BUS;
    clock_freq = rcc_get_bus_clock_freq_hz(usart_bus);

    // Calculate mantissa part
    div_mantissa = (clock_freq / (over8 * baudrate));

    // Calculate fraction part
    usart_div    = ((100 * clock_freq) / (over8 * baudrate));
    div_fraction = (usart_div - (div_mantissa * 100));

    // Fraction part must be multiplied by over8 then rounded.
    div_fraction = (div_fraction * over8);
    div_fraction = (div_fraction + 50) / 100;

    // If fraction part is too big, carry into mantissa section.
    if (0x10 == div_fraction)
    {
        div_mantissa += 1;
        div_fraction = 0;
    }

    // Set up variable to write into register.
    temp_brr = 0;
    temp_brr |= (div_fraction << USART_BRR_DIV_FRACTION_POS);
    temp_brr |= (div_mantissa << USART_BRR_DIV_MANTISSA_POS);

    // 3rd bit must be kept clear if over8 is used.
    if (USART_OVERSAMPLING_8 == oversampling_mode)
    {
        temp_brr &= ~(1 << USART_BRR_OVER8EN_CLEAR_POS);
    }

    p_usartx->BRR = temp_brr;
}

static void transfer_data(usart_handle *const p_usart_handle)
{
    p_usart_handle->p_usartx->DR = (uint32_t) *p_usart_handle->usart_it_data.txrx_buffer;
    p_usart_handle->usart_it_data.txrx_length--;
    p_usart_handle->usart_it_data.txrx_buffer++;
}

static void recieve_data(usart_handle *const p_usart_handle)
{
    *p_usart_handle->usart_it_data.txrx_buffer = p_usart_handle->p_usartx->DR;
    p_usart_handle->usart_it_data.txrx_length--;
    p_usart_handle->usart_it_data.txrx_buffer++;
}

// *End of file*
