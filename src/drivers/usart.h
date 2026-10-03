/*****************************************************************************************
File: usart.h
Overview: Provides functions for initializing, configuring, and using the USART peripheral.
Notes: None
******************************************************************************************/

#ifndef USART_H
#define USART_H

//} Includes
//=========================================//{

#include "stm32f4xx.h"

//======================================================================================//}
//                  Public Constants
//======================================================================================//{

//} Address Definitions
//=========================================//{

typedef enum : uint32_t {
    USART1_BASE_ADDR = ((APB2_BASE_ADDR) + (0x1000U)),
    USART6_BASE_ADDR = ((APB2_BASE_ADDR) + (0x1400U)),
    USART2_BASE_ADDR = ((APB1_BASE_ADDR) + (0x4400U)),
    USART3_BASE_ADDR = ((APB1_BASE_ADDR) + (0x4400U)),
    UART4_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x4400U)),
    UART5_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x4400U)),
} usart_base_addr_e;

//} Config Options
//=========================================//{

typedef enum : uint32_t {
    USART_OVERSAMPLING_16 = 0b0,
    USART_OVERSAMPLING_8  = 0b1,
} usart_oversampling_e;

typedef enum : uint32_t {
    USART_WORD_LENGTH_8_DATA_BITS = 0b0,
    USART_WORD_LENGTH_9_DATA_BITS = 0b1,
} usart_word_length_e;

typedef enum : uint32_t {
    USART_PARITY_CONTROL_DISABLE = 0b0,
    USART_PARITY_CONTROL_ENABLE  = 0b1,
} usart_parity_control_e;

typedef enum : uint32_t {
    USART_PARITY_SEL_EVEN = 0b0,
    USART_PARITY_SEL_ODD  = 0b1,
} usart_parity_select_e;

typedef enum : uint32_t {
    USART_STOP_BITS_1   = 0b00,
    USART_STOP_BITS_0_5 = 0b01,
    USART_STOP_BITS_2   = 0b10,
    USART_STOP_BITS_1_5 = 0b11,
} usart_stop_bits_e;

typedef enum : uint32_t {
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

//} API Function Argument/Return Options
//=========================================//{

//} Callback Options
//=========================================//{

typedef enum : uint8_t {
    USART_EVENT_CMPLT,
    USART_EVENT_ERROR,
} usart_event_e;

//} Interrupt Handler Constants
//=========================================//{

typedef enum : uint8_t {
    USART_MODE_NONE,
    USART_MODE_TX,
    USART_MODE_RX,
} usart_mode_e;

typedef enum : uint8_t {
    USART_STATUS_READY = 0,
    USART_STATUS_RUNNING,
} usart_status_e;

//} Misc. Constants
//=========================================//{

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

typedef volatile struct {
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
    volatile usart_status_e status; // Default: USART_STATUS_READY
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

#define USART1 ((usart_reg_def *) USART1_BASE_ADDR)
#define USART2 ((usart_reg_def *) USART2_BASE_ADDR)
#define USART3 ((usart_reg_def *) USART3_BASE_ADDR)
#define UART4  ((usart_reg_def *) UART4_BASE_ADDR)
#define UART5  ((usart_reg_def *) UART5_BASE_ADDR)
#define USART6 ((usart_reg_def *) USART6_BASE_ADDR)

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

// Init and reset
void usart_init(usart_handle *const p_usart_handle);
void usart_reset(usart_reg_def const *const p_usartx);

// Write/read blocking or polling
void usart_transmit(usart_reg_def *const p_usartx, uint8_t const *p_data, uint32_t length);
void usart_receive(usart_reg_def *const p_usartx, uint8_t *p_data, uint32_t length);
// Used for printf
void usart_transmit_single_byte(usart_reg_def *const p_usartx, uint8_t p_data);

// Write/read interrupt
void usart_transmit_it(usart_handle *const p_usart_handle, uint8_t *const p_data, uint32_t length);
void usart_receive_it(usart_handle *p_usart_handle, uint8_t *p_data, uint32_t length);

// Interrupt handling
void usart_config_it(usart_reg_def *const p_usartx, togglable_e const toggle);
void usart_it_handler(usart_handle *const p_usart_handle);

// User implement callback function to handle.
void __weak usart_callback(usart_handle *p_usart_handle, usart_event_e event);

#endif // USART_H

// *End of file*
