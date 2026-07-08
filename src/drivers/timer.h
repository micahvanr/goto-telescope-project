#ifndef TIMER_H
#define TIMER_H

#include "stm32f4xx.h"

// General Purpose Timers: TIM2 - TIM5
// General Purpose Timers: TIM9 - TIM14
// Basic Timers: TIM6 & TIM7
// Advanced Control Timers: TIM1 & TIM8

//======================================================================================//
//                  Address Definitions
//======================================================================================//

typedef enum {
    TIM1_BASE_ADDR = ((APB2_BASE_ADDR) + (0x0000u)),

    TIM2_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0000u)),
    TIM3_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0400u)),
    TIM4_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0800u)),
    TIM5_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0C00u)),
    TIM6_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x1000u)),
    TIM7_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x1400u)),
    TIM12_BASE_ADDR = ((APB1_BASE_ADDR) + (0x1800u)),
    TIM13_BASE_ADDR = ((APB1_BASE_ADDR) + (0x1C00u)),
    TIM14_BASE_ADDR = ((APB1_BASE_ADDR) + (0x2000u)),

    TIM8_BASE_ADDR  = ((APB2_BASE_ADDR) + (0x0400u)),
    TIM9_BASE_ADDR  = ((APB2_BASE_ADDR) + (0x4000u)),
    TIM10_BASE_ADDR = ((APB2_BASE_ADDR) + (0x4400u)),
    TIM11_BASE_ADDR = ((APB2_BASE_ADDR) + (0x4800u)),
} timer_base_addr_e;

//======================================================================================//
//                  Macros and Other Enums
//======================================================================================//

//======================================================================================//
//                  Register Structure Definitions
//======================================================================================//

// Peripheral structure definitions

// Function API prototypes
#endif
