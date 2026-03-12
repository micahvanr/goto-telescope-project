#include "../common/assert_handler.h"
#include "gpio.h"
#include "rcc.h"
#include "stm32f4xx.h"
#include "usart.h"
#include <stdint.h>

// clang-format off
// Test functions
#include "manual_tests/test_types.h"
#include "manual_tests/gpio_test.h"
#include "manual_tests/usart_test.h"
#include "manual_tests/misc_test.h"
// clang-format on

int main(void) 
{
    
    // Change this variable to run the correct test
    test_type_e test = MISC_TEST_ASSERT;
    switch (test) {

    // GPIO tests
    case GPIO_TEST_BLINK_LED:
    case GPIO_TEST_READ_WRITE:
    case GPIO_TEST_IT:
        gpio_tests(test);
    break;

    // USART tests
    case USART_TEST_WRITE:
    case USART_TEST_READ:
    case USART_TEST_WRITE_IT:
    case USART_TEST_READ_IT:
        usart_tests(test);
    break;
    // I2C tests
    case I2C_TEST_WRITE:
    case I2C_TEST_READ:
    case I2C_TEST_WRITE_IT:
    case I2C_TEST_READ_IT:
        i2c_tests(test);
    // Misc tests
    case MISC_TEST_ASSERT:
    case MISC_TEST_GET_CLOCK:
        misc_tests(test);
    break;
    }
}