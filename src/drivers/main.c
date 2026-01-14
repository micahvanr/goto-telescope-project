#include "../common/assert_handler.h"
#include "gpio.h"
#include "stm32f4xx.h"
#include <stdint.h>

void test_gpio_blink_led(void)
{
    gpio_handle gpio_test = {0};

    // Create settings for PD15 (LED pin)
    gpio_test.p_gpiox                   = GPIOD;
    gpio_test.gpio_conf.pin_no          = PIN_NO_15;
    gpio_test.gpio_conf.mode            = GPIO_MODE_OUTPUT;
    gpio_test.gpio_conf.output_type     = GPIO_OPTYPE_PUSH_PULL;
    gpio_test.gpio_conf.output_speed    = GPIO_OSPEED_MEDIUM;
    gpio_test.gpio_conf.pullup_pulldown = GPIO_NO_PUPD;
    gpio_test.gpio_conf.alt_fn_no       = GPIO_ALT_FN_NA;
    gpio_test.gpio_conf.it_trigger      = GPIO_IT_NA;

    gpio_init(&gpio_test);

    while (1) {
        gpio_toggle(gpio_test.p_gpiox, gpio_test.gpio_conf.pin_no);
        for (uint32_t i = 0; i < 500000; i++) {}
    }
}

void test_gpio_read_write(void)
{
    gpio_handle gpio_test = {0};

    // Create settings for PA0 (Button pin)
    gpio_test.p_gpiox                   = GPIOA;
    gpio_test.gpio_conf.pin_no          = PIN_NO_0;
    gpio_test.gpio_conf.mode            = GPIO_MODE_INPUT;
    gpio_test.gpio_conf.output_type     = GPIO_OPTYPE_PUSH_PULL;
    gpio_test.gpio_conf.output_speed    = GPIO_OSPEED_MEDIUM;
    gpio_test.gpio_conf.pullup_pulldown = GPIO_NO_PUPD;
    gpio_test.gpio_conf.alt_fn_no       = GPIO_ALT_FN_NA;
    gpio_test.gpio_conf.it_trigger      = GPIO_IT_NA;
    gpio_init(&gpio_test);

    // Create settings for PD15 (LED pin)
    gpio_test.p_gpiox               = GPIOD;
    gpio_test.gpio_conf.pin_no      = PIN_NO_15;
    gpio_test.gpio_conf.mode        = GPIO_MODE_OUTPUT;
    gpio_test.gpio_conf.output_type = GPIO_OPTYPE_PUSH_PULL;
    gpio_init(&gpio_test);

    while (1) {
        uint8_t gpio_read_result = 0;
        gpio_read_result         = gpio_read(GPIOA, PIN_NO_0);
        if (gpio_read_result) {
            gpio_write(gpio_test.p_gpiox, gpio_test.gpio_conf.pin_no, HIGH);
        } else {
            gpio_write(gpio_test.p_gpiox, gpio_test.gpio_conf.pin_no, LOW);
        }
        for (uint32_t i = 0; i < 500000; i++);
    }
}

void test_gpio_it(void)
{
    // Create settings for PD14 (LED)
    gpio_handle gpio_test = {0};

    gpio_test.p_gpiox                   = GPIOD;
    gpio_test.gpio_conf.pin_no          = PIN_NO_14;
    gpio_test.gpio_conf.mode            = GPIO_MODE_OUTPUT;
    gpio_test.gpio_conf.output_type     = GPIO_OPTYPE_PUSH_PULL;
    gpio_test.gpio_conf.output_speed    = GPIO_OSPEED_MEDIUM;
    gpio_test.gpio_conf.pullup_pulldown = GPIO_NO_PUPD;
    gpio_test.gpio_conf.alt_fn_no       = GPIO_ALT_FN_NA;
    gpio_test.gpio_conf.it_trigger      = GPIO_IT_NA;
    gpio_init(&gpio_test);

    // Create settings for PA0 (Button pin)
    gpio_test.p_gpiox              = GPIOA;
    gpio_test.gpio_conf.pin_no     = PIN_NO_0;
    gpio_test.gpio_conf.mode       = GPIO_MODE_INPUT;
    gpio_test.gpio_conf.it_trigger = GPIO_IT_RT;
    gpio_init(&gpio_test);

    while (1);
}

void test_assert(void) {
    ASSERT(1);
    ASSERT(0);
}

int main(void)
{
    //test_gpio_blink_led();
    //test_gpio_read_write();
    //test_gpio_it();
    test_assert();
    return 0;
}

void EXTI0_IRQHandler(void)
{
    gpio_toggle(GPIOD, PIN_NO_14);

    for (uint32_t i = 0; i < 500000; i++);

    gpio_handler(EXTI_LINE_NO_0);
}
