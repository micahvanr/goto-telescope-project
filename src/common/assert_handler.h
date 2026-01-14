#ifndef ASSERT_HANDLER
#define ASSERT_HANDLER

#include "../drivers/gpio.h"
#include "../drivers/stm32f4xx.h"

#define ASSERT(expression)    \
    do {                      \
        if (!(expression)) {  \
            assert_handler(); \
        }                     \
    } while (0)

void assert_handler(void);

#endif