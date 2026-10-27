//======================================================================================//{
//  File: debug_tools.c
//  Overview: Provide functions that are helpful in debugging.
//  Features:
//      Debug pin
//      Debug button
//  Notes:
//======================================================================================//}

#ifndef DEBUG_TOOLS_H
#define DEBUG_TOOLS_H

//} Include Statements
//=========================================//{

#include "stm32f4xx.h"

//======================================================================================//}
//                  Public Constants
//======================================================================================//{

#define DEBUG_PIN_PORT       GPIOA
#define DEBUG_PIN_NUM        PIN_NUM_3
#define DEBUG_BUTTON_PORT    GPIOA
#define DEBUG_BUTTUN_PIN_NUM PIN_NUM_0

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

void debug_pin_init(void);
void toggle_debug_pin(void);

void debug_button_init(void);
pin_logic_level_e read_debug_button(void);

#endif

// *End of file*
