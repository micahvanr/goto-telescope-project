#include "pwr.h"
#include "common.h"
#include "rcc.h"
#include "stm32f4xx.h"

//======================================================================================//}
//                  Helper Function Prototypes
//======================================================================================//{

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

//======================================================================================//}
//                  Peripheral Function API Implementation
//======================================================================================//{

/***************************************************************************
Function: pwr_config_clock 
Overview: Enables or disables the clock to the PWR peripheral.
Parameters: 
    toggle:
        ENABLE
        DISABLE
Return: 
    None
Note: None
***************************************************************************/
void pwr_config_clock(togglable_e toggle)
{
    switch (toggle) {
        case ENABLE:
            RCC->APB1ENR |= RCC_APB1ENR_PWREN;
            break;
        case DISABLE:
            RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;
            break;
    }
}

/***************************************************************************
Function: pwr_disable_backup_write_prot
Overview: Disables the write protection for the RTC Backup registers and backup SRAM.
Parameters: 
    None
Return: 
    None
Note: None
***************************************************************************/
void pwr_disable_backup_write_prot(void)
{
    uint32_t dummy_read;
    PWR->CR |= PWR_CR_DBP;
    dummy_read = PWR->CR;
    UNUSED(dummy_read);
}

//======================================================================================//}
//                  Helper Function Implementation
//======================================================================================//{
