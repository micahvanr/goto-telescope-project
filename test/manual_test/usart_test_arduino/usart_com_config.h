#ifndef USART_COM_CONFIG_H
#define USART_COM_CONFIG_H

#include <stdint.h>

typedef enum : uint8_t {
    TEST_USART_TRANSMIT,
    TEST_USART_RECIEVE,
    TEST_USART_TRANSMIT_IT,
    TEST_USART_RECIEVE_IT,

} usart_com_config_options_e;

// Change these enums to set the corresponding option for both Arduino and main board settings
typedef enum : uint32_t {
    USART_COM_SPEED = 115200,
    TEST_USART_MODE = TEST_USART_RECIEVE,
} usart_com_config_sel_e;

#endif 
