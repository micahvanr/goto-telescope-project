#include "debug_tools.h"
#include "gpio.h"

/***************************************************************************
Function: debug_pin_init
Overview: Initializes the debug GPIO pin (defined in header)
Parameters: 
    None
Return: 
    None
Note: None
***************************************************************************/
void debug_pin_init(void)
{
    gpio_handle debug_pin      = {0};
    debug_pin.p_gpiox          = DEBUG_PIN_PORT;
    debug_pin.gpio_conf.pin_no = DEBUG_PIN_NO;

    debug_pin.gpio_conf.alt_fn_no    = GPIO_ALT_FN_NA;
    debug_pin.gpio_conf.it_trigger   = GPIO_IT_NA;
    debug_pin.gpio_conf.mode         = GPIO_MODE_OUTPUT;
    debug_pin.gpio_conf.output_speed = GPIO_OSPEED_FAST;
    debug_pin.gpio_conf.output_type  = GPIO_OPTYPE_PUSH_PULL;

    gpio_init(&debug_pin);
}

/***************************************************************************
Function: toggle_debug_pin
Overview: Toggles the debug pin (defined in header)
Parameters:
    None
Return: 
    None
Note: None
***************************************************************************/
void toggle_debug_pin(void)
{
    gpio_toggle(DEBUG_PIN_PORT, DEBUG_PIN_NO);
}

/***************************************************************************
Function: debug_button_init
Overview: Initializes the debug button GPIO pin (defined in header)
Parameters: 
    None
Return: 
    None
Note: None
***************************************************************************/
void debug_button_init(void)
{
    gpio_handle debug_button      = {0};
    debug_button.p_gpiox          = DEBUG_BUTTON_PORT;
    debug_button.gpio_conf.pin_no = DEBUG_BUTTUN_PIN_NO;

    debug_button.gpio_conf.alt_fn_no       = GPIO_ALT_FN_NA;
    debug_button.gpio_conf.it_trigger      = GPIO_IT_NA;
    debug_button.gpio_conf.mode            = GPIO_MODE_INPUT;
    debug_button.gpio_conf.pullup_pulldown = GPIO_PUPD_NO;

    gpio_init(&debug_button);
}

/***************************************************************************
Function: read_debug_button
Overview: Reads the pin that is connected to the button on the Discovery board 
Parameters:
    None
Return: 
    pin_logic_level_e: Level of the pin connected to the button
        HIGH (1) 
        LOW (0)
Note: Connected to pull down resistor:
    Button pressed -> returns 1
    Button not pressed -> returns 0
***************************************************************************/
pin_logic_level_e read_debug_button(void)
{
    return gpio_read(DEBUG_BUTTON_PORT, DEBUG_BUTTUN_PIN_NO);
}
