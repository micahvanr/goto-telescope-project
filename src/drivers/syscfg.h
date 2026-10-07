/*****************************************************************************************
File: syscfg.h
Overview: Provides functions for using the SYSCFG peripheral.
Features:
    EXTI interrupt configuration
Notes: None
******************************************************************************************/

#ifndef SYSCFG_H
#define SYSCFG_H

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
    SYSCFG_BASE_ADDR = ((APB2_BASE_ADDR) + (0x3800U)),
} syscfg_base_addr_e;

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

typedef volatile struct {
    uint32_t MEMRM;      // memory remap register                            Offset: 0x0
    uint32_t PMC;        // peripheral mode configuration register           Offset: 0x4
    uint32_t EXTICR[4];  // external interrupt configuration register       Offset: 0x8
    uint32_t reserved_1; // Reserved 0x18
    uint32_t reserved_2; // Reserved 0x1C
    uint32_t CMPCR;      // Compensation cell control register               Offset: 0x20
} syscfg_reg_def;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define SYSCFG ((syscfg_reg_def *) SYSCFG_BASE_ADDR)

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

void syscfg_config_exti(uint8_t port_num, uint8_t exti_line, togglable_e toggle);

#endif // SYSCFG_H

// *End of file*
