/*****************************************************************************************
File: exti.h
Overview: Provides functions for initializing, configuring, and using the EXTI peripheral.
    Used to configure GPIO interrupts and other external interrupts and events.
Features:
    Configure interrupts
    Configure events
    Configure rising edge
    Configure falling edge
    Trigger software interrupt events
    Check pending status
    Clear pending status
Notes: EXTI are connected to GPIO pins and other external sources.
******************************************************************************************/

#ifndef EXTI_H
#define EXTI_H

//} Include Statements
//=========================================//{

#include "stm32f4xx.h"
#include <stdint.h>

//======================================================================================//}
//                  Public Constants
//======================================================================================//{

//} Address Definitions
//=========================================//{

typedef enum {
    EXTI_BASE_ADDR = ((APB2_BASE_ADDR) + (0x3C00U)),
} exti_base_addr_e;

//} API Function Argument/Return Options
//=========================================//{

typedef enum : uint32_t {
    EXTI_LINE_NUM_0  = 0,
    EXTI_LINE_NUM_1  = 1,
    EXTI_LINE_NUM_2  = 2,
    EXTI_LINE_NUM_3  = 3,
    EXTI_LINE_NUM_4  = 4,
    EXTI_LINE_NUM_5  = 5,
    EXTI_LINE_NUM_6  = 6,
    EXTI_LINE_NUM_7  = 7,
    EXTI_LINE_NUM_8  = 8,
    EXTI_LINE_NUM_9  = 9,
    EXTI_LINE_NUM_10 = 10,
    EXTI_LINE_NUM_11 = 11,
    EXTI_LINE_NUM_12 = 12,
    EXTI_LINE_NUM_13 = 13,
    EXTI_LINE_NUM_14 = 14,
    EXTI_LINE_NUM_15 = 15,
    EXTI_LINE_NUM_16 = 16,
    EXTI_LINE_NUM_17 = 17,
    EXTI_LINE_NUM_18 = 18,
    EXTI_LINE_NUM_19 = 19,
    EXTI_LINE_NUM_20 = 20,
    EXTI_LINE_NUM_21 = 21,
    EXTI_LINE_NUM_22 = 22,
} exti_lines_e;

// Applications:
// - EXTI PR
typedef enum : uint32_t {
    EXTI_PEND_STATUS_NOT_TRIGGERED = 0b0,
    EXTI_PEND_STATUS_TRIGGERED     = 0b1,
} exti_pend_status_e;

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

typedef volatile struct {
    uint32_t IMR;   // Interrupt mask register                             Offset: 0x0
    uint32_t EMR;   // Event mask register                                 Offset: 0x4
    uint32_t RTSR;  // Rising Trigger selection register                   Offset: 0x8
    uint32_t FTSR;  // Falling Trigger selection register                  Offset: 0xC
    uint32_t SWIER; // Software interrupt event register                   Offset: 0x10
    uint32_t PR;    // Pending register                                    Offset: 0x14
} exti_reg_def;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define EXTI ((exti_reg_def *) EXTI_BASE_ADDR)

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

void exti_config_it(exti_lines_e line_no, togglable_e toggle);
void exti_config_event(exti_lines_e line_no, togglable_e toggle);
void exti_config_rising_edge(exti_lines_e line_no, togglable_e toggle);
void exti_config_falling_edge(exti_lines_e line_no, togglable_e toggle);
void exti_trigger_software_it_event(exti_lines_e line_no);
exti_pend_status_e exti_get_pending(exti_lines_e line_no);
void exti_clear_pending(exti_lines_e line_no);

#endif // EXTI_H

// *End of file*
