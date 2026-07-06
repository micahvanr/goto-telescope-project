#include "usart.h"
#include "assert_handler.h"
#include "common.h"
#include "rcc.h"

/*****************************************************************
                        Helper Function Prototypes
*****************************************************************/

// Assert helper functions
static void usart_init_asserts(usart_handle const *const p_usart_handle);

// General helper functions
static inline usart_init_port_num_e map_usart_ports_to_num(usart_reg_def const *const p_usartx);
static inline bool verify_usart_initialized(usart_reg_def const *const p_usartx);
static void set_baudrate(usart_reg_def *p_usartx, usart_oversampling_e oversampling_mode, usart_baudrate_e baudrate);

static void transfer_data(usart_handle *p_usart_handle);
static void recieve_data(usart_handle *const p_usart_handle);
static inline void enable_interrupts(usart_reg_def *const p_usartx);
static inline void disable_interrupts(usart_reg_def *const p_usartx);

static inline void usart_clock_enable(usart_reg_def const *const p_usartx);
static inline void usart_clock_disable(usart_reg_def const *const p_usartx);

/*****************************************************************
                        Global variables
*****************************************************************/

// Same concept with GPIO init variable but with USART ports instead.
uint8_t g_usart_port_init = 0;

/****************************************************************************************************
                                    Peripheral Function API Implementation
****************************************************************************************************/

/***************************************************************************
Function: usart_init
Overview: Initializes the USART peripheral inside the handle with the settings in the configuration structure
Parameters:
    p_usart_handle: Structure with the corresponding settings to configure the USART peripheral
Return: 
    None
Note: None
***************************************************************************/
void usart_init(usart_handle *const p_usart_handle)
{
    uint32_t temp_reg;

    // Ensure handle structure values are valid
    usart_init_asserts(p_usart_handle);

    // Initialize the port in the init variable
    g_usart_port_init |= (1 << map_usart_ports_to_num(p_usart_handle->p_usartx));

    // Enable clock of USART peripheral
    usart_clock_enable(p_usart_handle->p_usartx);

    // Enable USART peripheral
    p_usart_handle->p_usartx->CR1 |= USART_CR1_UE;
    p_usart_handle->p_usartx->CR1 |= USART_CR1_TE;
    p_usart_handle->p_usartx->CR1 |= USART_CR1_RE;

    // Set CR1 - Reset value: 0x0000 0000
    temp_reg = 0;
    // Set oversampling mode
    temp_reg |= (p_usart_handle->usart_conf.oversampling_mode << USART_CR1_OVER8_POS);

    // Set word length
    temp_reg |= (p_usart_handle->usart_conf.word_length << USART_CR1_M_POS);

    // Set parity control
    temp_reg |= (p_usart_handle->usart_conf.parity_control << USART_CR1_PCE_POS);

    // Set parity selection (even/odd)
    if (p_usart_handle->usart_conf.parity_control == USART_PARITY_CONTROL_ENABLE) {
        temp_reg |= (p_usart_handle->usart_conf.parity_select << USART_CR1_PS_POS);
    }

    p_usart_handle->p_usartx->CR1 |= temp_reg;

    // Set CR2 - Reset value: 0x0000 0000
    temp_reg = 0;

    // Set stop bits
    temp_reg |= (p_usart_handle->usart_conf.stop_bits << USART_CR2_STOP_POS);

    p_usart_handle->p_usartx->CR2 |= temp_reg;

    // Set baudrate
    set_baudrate(p_usart_handle->p_usartx, p_usart_handle->usart_conf.oversampling_mode,
                 p_usart_handle->usart_conf.baudrate);
}

// Check each setting of the handle and ensure it is one of the available enum values
static void usart_init_asserts(usart_handle const *const p_usart_handle)
{
    uint8_t found_setting = false;

    // Peripheral check
    found_setting = (p_usart_handle->p_usartx == USART1) ? true
                  : (p_usart_handle->p_usartx == USART2) ? true
                  : (p_usart_handle->p_usartx == USART3) ? true
                  : (p_usart_handle->p_usartx == UART4)  ? true
                  : (p_usart_handle->p_usartx == UART5)  ? true
                  : (p_usart_handle->p_usartx == USART6) ? true
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
}

/***************************************************************************
Function: usart_reset
Overview: Disables the clock for the given USART peripheral, resetting it
Parameters:
    p_usart_handle: USART register peripheral to be reset
Return: 
    None
Note: None
***************************************************************************/
void usart_reset(usart_reg_def const *const p_usartx)
{
    usart_clock_disable(p_usartx);
}

/***************************************************************************
Function: usart_transmit (blocking)
Overview: Writes an array of data into the given usart peripheral
Parameters:
    p_usartx: USART register peripheral to be written to
        USARTx (1, 2, 3, 6) or
        UARTx (4, 5)
    p_data: Array of data bytes to write to peripheral
    length: Length of data array
Return: 
    None
Note: 
    9 Bit write with no parity not implemented.
    If sending string, ensure the size does not include the '\0' character. 
***************************************************************************/
void usart_transmit(usart_reg_def *const p_usartx, uint8_t const *p_data, uint32_t length)
{
    ASSERT(verify_usart_initialized(p_usartx));
    p_usartx->CR1 |= USART_CR1_TE;

    for (uint32_t i = 0; i < length; i++) {
        while (!(p_usartx->SR & USART_SR_TXE));
        p_usartx->DR = *p_data;
        p_data++;
    }

    UNUSED(length);
}

/***************************************************************************
Function: usart_receive (blocking)
Overview: Reads an array of data from the given usart peripheral
Parameters:
    p_usartx: USART register peripheral to be read from
        USARTx (1, 2, 3, 6) or
        UARTx (4, 5)
    p_data: Array of data bytes to read from peripheral
    length: Length of data array
Return: 
    None
Note: 9 Bit write with no parity not implemented
***************************************************************************/
void usart_receive(usart_reg_def *const p_usartx, uint8_t *p_data, uint32_t length)
{
    ASSERT(verify_usart_initialized(p_usartx));
    p_usartx->CR1 |= USART_CR1_RE;

    for (uint32_t i = 0; i < length; i++) {
        while (!(p_usartx->SR & USART_SR_RXNE));
        *p_data = p_usartx->DR;
        p_data++;
    }
}

/***************************************************************************
Function: usart_transmit_it (non-blocking)
Overview: Uses interrupts to write data with the USART peripheral
Parameters:
    p_usart_handle: Structure with the corresponding settings to configure the USART peripheral
    p_data: Pointer to where data should be written from
    length: Length in bytes of data
Return: 
    None
Note: 
    9 Bit write with no parity not implemented.
    If sending string, ensure the size does not include the '\0' character. 
    If the USART peripheral is busy transmitting/receiving, it will poll until it is available.
***************************************************************************/
void usart_transmit_it(usart_handle *p_usart_handle, uint8_t *p_data, uint32_t const length)
{
    ASSERT(verify_usart_initialized(p_usart_handle->p_usartx));

    // Wait until usart is available (blocking)
    while (p_usart_handle->usart_it_data.status == USART_STATUS_RUNNING);
    p_usart_handle->usart_it_data.status = USART_STATUS_RUNNING;

    // Configure transfer settings
    p_usart_handle->usart_it_data.txrx_length = length;
    p_usart_handle->usart_it_data.txrx_buffer = p_data;
    p_usart_handle->usart_it_data.mode        = USART_MODE_TX;

    enable_interrupts(p_usart_handle->p_usartx);
}

/***************************************************************************
Function: usart_receive_it (non-blocking)
Overview: Uses interrupts to read data with the USART peripheral
Parameters:
    p_usart_handle: Structure with the corresponding settings to configure the USART peripheral
    p_data: Pointer to where data should be read from
    length: Length in bytes of data
Return: 
    None
Note: 
    9 Bit write with no parity not implemented
    If the USART peripheral is busy transmitting/receiving, it will poll until it is available.
***************************************************************************/
void usart_receive_it(usart_handle *p_usart_handle, uint8_t *p_data, uint32_t const length)
{
    ASSERT(verify_usart_initialized(p_usart_handle->p_usartx));

    // Wait until usart is available (blocking)
    while (p_usart_handle->usart_it_data.status == USART_STATUS_RUNNING);
    p_usart_handle->usart_it_data.status = USART_STATUS_RUNNING;

    // Configure read settings
    p_usart_handle->usart_it_data.txrx_length = length;
    p_usart_handle->usart_it_data.txrx_buffer = p_data;
    p_usart_handle->usart_it_data.mode        = USART_MODE_RX;

    if (p_usart_handle->usart_conf.parity_control == USART_PARITY_CONTROL_ENABLE) {
        p_usart_handle->p_usartx->CR1 |= USART_CR1_PCE;
    }
    enable_interrupts(p_usart_handle->p_usartx);
}

/***************************************************************************
Function: usart_it_config
Overview: Enables or disables the given USART peripheral interrupts
Parameters:
    p_usartx: USART peripheral 
        USARTx (1, 2, 3, 6) or
        UARTx (4, 5)
    toggle: 
        ENABLE (1)
        DISABLE (0)
Return: 
    None
Note: None
***************************************************************************/
void usart_it_config(usart_reg_def const *const p_usartx, togglable_e toggle)
{
    if (p_usartx == USART1) {
        irq_config(USART1_IRQ_NO_37, toggle);
    } else if (p_usartx == USART2) {
        irq_config(USART2_IRQ_NO_38, toggle);
    } else if (p_usartx == USART3) {
        irq_config(USART3_IRQ_NO_39, toggle);
    } else if (p_usartx == UART4) {
        irq_config(UART4_IRQ_NO_52, toggle);
    } else if (p_usartx == UART5) {
        irq_config(UART5_IRQ_NO_53, toggle);
    } else if (p_usartx == USART6) {
        irq_config(USART6_IRQ_NO_71, toggle);
    }
}

/***************************************************************************
Function: usart_it_handler
Overview: 
Parameters:
    p_usart_handle: Structure with the corresponding settings to configure the USART peripheral
Return: 
    None
Note: Should be called from the ISR function
***************************************************************************/
void usart_it_handler(usart_handle *const p_usart_handle)
{
    // Check for flags
    uint32_t status_reg = p_usart_handle->p_usartx->SR;

    switch (p_usart_handle->usart_it_data.mode) {

    // Receive data
    case USART_MODE_RX:
        if ((status_reg & USART_SR_PE) || (status_reg & USART_SR_FE) || (status_reg & USART_SR_NF)
            || (status_reg & USART_SR_ORE)) {
            usart_callback(p_usart_handle, USART_EVENT_ERROR);
            ASSERT(false);
        }
        if (p_usart_handle->p_usartx->SR & USART_SR_RXNE) {
            recieve_data(p_usart_handle);
        }
        if (p_usart_handle->usart_it_data.txrx_length == 0) {
            disable_interrupts(p_usart_handle->p_usartx);
            p_usart_handle->usart_it_data.status = USART_STATUS_READY;
            usart_callback(p_usart_handle, USART_EVENT_CMPLT);
        }
        break;

    // Tranfer data
    case USART_MODE_TX:
        //  Disable interrupts before reading transmit register so IT will not trigger again after it is empty
        if ((p_usart_handle->p_usartx->SR & USART_SR_TXE) && (p_usart_handle->usart_it_data.txrx_length != 0)) {
            transfer_data(p_usart_handle);
        }
        if ((p_usart_handle->usart_it_data.txrx_length == 0) && (status_reg & USART_SR_TC)) {
            disable_interrupts(p_usart_handle->p_usartx);
            p_usart_handle->p_usartx->CR1 &= ~USART_CR1_UE;
            p_usart_handle->usart_it_data.status = USART_STATUS_READY;
            usart_callback(p_usart_handle, USART_EVENT_CMPLT);
        }

        break;
    default: ASSERT(false);
    }
}

/***************************************************************************
Function: usart_transmit_single_byte
Overview: Writes 1 byte of data into the given usart peripheral
Parameters:
    p_usartx: USART register peripheral to be written to
        USARTx (1, 2, 3, 6) or
        UARTx (4, 5)
    data: Byte of data to be sent
Return: 
    None
Note: 
    Used for printf implementation
***************************************************************************/
void usart_transmit_single_byte(usart_reg_def *const p_usartx, uint8_t const data)
{
    ASSERT(verify_usart_initialized(p_usartx));
    p_usartx->CR1 |= USART_CR1_TE;

    while (!(p_usartx->SR & USART_SR_TXE));
    p_usartx->DR = data;
}
/****************************************************************************************************
                                Helper Function Implementation
****************************************************************************************************/

static inline void usart_clock_enable(usart_reg_def const *const p_usartx)
{
    // If USART peripheral on APB1
    if ((p_usartx == USART2) || (p_usartx == USART3) || (p_usartx == UART4) || (p_usartx == UART5)) {
        RCC->APB1ENR |= (p_usartx == USART2) ? RCC_APB1ENR_USART2
                      : (p_usartx == USART3) ? RCC_APB1ENR_USART3
                      : (p_usartx == UART4)  ? RCC_APB1ENR_UART4
                      : (p_usartx == UART5)  ? RCC_APB1ENR_UART5
                                             : 0;
    }
    // If USART peripheral on APB2
    else {
        RCC->APB2ENR |= (p_usartx == USART1) ? RCC_APB2ENR_USART1 : (p_usartx == USART6) ? RCC_APB2ENR_USART6 : 0;
    }
}
static inline void usart_clock_disable(usart_reg_def const *const p_usartx)
{
    // If USART peripheral on APB1
    if ((p_usartx == USART2) || (p_usartx == USART3) || (p_usartx == UART4) || (p_usartx == UART5)) {
        RCC->APB1RSTR |= (p_usartx == USART2) ? RCC_APB1ENR_USART2
                       : (p_usartx == USART3) ? RCC_APB1ENR_USART3
                       : (p_usartx == UART4)  ? RCC_APB1ENR_UART4
                       : (p_usartx == UART5)  ? RCC_APB1ENR_UART5
                                              : 0;
    }
    // If USART peripheral on APB2
    else {
        RCC->APB2RSTR |= (p_usartx == USART1) ? RCC_APB2ENR_USART1 : (p_usartx == USART6) ? RCC_APB2ENR_USART6 : 0;
    }
}

static inline usart_init_port_num_e map_usart_ports_to_num(usart_reg_def const *const p_usartx)
{
    if (p_usartx == USART1) {
        return USART1_INIT_NUM;
    } else if (p_usartx == USART2) {
        return USART2_INIT_NUM;
    } else if (p_usartx == USART3) {
        return USART3_INIT_NUM;
    } else if (p_usartx == UART4) {
        return UART4_INIT_NUM;
    } else if (p_usartx == UART5) {
        return UART5_INIT_NUM;
    } else if (p_usartx == USART6) {
        return USART6_INIT_NUM;
    } else {
        ASSERT(0);
    }
    return 0;
}

static inline bool verify_usart_initialized(usart_reg_def const *const p_usartx)
{
    if (g_usart_port_init & (1 << map_usart_ports_to_num(p_usartx))) {
        return true;
    } else {
        return false;
    }
}

// Sets the baudrate for the given USART peripheral
static void set_baudrate(usart_reg_def *p_usartx, usart_oversampling_e oversampling_mode, usart_baudrate_e baudrate)
{
    uint32_t temp_reg;
    uint32_t clock_freq;
    bus_types usart_bus;

    uint32_t usart_div;
    uint32_t div_mantissa;
    uint32_t div_fraction;
    uint32_t over8;

    // If over8 enabled, it will be 8, otherwise it will be 16
    over8 = (8 * (2 - oversampling_mode));

    // Get clock frequency
    usart_bus  = ((p_usartx == USART1) || (p_usartx == USART6)) ? APB2_BUS : APB1_BUS;
    clock_freq = rcc_get_bus_clock_freq_hz(usart_bus);

    // Calculate mantissa part
    div_mantissa = (clock_freq / (over8 * baudrate));

    // Calculate fraction part
    usart_div    = ((100 * clock_freq) / (over8 * baudrate));
    div_fraction = (usart_div - (div_mantissa * 100));

    // Fraction part must be multiplied by over8 then rounded
    div_fraction = (div_fraction * over8);
    div_fraction = (div_fraction + 50) / 100;

    // If fraction part is too big, carry into mantissa section
    if (div_fraction == 0x10) {
        div_mantissa += 1;
        div_fraction = 0;
    }

    // Set up variable to write into register
    temp_reg = 0;
    temp_reg |= (div_fraction << USART_BRR_FRACTION_POS);
    temp_reg |= (div_mantissa << USART_BRR_MANTISSA_POS);

    // 3rd bit must be kept clear if over8 is used
    if (oversampling_mode == USART_OVERSAMPLING_8) {
        temp_reg &= ~(1 << USART_BRR_OVER8EN_CLEAR_POS);
    }

    p_usartx->BRR = temp_reg;
}

static void transfer_data(usart_handle *p_usart_handle)
{
    p_usart_handle->p_usartx->DR = (uint32_t)*p_usart_handle->usart_it_data.txrx_buffer;
    p_usart_handle->usart_it_data.txrx_length--;
    p_usart_handle->usart_it_data.txrx_buffer++;
}

static void recieve_data(usart_handle *p_usart_handle)
{
    *p_usart_handle->usart_it_data.txrx_buffer = p_usart_handle->p_usartx->DR;
    p_usart_handle->usart_it_data.txrx_length--;
    p_usart_handle->usart_it_data.txrx_buffer++;
}

static inline void enable_interrupts(usart_reg_def *const p_usartx)
{
    usart_it_config(p_usartx, ENABLE);
    uint32_t temp_reg = 0;
    temp_reg |= USART_CR1_UE;
    temp_reg |= USART_CR1_TXEIE;
    temp_reg |= USART_CR1_TCIE;
    temp_reg |= USART_CR1_PEIE;
    p_usartx->CR1 |= temp_reg;
}

static inline void disable_interrupts(usart_reg_def *const p_usartx)
{
    p_usartx->CR1 &= ~USART_CR1_RXNEIE;
    p_usartx->CR1 &= ~USART_CR1_TCIE;
    p_usartx->CR1 &= ~USART_CR1_TXEIE;
    p_usartx->CR1 &= ~USART_CR1_PEIE;
    p_usartx->CR1 &= ~USART_CR1_PCE;
    p_usartx->CR1 &= ~USART_CR1_UE;
}
