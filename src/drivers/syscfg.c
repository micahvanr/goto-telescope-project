/*****************************************************************************************
File: syscfg.c
Overview: Provides functions for using the SYSCFG peripheral.
Features:
    EXTI interrupt configuration
Notes: None
******************************************************************************************/

//} Include Statements
//=========================================//{

#include "syscfg.h"

//========================================================//}
//          Register Constants
//========================================================//{

// clang-format off
typedef enum : uint32_t {
    SYSCFG_MEMRM_MEM_MODE_POS = 0U,   // MEM_MODE
} syscfg_memrm_pos_e;

typedef enum : uint32_t {
    SYSCFG_MEMRM_MEM_MODE = (1U << SYSCFG_MEMRM_MEM_MODE_POS),   // MEM_MODE
} syscfg_memrm_e;

typedef enum : uint32_t {
    SYSCFG_MEMRM_MEM_MODE_MASK = 0b11U,   // 2 bit(s)
} syscfg_memrm_mask_e;

typedef enum : uint32_t {
    SYSCFG_PMC_MII_RMII_SEL_POS = 23U,   // Ethernet PHY interface selection
} syscfg_pmc_pos_e;

typedef enum : uint32_t {
    SYSCFG_PMC_MII_RMII_SEL = (1U << SYSCFG_PMC_MII_RMII_SEL_POS),   // Ethernet PHY interface selection
} syscfg_pmc_e;

typedef enum : uint32_t {
    SYSCFG_PMC_MII_RMII_SEL_MASK = 0b1U,   // 1 bit(s)
} syscfg_pmc_mask_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR1_EXTI3_POS = 12U,   // EXTI x configuration (x = 0 to 3)
    SYSCFG_EXTICR1_EXTI2_POS = 8U,    // EXTI x configuration (x = 0 to 3)
    SYSCFG_EXTICR1_EXTI1_POS = 4U,    // EXTI x configuration (x = 0 to 3)
    SYSCFG_EXTICR1_EXTI0_POS = 0U,    // EXTI x configuration (x = 0 to 3)
} syscfg_exticr1_pos_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR1_EXTI3 = (1U << SYSCFG_EXTICR1_EXTI3_POS),   // EXTI x configuration (x = 0 to 3)
    SYSCFG_EXTICR1_EXTI2 = (1U << SYSCFG_EXTICR1_EXTI2_POS),   // EXTI x configuration (x = 0 to 3)
    SYSCFG_EXTICR1_EXTI1 = (1U << SYSCFG_EXTICR1_EXTI1_POS),   // EXTI x configuration (x = 0 to 3)
    SYSCFG_EXTICR1_EXTI0 = (1U << SYSCFG_EXTICR1_EXTI0_POS),   // EXTI x configuration (x = 0 to 3)
} syscfg_exticr1_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR1_EXTI3_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR1_EXTI2_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR1_EXTI1_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR1_EXTI0_MASK = 0b1111U,   // 4 bit(s)
} syscfg_exticr1_mask_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR2_EXTI7_POS = 12U,   // EXTI x configuration (x = 4 to 7)
    SYSCFG_EXTICR2_EXTI6_POS = 8U,    // EXTI x configuration (x = 4 to 7)
    SYSCFG_EXTICR2_EXTI5_POS = 4U,    // EXTI x configuration (x = 4 to 7)
    SYSCFG_EXTICR2_EXTI4_POS = 0U,    // EXTI x configuration (x = 4 to 7)
} syscfg_exticr2_pos_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR2_EXTI7 = (1U << SYSCFG_EXTICR2_EXTI7_POS),   // EXTI x configuration (x = 4 to 7)
    SYSCFG_EXTICR2_EXTI6 = (1U << SYSCFG_EXTICR2_EXTI6_POS),   // EXTI x configuration (x = 4 to 7)
    SYSCFG_EXTICR2_EXTI5 = (1U << SYSCFG_EXTICR2_EXTI5_POS),   // EXTI x configuration (x = 4 to 7)
    SYSCFG_EXTICR2_EXTI4 = (1U << SYSCFG_EXTICR2_EXTI4_POS),   // EXTI x configuration (x = 4 to 7)
} syscfg_exticr2_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR2_EXTI7_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR2_EXTI6_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR2_EXTI5_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR2_EXTI4_MASK = 0b1111U,   // 4 bit(s)
} syscfg_exticr2_mask_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR3_EXTI11_POS = 12U,   // EXTI x configuration (x = 8 to 11)
    SYSCFG_EXTICR3_EXTI10_POS = 8U,    // EXTI10
    SYSCFG_EXTICR3_EXTI9_POS  = 4U,    // EXTI x configuration (x = 8 to 11)
    SYSCFG_EXTICR3_EXTI8_POS  = 0U,    // EXTI x configuration (x = 8 to 11)
} syscfg_exticr3_pos_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR3_EXTI11 = (1U << SYSCFG_EXTICR3_EXTI11_POS),   // EXTI x configuration (x = 8 to 11)
    SYSCFG_EXTICR3_EXTI10 = (1U << SYSCFG_EXTICR3_EXTI10_POS),   // EXTI10
    SYSCFG_EXTICR3_EXTI9  = (1U << SYSCFG_EXTICR3_EXTI9_POS),    // EXTI x configuration (x = 8 to 11)
    SYSCFG_EXTICR3_EXTI8  = (1U << SYSCFG_EXTICR3_EXTI8_POS),    // EXTI x configuration (x = 8 to 11)
} syscfg_exticr3_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR3_EXTI11_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR3_EXTI10_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR3_EXTI9_MASK  = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR3_EXTI8_MASK  = 0b1111U,   // 4 bit(s)
} syscfg_exticr3_mask_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR4_EXTI15_POS = 12U,   // EXTI x configuration (x = 12 to 15)
    SYSCFG_EXTICR4_EXTI14_POS = 8U,    // EXTI x configuration (x = 12 to 15)
    SYSCFG_EXTICR4_EXTI13_POS = 4U,    // EXTI x configuration (x = 12 to 15)
    SYSCFG_EXTICR4_EXTI12_POS = 0U,    // EXTI x configuration (x = 12 to 15)
} syscfg_exticr4_pos_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR4_EXTI15 = (1U << SYSCFG_EXTICR4_EXTI15_POS),   // EXTI x configuration (x = 12 to 15)
    SYSCFG_EXTICR4_EXTI14 = (1U << SYSCFG_EXTICR4_EXTI14_POS),   // EXTI x configuration (x = 12 to 15)
    SYSCFG_EXTICR4_EXTI13 = (1U << SYSCFG_EXTICR4_EXTI13_POS),   // EXTI x configuration (x = 12 to 15)
    SYSCFG_EXTICR4_EXTI12 = (1U << SYSCFG_EXTICR4_EXTI12_POS),   // EXTI x configuration (x = 12 to 15)
} syscfg_exticr4_e;

typedef enum : uint32_t {
    SYSCFG_EXTICR4_EXTI15_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR4_EXTI14_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR4_EXTI13_MASK = 0b1111U,   // 4 bit(s)
    SYSCFG_EXTICR4_EXTI12_MASK = 0b1111U,   // 4 bit(s)
} syscfg_exticr4_mask_e;

typedef enum : uint32_t {
    SYSCFG_CMPCR_READY_POS  = 8U,   // READY
    SYSCFG_CMPCR_CMP_PD_POS = 0U,   // Compensation cell power-down
} syscfg_cmpcr_pos_e;

typedef enum : uint32_t {
    SYSCFG_CMPCR_READY  = (1U << SYSCFG_CMPCR_READY_POS),    // READY
    SYSCFG_CMPCR_CMP_PD = (1U << SYSCFG_CMPCR_CMP_PD_POS),   // Compensation cell power-down
} syscfg_cmpcr_e;

typedef enum : uint32_t {
    SYSCFG_CMPCR_READY_MASK  = 0b1U,   // 1 bit(s)
    SYSCFG_CMPCR_CMP_PD_MASK = 0b1U,   // 1 bit(s)
} syscfg_cmpcr_mask_e;

// clang-format on

//======================================================================================//}
//                  Public Function Implementation
//======================================================================================//{

/***************************************************************************
Function: syscfg_config_exti
Overview: Configures the GPIO EXTI interrupt lines.
Parameters: 
    port_num: Number attributed to GPIO port. Use map_gpio_ports_to_num to retreive number or use map below.
        GPIOA [0]
        GPIOB [1]
        GPIOC [2]
        GPIOD [3]
        GPIOE [4]
        GPIOF [5]
        GPIOG [6]
        GPIOH [7]
    exti_line: EXTI line number ie. the pin number of the GPIO pin.
        0-15
    toggle:
        ENABLE
        DISABLE
Return: 
    None
Note: None
***************************************************************************/
void syscfg_config_exti(uint8_t port_num, uint8_t exti_line, togglable_e toggle)
{
    uint8_t reg_num = exti_line % 4;
    // shift amount is exti x width multiplied by 0-3.
    uint8_t shift_amount = 4 * (exti_line % 4);

    switch (toggle)
    {
        case ENABLE:
            SYSCFG->EXTICR[reg_num] |= port_num << shift_amount;
            break;
        case DISABLE:
            SYSCFG->EXTICR[reg_num] &= ~(port_num << shift_amount);
            break;
    }
}

// *End of file*
