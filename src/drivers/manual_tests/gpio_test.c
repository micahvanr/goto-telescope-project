#include "gpio_test.h"
#include "../gpio.h"

static void test_gpio_blink_led(void);
static void test_gpio_read_write(void);
static void test_gpio_it(void);

void gpio_tests(test_type_e test)
{
    switch (test) {
    case GPIO_TEST_BLINK_LED:  test_gpio_blink_led(); break;
    case GPIO_TEST_READ_WRITE: test_gpio_read_write(); break;
    case GPIO_TEST_IT:         test_gpio_it(); break;
    default:                   ASSERT(FALSE);
    }

    while (1);
}

static void test_gpio_blink_led(void)
{
    gpio_handle gpio_test = {0};

    // Create settings for PD15 (LED pin)
    gpio_test.p_gpiox                   = GPIOD;
    gpio_test.gpio_conf.pin_no          = PIN_NO_15;
    gpio_test.gpio_conf.mode            = GPIO_MODE_OUTPUT;
    gpio_test.gpio_conf.output_type     = GPIO_OPTYPE_PUSH_PULL;
    gpio_test.gpio_conf.output_speed    = GPIO_OSPEED_MEDIUM;
    gpio_test.gpio_conf.pullup_pulldown = GPIO_PUPD_NO;
    gpio_test.gpio_conf.alt_fn_no       = GPIO_ALT_FN_NA;
    gpio_test.gpio_conf.it_trigger      = GPIO_IT_NA;

    gpio_init(&gpio_test);

    while (1) {
        gpio_toggle(gpio_test.p_gpiox, gpio_test.gpio_conf.pin_no);
        for (uint32_t i = 0; i < 500000; i++) {}
    }
}

static void test_gpio_read_write(void)
{
    gpio_handle gpio_test = {0};

    // Create settings for PA0 (Button pin)
    gpio_test.p_gpiox                   = GPIOA;
    gpio_test.gpio_conf.pin_no          = PIN_NO_0;
    gpio_test.gpio_conf.mode            = GPIO_MODE_INPUT;
    gpio_test.gpio_conf.output_type     = GPIO_OPTYPE_PUSH_PULL;
    gpio_test.gpio_conf.output_speed    = GPIO_OSPEED_MEDIUM;
    gpio_test.gpio_conf.pullup_pulldown = GPIO_PUPD_NO;
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

static void test_gpio_it(void)
{
    // Create settings for PD13 (LED)
    gpio_handle gpio_test = {0};

    gpio_test.p_gpiox                   = GPIOD;
    gpio_test.gpio_conf.pin_no          = PIN_NO_13;
    gpio_test.gpio_conf.mode            = GPIO_MODE_OUTPUT;
    gpio_test.gpio_conf.output_type     = GPIO_OPTYPE_PUSH_PULL;
    gpio_test.gpio_conf.output_speed    = GPIO_OSPEED_MEDIUM;
    gpio_test.gpio_conf.pullup_pulldown = GPIO_PUPD_NO;
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

void EXTI0_IRQHandler(void)
{
    gpio_toggle(GPIOD, PIN_NO_13);

    for (uint32_t i = 0; i < 500000; i++);

    gpio_it_handler(EXTI_LINE_NO_0);
}
