#include "syscfg.h"
#include "rcc.h"
#include "stm32f4xx.h"

// * Helper Function Prototypes
//======================================================================================//}
//                  Helper Function Protoypes
//======================================================================================//{

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

//======================================================================================//}
//                  Peripheral Function API Implementation
//======================================================================================//{

/***************************************************************************
Function: syscfg_clock_enable
Overview: Enables the SYSCFG clock inside RCC
Parameters: 
    None
Return: 
    None
Note: None
***************************************************************************/
void syscfg_clock_enable(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

/***************************************************************************
Function: syscfg_exti_config
Overview: Configures the GPIO EXTI interrupt lines
Parameters: 
    port_num: Number attributed to GPIO port. Use map_gpio_ports_to_num to retreive number or use map below
        GPIOA [0]
        GPIOB [1]
        GPIOC [2]
        GPIOD [3]
        GPIOE [4]
        GPIOF [5]
        GPIOG [6]
        GPIOH [7]
    exti_line: EXTI line number ie. the pin number of the GPIO pin
        0-15
    toggle:
        ENABLE
        DISABLE
Return: 
    None
Note: None
***************************************************************************/
void syscfg_exti_config(uint8_t port_num, uint8_t exti_line, togglable_e toggle)
{
    uint8_t reg_num = exti_line % 4;
    // shift amount is exti x width multiplied by 0-3
    uint8_t shift_amount = 4 * (exti_line % 4);

    switch (toggle) {
    case ENABLE:  SYSCFG->EXTICR[reg_num] |= port_num << shift_amount; break;
    case DISABLE: SYSCFG->EXTICR[reg_num] &= ~(port_num << shift_amount); break;
    }
}

//======================================================================================//}
//                  Helper Function Implementation
//======================================================================================//{
