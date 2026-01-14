#include "stm32f4xx.h"

/****************************************************************************************************
                                    General MCU API Function Implementation
****************************************************************************************************/

/***************************************************************************
Function: irq_config
Overview: Takes an IRQ number and enables or disables it inside the NVIC or processor of MCU
Parameters:
    irq_num - IRQ number that needs to be enabled or disables
    toggle  - ENABLE or DISABLE
Return: None
Note: None
***************************************************************************/
void irq_config(irq_number_e irq_num, togglable_e toggle)
{
    uint32_t volatile *p_nvic_iser = (uint32_t *)NVIC_ISER_BASE_ADDR;
    uint32_t volatile *p_nvic_icer = (uint32_t *)NVIC_ICER_BASE_ADDR;

    uint8_t nvic_it_reg  = (irq_num / 32);
    uint32_t nvic_it_bit = (irq_num % 32);

    switch (toggle) {
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

/***************************************************************************
Function: irq_priority
Overview: Takes an IRQ number and configures the priority of the IRQ 
Parameters:
    irq_num - IRQ number to modify the priority of
    irq_pri - IRQ priority number -> lower number means higher priority
Return: None
Note: Only 4 of the 8 bits are implemented in the stm32f4xx MCUs so 16 levels or priority
***************************************************************************/
void irq_priority(irq_number_e irq_num, irq_priority_e irq_pri)
{
    uint32_t volatile *p_nvic_ipr = (uint32_t *)NVIC_IPR_BASE_ADDR;

    uint8_t nvic_pri_reg       = (irq_num / 4);
    uint32_t nvic_pri_field    = (irq_num % 4);
    uint32_t nvic_shift_amount = ((nvic_pri_field * 8) + (8 - IRQ_NUM_PRIORITY_BITS_IMPLMENTED));

    p_nvic_ipr += nvic_pri_reg;
    *p_nvic_ipr |= (irq_pri << nvic_shift_amount);
}