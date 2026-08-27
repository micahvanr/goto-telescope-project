#ifndef TEST_TYPES_H
#define TEST_TYPES_H

typedef enum {
    // GPIO tests options
    GPIO_TEST_BLINK_LED,
    GPIO_TEST_READ_WRITE,
    GPIO_TEST_IT,
    // USART tests options
    USART_TEST_RXTX,
    USART_TEST_RXTX_IT,
    // I2C test options
    I2C_TEST_REG,
    I2C_TEST_IT,
    // Timer test options
    TIMER_TEST_AUTO,
    TIMER_TEST_MANUAL,
    TIMER_TEST_FREQ,
    TIMER_TEST_DELAY,
    TIMER_TEST_OC_PWM,
    TIMER_TEST_IC_IT,
    TIMER_TEST_BASIC_IT,

    // Misc tests options
    MISC_TEST_ASSERT,
    MISC_TEST_GET_CLOCK,
    MISC_TEST_PRINTF,
} test_type_e;

#endif
