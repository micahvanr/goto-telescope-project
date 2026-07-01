// #include "../common/assert_handler.h"
// #include "gpio.h"
// #include "rcc.h"
// #include "stm32f4xx.h"
// #include "usart.h"

// clang-format off
// Test functions
#include "test_types.h"
#include "gpio_test.h"
#include "usart_test.h"
#include "misc_test.h"
#include "i2c_test.h"
// clang-format on
#include "assert_handler.h"
#include "debug_pin.h"
#include "printf.h"

// TODO: Improve test functions:
// USART & I2C
// Put Arduino code in git repo. Arduino files should test 1-3 functions but should be organized logically.
// Ex. I2C slave transmit with/without interrupts should have their own Arduino file.

int main(void)
{
    // Uses USART2 and PA2 - change inside printf.h
    printf_init();
    // Uses PA3
    debug_pin_init();

    // NOTE: Change this variable to run the correct test
    test_type_e test = USART_TEST_RXTX;
    switch (test) {

    // GPIO tests
    case GPIO_TEST_BLINK_LED:
    case GPIO_TEST_READ_WRITE:
    case GPIO_TEST_IT:                gpio_tests(test); break;

    // USART tests
    case USART_TEST_RXTX:
    case USART_TEST_RXTX_IT:          usart_tests(test); break;

    // I2C tests
    case I2C_TEST_MASTER_TRANSMIT:
    case I2C_TEST_MASTER_RECEIVE:
    case I2C_TEST_SLAVE_TRANSMIT:
    case I2C_TEST_SLAVE_RECEIVE:
    case I2C_TEST_MASTER_TRANSMIT_IT:
    case I2C_TEST_MASTER_RECEIVE_IT:
    case I2C_TEST_SLAVE_TRANSMIT_IT:
    case I2C_TEST_SLAVE_RECEIVE_IT:   i2c_tests(test); break;

    // Misc tests
    case MISC_TEST_ASSERT:
    case MISC_TEST_GET_CLOCK:
    case MISC_TEST_PRINTF:            misc_tests(test); break;
    }

    ASSERT(false);
}
