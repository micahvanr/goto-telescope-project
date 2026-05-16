// #include "../common/assert_handler.h"
// #include "gpio.h"
// #include "rcc.h"
// #include "stm32f4xx.h"
// #include "usart.h"
#include <stdint.h>

// clang-format off
// Test functions
#include "manual_tests/test_types.h"
#include "manual_tests/gpio_test.h"
#include "manual_tests/usart_test.h"
#include "manual_tests/misc_test.h"
#include "manual_tests/i2c_test.h"
// clang-format on
#include "../common/printf.h"

int main(void)
{

    // Uses USART1 and PA9
    printf_init();

    // Change this variable to run the correct test
    test_type_e test = I2C_TEST_SLAVE_RECEIVE_IT;
    switch (test) {

    // GPIO tests
    case GPIO_TEST_BLINK_LED:
    case GPIO_TEST_READ_WRITE:
    case GPIO_TEST_IT:                gpio_tests(test); break;

    // USART tests
    case USART_TEST_WRITE:
    case USART_TEST_READ:
    case USART_TEST_WRITE_IT:
    case USART_TEST_READ_IT:          usart_tests(test); break;
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
}
