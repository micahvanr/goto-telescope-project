//======================================================================================//{
//  File: nvic.c
//  Overview: Provides functions for using the NVIC peripheral. Used to configure interrupts in
//      the processor.
//  Features:
//      Configure interrupts
//      Configure priority of interrupts
//  Notes: None
//======================================================================================//{

//} Include Statements
//=========================================//{

#include "nvic.h"
#include <stdint.h>

//======================================================================================//}
//                  Public Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: nvic_toggable_irq
//  Overview: Enable or disable the specified IRQ number inside the NVIC.
//  Parameters:
//      irq_num: IRQ number that needs to be enabled or disables.
//          See irq_number_e enum
//      toggle:
//          ENABLE
//          DISABLE
//  Return: None
//  Note: None
//============================================================================//}
void nvic_toggable_irq(irq_number_e const irq_num, togglable_e const toggle)
{
    uint32_t volatile *p_nvic_iser = (uint32_t *) NVIC_ISER_BASE_ADDR;
    uint32_t volatile *p_nvic_icer = (uint32_t *) NVIC_ICER_BASE_ADDR;

    uint8_t nvic_it_reg  = (irq_num / 32);
    uint32_t nvic_it_bit = (irq_num % 32);

    switch (toggle)
    {
        case ENABLE:
            p_nvic_iser += nvic_it_reg;
            *p_nvic_iser |= (1 << nvic_it_bit); // 1000000
            break;
        case DISABLE:
            p_nvic_icer += nvic_it_reg;
            *p_nvic_icer |= (1 << nvic_it_bit);
            break;
    }
}

//============================================================================//{
//  Function: nvim_set_irq_priority
//  Overview: .
//  Parameters:
//       irq_num: IRQ to modify the priority number.
//           See irq_number_e enum
//      irq_pri: IRQ priority number -> lower number means higher priority.
//         IRQ_PRIORITY_x (0-15)
//  Return: None
//  Note: Only 4 of the 8 bits are implemented in the stm32f4xx MCUs so 16 levels or priority.
//============================================================================//}
void nvim_set_irq_priority(irq_number_e const irq_num, irq_priority_e const irq_pri)
{
    uint32_t volatile *p_nvic_ipr = (uint32_t *) NVIC_IPR_BASE_ADDR;

    uint8_t nvic_pri_reg       = (irq_num / 4);
    uint32_t nvic_pri_field    = (irq_num % 4);
    uint32_t nvic_shift_amount = ((nvic_pri_field * 8) + (8 - IRQ_NUM_PRIORITY_BITS_IMPLMENTED));

    p_nvic_ipr += nvic_pri_reg;
    *p_nvic_ipr |= (irq_pri << nvic_shift_amount);
}

// *End of file*
