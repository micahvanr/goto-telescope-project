// #include "../common/assert_handler.h"
// #include "gpio.h"
// #include "rcc.h"
// #include "stm32f4xx.h"
// #include "usart.h"

// clang-format off
// Test functions
#include "test_types.h"
#include "gpio_test.h"
#include "timer_test.h"
#include "usart_test.h"
#include "misc_test.h"
#include "i2c_test.h"
// clang-format on
#include "assert_handler.h"
#include "debug_tools.h"
#include "printf.h"

// TODO: Change all drivers to clear bitfields before setting them
// TODO: Add more const qualifiers where it makes sense

int main(void)
{
    // Uses USART2 and PA2 - change inside printf.h
    printf_init();

    // Uses PA3
    debug_pin_init();
    debug_button_init();

    // NOTE: Uncomment line below if you want to run the program after button press
    // while(!read_debug_button());

    printf_(" ");
    toggle_debug_pin();
    toggle_debug_pin();

    // NOTE: Change this variable to run the correct test
    test_type_e test = TIMER_TEST_FREQ;
    switch (test) {

    // GPIO tests
    case GPIO_TEST_BLINK_LED:
    case GPIO_TEST_READ_WRITE:
    case GPIO_TEST_IT:         gpio_tests(test); break;

    // USART tests (tested with Arduino)
    case USART_TEST_RXTX:
    case USART_TEST_RXTX_IT:   usart_tests(test); break;

    // I2C tests - Change master/slave and transmit/receive options in I2C config header file
    // (tested with Arduino)
    case I2C_TEST_REG:
    case I2C_TEST_IT:          i2c_tests(test); break;

    // Timer tests
    case TIMER_TEST_AUTO:
    case TIMER_TEST_MANUAL:
    case TIMER_TEST_FREQ:
    case TIMER_TEST_DELAY:
    case TIMER_TEST_OC_PWM:
    case TIMER_TEST_BASIC_IT:
    case TIMER_TEST_IC_IT:     timer_tests(test); break;

    // Misc tests
    case MISC_TEST_ASSERT:
    case MISC_TEST_GET_CLOCK:
    case MISC_TEST_PRINTF:     misc_tests(test); break;
    }

    ASSERT(false);
}
