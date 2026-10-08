//======================================================================================//{
//  File: gpio_test.c
//  Overview: Tests some of the GPIO functionality.
//  Tests:
//      - Blink led by toggling
//      - Turn on led if button is pressed
//      - Toggle led when button is pressed using interrupts
//  Notes: None
//======================================================================================//}

//} Includes
//=========================================//{

#include "gpio_test.h"
#include "gpio.h"
#include "stm32f4xx.h"
#include <stdbool.h>

//======================================================================================//}
//                  Public Test Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: test_gpio_blink_led
//  Overview: Blink led by toggling it periodically.
//  Test goals:
//      - GPIO initialization:
//          - Push pull
//          - Output mode
//          - Pin num 15
//      - Toggle GPIO pin function
//  Setup:
//      None
//  Expectations: Blue led should be toggled repeatedly relatively quickly.
//  Note: None
//============================================================================//}
void test_gpio_blink_led(void)
{
    gpio_handle gpio_test = {0};

    gpio_test.p_gpiox                   = LED_BLUE_PORT;
    gpio_test.gpio_conf.pin_num         = (pin_number_e) LED_BLUE_PIN;
    gpio_test.gpio_conf.mode            = GPIO_MODE_OUTPUT;
    gpio_test.gpio_conf.output_type     = GPIO_OPTYPE_PUSH_PULL;
    gpio_test.gpio_conf.output_speed    = GPIO_OSPEED_MEDIUM;
    gpio_test.gpio_conf.pullup_pulldown = GPIO_PUPD_NO;
    gpio_test.gpio_conf.alt_fn_num      = GPIO_ALT_FN_NA;
    gpio_test.gpio_conf.it_trigger      = GPIO_IT_NA;

    gpio_init(&gpio_test);

    while (1)
    {
        gpio_toggle(gpio_test.p_gpiox, gpio_test.gpio_conf.pin_num);
        for (uint32_t i = 0; i < 500000; i++)
        {
        }
    }
}

//============================================================================//{
//  Function: test_gpio_read_write
//  Overview: Read button pin and turn on led if it is pressed, otherwise turn it off.
//  Test goals:
//      - GPIO initialization
//          - Input mode
//      - Get GPIO pin function
//      - Set GPIO pin function
//  Setup :
//      None
//  Expectations: When the button is pressed, the blue led should turn on. Otherwise
//      it should be off.
//  Note: None
//============================================================================//}
void test_gpio_read_write(void)
{
    gpio_handle gpio_test = {0};

    // Create settings for PA0 (Button pin)
    gpio_test.p_gpiox                   = USER_BUTTON_PORT;
    gpio_test.gpio_conf.pin_num         = (pin_number_e) USER_BUTTON_PIN;
    gpio_test.gpio_conf.mode            = GPIO_MODE_INPUT;
    gpio_test.gpio_conf.output_type     = GPIO_OPTYPE_PUSH_PULL;
    gpio_test.gpio_conf.output_speed    = GPIO_OSPEED_MEDIUM;
    gpio_test.gpio_conf.pullup_pulldown = GPIO_PUPD_NO;
    gpio_test.gpio_conf.alt_fn_num      = GPIO_ALT_FN_NA;
    gpio_test.gpio_conf.it_trigger      = GPIO_IT_NA;
    gpio_init(&gpio_test);

    // Create settings for PD15 (LED pin)
    gpio_test.p_gpiox               = LED_BLUE_PORT;
    gpio_test.gpio_conf.pin_num     = (pin_number_e) LED_BLUE_PIN;
    gpio_test.gpio_conf.mode        = GPIO_MODE_OUTPUT;
    gpio_test.gpio_conf.output_type = GPIO_OPTYPE_PUSH_PULL;
    gpio_init(&gpio_test);

    while (1)
    {
        uint8_t gpio_read_result = 0;
        gpio_read_result         = gpio_read(GPIOA, PIN_NUM_0);
        if (gpio_read_result)
        {
            gpio_write(gpio_test.p_gpiox, gpio_test.gpio_conf.pin_num, HIGH);
        }
        else
        {
            gpio_write(gpio_test.p_gpiox, gpio_test.gpio_conf.pin_num, LOW);
        }
        for (uint32_t i = 0; i < 500000; i++);
    }
}

//============================================================================//{
//  Function: test_gpio_it
//  Overview: Toggle led when button is pressed using interrupts.
//  Test goals: 
//      - Interrupt functionality
//          - Interrupt handler
//          - Interrupt trigger
//  Setup : 
//      None
//  Expectations: The led should be toggled when the user button is pressed.
//  Note: None
//============================================================================//}
void test_gpio_it(void)
{
    // Create settings for PD13 (LED)
    gpio_handle gpio_test = {0};

    gpio_test.p_gpiox                   = GPIOD;
    gpio_test.gpio_conf.pin_num         = PIN_NUM_13;
    gpio_test.gpio_conf.mode            = GPIO_MODE_OUTPUT;
    gpio_test.gpio_conf.output_type     = GPIO_OPTYPE_PUSH_PULL;
    gpio_test.gpio_conf.output_speed    = GPIO_OSPEED_MEDIUM;
    gpio_test.gpio_conf.pullup_pulldown = GPIO_PUPD_NO;
    gpio_test.gpio_conf.alt_fn_num      = GPIO_ALT_FN_NA;
    gpio_test.gpio_conf.it_trigger      = GPIO_IT_NA;
    gpio_init(&gpio_test);

    // Create settings for PA0 (Button pin)
    gpio_test.p_gpiox              = GPIOA;
    gpio_test.gpio_conf.pin_num    = PIN_NUM_0;
    gpio_test.gpio_conf.mode       = GPIO_MODE_INPUT;
    gpio_test.gpio_conf.it_trigger = GPIO_IT_RT;
    gpio_init(&gpio_test);

    while (1);
}

//======================================================================================//}
//                  ISR Handlers
//======================================================================================//{

// Toggle GPIO pin
void EXTI0_IRQHandler(void)
{
    gpio_toggle(GPIOD, PIN_NUM_13);

    gpio_it_handler(EXTI_LINE_NUM_0);

    for (uint32_t i = 0; i < 500000; i++);
}

// *End of file*
