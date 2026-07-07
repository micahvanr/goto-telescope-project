#ifndef DEBUG_PIN_H
#define DEBUG_PIN_H

#include "stm32f4xx.h"

#define DEBUG_PIN_PORT      GPIOA
#define DEBUG_PIN_NO        PIN_NO_3
#define DEBUG_BUTTON_PORT   GPIOA
#define DEBUG_BUTTUN_PIN_NO PIN_NO_0

void debug_pin_init(void);
void toggle_debug_pin(void);

void debug_button_init(void);
pin_logic_level_e read_debug_button(void);

#endif
