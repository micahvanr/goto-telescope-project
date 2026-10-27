//======================================================================================//{
//  File: main.c
//  Overview: Main entry point of program. Currently runs tests.
//  Features:
//      - Run various tests
//  Notes: None
//======================================================================================//}

// clang-format off
// Test functions
#include "rtc_test.h"
#include "test_types.h"
#include "gpio_test.h"
#include "timer_test.h"
#include "usart_test.h"
#include "misc_test.h"
#include "i2c_test.h"
#include "i2c_test_arduino/i2c_com_config.h"
// clang-format on

#include "assert_handler.h"
#include "debug_tools.h"
#include "printf.h"
#include <stdbool.h>
#include <stdint.h>

// TODO: Refactor linker script and startup code.

// TODO: Add project roadmap using mermaid markdown.
// TODO: Think about removing handle struct.

int main(void)
{
    // NOTE: Change this variable to run the correct test.
    test_type_e test = TIMER_TEST_FREQ;

    // NOTE: Change "I2C_TEST_MODE" constant to change I2C test mode further.
    uint8_t i2c_test_mode = I2C_TEST_MODE;

    // Uses USART2 and PA2 - change inside printf.h
    printf_init();

    // Uses PA3
    debug_pin_init();
    debug_button_init();

    // NOTE: Uncomment line below if you want to run the program after button press.
    // while(!read_debug_button());

    printf_(" ");
    toggle_debug_pin();
    toggle_debug_pin();

    switch (test)
    {

        // GPIO tests
        case GPIO_TEST_BLINK_LED:
            test_gpio_blink_led();
            break;
        case GPIO_TEST_READ_WRITE:
            test_gpio_read_write();
            break;
        case GPIO_TEST_IT:
            test_gpio_it();
            break;

        // USART tests (tested with Arduino)
        case USART_TEST_RXTX:
            test_usart_txrx();
            break;
        case USART_TEST_RXTX_IT:
            test_usart_txrx_it();
            break;

        // I2C tests (tested with Arduino)
        case I2C_TEST_REG:
            switch (i2c_test_mode)
            {
                case I2C_TEST_STM_MASTER_TX:
                    test_i2c_master_transmit();
                    break;
                case I2C_TEST_STM_MASTER_RX:
                    test_i2c_master_receive();
                    break;
                case I2C_TEST_STM_SLAVE_TX:
                    test_i2c_slave_transmit();
                    break;
                case I2C_TEST_STM_SLAVE_RX:
                    test_i2c_slave_receive();
                    break;
            }
            break;
        case I2C_TEST_IT:
            switch (i2c_test_mode)
            {
                case I2C_TEST_STM_MASTER_TX:
                    test_i2c_master_transmit_it();
                    break;
                case I2C_TEST_STM_MASTER_RX:
                    test_i2c_master_receive_it();
                    break;
                case I2C_TEST_STM_SLAVE_TX:
                    test_i2c_slave_transmit_it();
                    break;
                case I2C_TEST_STM_SLAVE_RX:
                    test_i2c_slave_receive_it();
                    break;
            }
            break;

        // Timer tests
        case TIMER_TEST_AUTO:
            test_timer_auto();
            break;
        case TIMER_TEST_MANUAL:
            test_timer_manual();
            break;
        case TIMER_TEST_FREQ:
            test_timer_freq();
            break;
        case TIMER_TEST_DELAY:
            test_delay();
            break;
        case TIMER_TEST_OC_PWM:
            test_oc_pwm();
            break;
        case TIMER_TEST_IC_IT:
            test_ic_it();
            break;
        case TIMER_TEST_BASIC_IT:
            test_it();
            break;

        // RTC tests
        case RTC_TEST_BASE:
            test_rtc_base();
            break;
        case RTC_TEST_ALARM:
            test_rtc_alarm();
            break;
        case RTC_TEST_TIMESTAMP:
            test_rtc_timestamp();
            break;
        case RTC_TEST_WAKEUP:
            test_rtc_wakeup();
            break;

        // Misc tests
        case MISC_TEST_ASSERT:
            test_assert();
            break;
        case MISC_TEST_GET_CLOCK:
            test_get_clock();
            break;
        case MISC_TEST_PRINTF:
            test_printf();
            break;
    }

    ASSERT(false);
}

// *End of file*
