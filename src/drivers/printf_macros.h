#ifndef PRINTF_MACROS_H
#define PRINTF_MACROS_H

// NOTE: Used in common/printf and potentially main. May be moved in future
#include "usart.h"

#define PRINTF_USART USART2
#define PRINTF_USART_TX_PORT GPIOA
#define PRINTF_USART_TX_PIN PIN_NO_2

#endif
