/*****************************************************************************************
File: pwr.h
Overview: Provides functions for using the PWR peripheral.
Features:
    Disabling backup write protection
Notes: None
******************************************************************************************/

#ifndef PWR_H
#define PWR_H

//} Include Statements
//=========================================//{

#include "stm32f4xx.h"

//======================================================================================//}
//                  Public Constants
//======================================================================================//{

//} Address Definitions
//=========================================//{

typedef enum {
    PWR_BASE_ADDR = ((APB1_BASE_ADDR) + (0x7000U))
} pwr_base_addr_e;

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

typedef volatile struct {
    uint32_t CR;  // power control register            Offset: 0x0
    uint32_t CSR; // power control/status register     Offset: 0x4
} pwr_reg_def;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define PWR ((pwr_reg_def *) PWR_BASE_ADDR)

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

void pwr_disable_backup_write_prot(void);

#endif // PWR_H

// *End of file*
