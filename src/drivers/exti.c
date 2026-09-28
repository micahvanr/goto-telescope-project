#include "exti.h"

//======================================================================================//}
//                  Peripheral Function API Implementation
//======================================================================================//{

/***************************************************************************
Function: exti_it_config
Overview: Enables or disable the interrupts for the specified EXTI line
Parameters: 
    line_no:
        EXTI_LINE_NO_x (0-22)
    toggle:
        ENABLE  [1]
        DISABLE [0]
Return: 
    None
Note: None
***************************************************************************/
void exti_it_config(exti_lines_e line_no, togglable_e toggle)
{
    switch (toggle) {
    case ENABLE:  EXTI->IMR |= (1 << line_no); break;
    case DISABLE: EXTI->IMR &= ~(1 << line_no); break;
    }
}

/***************************************************************************
Function: exti_it_config
Overview: Enables or disables the events for the specified EXTI line
Parameters: 
    line_no:
        EXTI_LINE_NO_x (0-22)
    toggle:
        ENABLE  [1]
        DISABLE [0]
Return: 
    None
Note: None
***************************************************************************/
void exti_event_config(exti_lines_e line_no, togglable_e toggle)
{
    switch (toggle) {
    case ENABLE:  EXTI->EMR |= (1 << line_no); break;
    case DISABLE: EXTI->EMR &= ~(1 << line_no); break;
    }
}

/***************************************************************************
Function: exti_rising_edge_config
Overview: Enables or disables the rising edge trigger for the specified EXTI line
Parameters: 
    line_no:
        EXTI_LINE_NO_x (0-22)
    toggle:
        ENABLE  [1]
        DISABLE [0]
Return: 
    None
Note: None
***************************************************************************/
void exti_rising_edge_config(exti_lines_e line_no, togglable_e toggle)
{
    switch (toggle) {
    case ENABLE:  EXTI->RTSR |= (1 << line_no); break;
    case DISABLE: EXTI->RTSR &= ~(1 << line_no); break;
    }
}

/***************************************************************************
Function: exti_falling_edge_config
Overview: Enables or disables the falling edge trigger for the specified EXTI line
Parameters: 
    line_no:
        EXTI_LINE_NO_x (0-22)
    toggle:
        ENABLE  [1]
        DISABLE [0]
Return: 
    None
Note: None
***************************************************************************/
void exti_falling_edge_config(exti_lines_e line_no, togglable_e toggle)
{
    switch (toggle) {
    case ENABLE:  EXTI->FTSR |= (1 << line_no); break;
    case DISABLE: EXTI->FTSR &= ~(1 << line_no); break;
    }
}

/***************************************************************************
Function: exti_software_it_event 
Overview: Triggers a software interrupt/event for the specified EXTI line
Parameters: 
    line_no:
        EXTI_LINE_NO_x (0-22)
Return: 
    None
Note: None
***************************************************************************/
void exti_software_it_event(exti_lines_e line_no)
{
    EXTI->SWIER |= (1 << line_no);
}

/***************************************************************************
Function: exti_get_pending 
Overview: Returns whether the EXTI line has a pending trigger
Parameters: 
    line_no:
        EXTI_LINE_NO_x (0-22)
Return: 
    exti_pend_status_e 
        EXTI_PEND_STATUS_NOT_TRIGGERED 
        EXTI_PEND_STATUS_TRIGGERED     
Note: None
***************************************************************************/
exti_pend_status_e exti_get_pending(exti_lines_e line_no)
{
    return 0b1 & (EXTI->SWIER >> line_no);
}

/***************************************************************************
Function: exti_clear_pending 
Overview: Clears the specified EXTI line's pending trigger
Parameters: 
    line_no:
        EXTI_LINE_NO_x (0-22)
Return: 
    None
Note: None
***************************************************************************/
void exti_clear_pending(exti_lines_e line_no)
{
    EXTI->PR |= (1 << line_no);
}
