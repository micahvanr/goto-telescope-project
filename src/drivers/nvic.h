//======================================================================================//{
//  File: nvic.h
//  Overview: Provides functions for using the NVIC peripheral. Used to configure interrupts in
//      the processor.
//  Features:
//      Configure interrupts
//      Configure priority of interrupts
//  Notes: None
//======================================================================================//{

#ifndef NVIC_H
#define NVIC_H

//} Include Statements
//=========================================//{

#include "stm32f4xx.h"

//======================================================================================//}
//                  Public Constants
//======================================================================================//{

//} Address Definitions
//=========================================//{

typedef enum {
    NVIC_ISER_BASE_ADDR = (0xE000E100ul),
    NVIC_ICER_BASE_ADDR = (0xE000E180ul),
    NVIC_ISPR_BASE_ADDR = (0xE000E200ul),
    NVIC_ICPR_BASE_ADDR = (0xE000E280ul),
    NVIC_IABR_BASE_ADDR = (0xE000E300ul),
    NVIC_IPR_BASE_ADDR  = (0xE000E400ul),
} nvic_base_addr_e;

//} Misc. Constants
//=========================================//{

// Lower number means higher priority
typedef enum {
    IRQ_PRIORITY_0                   = 0,
    IRQ_PRIORITY_1                   = 1,
    IRQ_PRIORITY_2                   = 2,
    IRQ_PRIORITY_3                   = 3,
    IRQ_PRIORITY_4                   = 4,
    IRQ_PRIORITY_5                   = 5,
    IRQ_PRIORITY_6                   = 6,
    IRQ_PRIORITY_7                   = 7,
    IRQ_PRIORITY_8                   = 8,
    IRQ_PRIORITY_9                   = 9,
    IRQ_PRIORITY_10                  = 10,
    IRQ_PRIORITY_11                  = 11,
    IRQ_PRIORITY_12                  = 12,
    IRQ_PRIORITY_13                  = 13,
    IRQ_PRIORITY_14                  = 14,
    IRQ_PRIORITY_15                  = 15,
    IRQ_NUM_PRIORITY_BITS_IMPLMENTED = 4,
} irq_priority_e;

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

void nvic_irq_config(irq_number_e irq_num, togglable_e toggle);
void nvim_set_irq_priority(irq_number_e irq_num, irq_priority_e irq_pri);

#endif // NVIC_H

// *End of file*
