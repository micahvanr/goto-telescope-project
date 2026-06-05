#include "debug_pin.h"
#include "gpio.h"

void debug_pin_init(void)
{
    gpio_handle debug_pin            = {0};
    debug_pin.p_gpiox                = DEBUG_PIN_PORT;
    debug_pin.gpio_conf.alt_fn_no    = GPIO_ALT_FN_NA;
    debug_pin.gpio_conf.it_trigger   = GPIO_IT_NA;
    debug_pin.gpio_conf.mode         = GPIO_MODE_OUTPUT;
    debug_pin.gpio_conf.output_speed = GPIO_OSPEED_FAST;
    debug_pin.gpio_conf.output_type  = GPIO_OPTYPE_PUSH_PULL;
    debug_pin.gpio_conf.pin_no       = DEBUG_PIN_NO;

    gpio_init(&debug_pin);
}

void toggle_debug_pin(void)
{
    gpio_toggle(DEBUG_PIN_PORT, DEBUG_PIN_NO);
}
