#include "../common/assert_handler.h"
#include "gpio.h"
#include "rcc.h"
#include "stm32f4xx.h"
#include "usart.h"
#include <stdint.h>

usart_handle g_usart_test = {0};

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
    // Create settings for PD13 (LED)
    gpio_handle gpio_test = {0};

    gpio_test.p_gpiox                   = GPIOD;
    gpio_test.gpio_conf.pin_no          = PIN_NO_13;
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

void test_assert(void)
{
    ASSERT(1);
    ASSERT(0);
}

void test_get_clock(void)
{
    uint32_t ahb1_freq = rcc_get_bus_clock_freq(AHB1_BUS);
    uint32_t apb1_freq = rcc_get_bus_clock_freq(APB1_BUS);
    uint32_t apb2_freq = rcc_get_bus_clock_freq(APB2_BUS);
    UNUSED(ahb1_freq);
    UNUSED(apb1_freq);
    UNUSED(apb2_freq);
}

void usart_gpio_init(void)
{
    gpio_handle gpio_usart           = {0};
    gpio_usart.p_gpiox               = GPIOA;
    gpio_usart.gpio_conf.alt_fn_no   = GPIO_ALT_FN_7;
    gpio_usart.gpio_conf.mode        = GPIO_MODE_ALT_FN;
    gpio_usart.gpio_conf.output_type = GPIO_OPTYPE_PUSH_PULL;
    gpio_usart.gpio_conf.it_trigger  = GPIO_IT_NA;
    gpio_usart.gpio_conf.pin_no      = PIN_NO_2;
    gpio_init(&gpio_usart);
    gpio_usart.gpio_conf.pin_no = PIN_NO_3;
    gpio_init(&gpio_usart);
    // PA2 - TX
    // PA3 - RX
}

void test_usart(void)
{
    usart_gpio_init();

    g_usart_test.p_usartx                     = USART2;
    g_usart_test.usart_conf.oversampling_mode = USART_OVERSAMPLING_16;
    g_usart_test.usart_conf.baudrate          = USART_BAUDRATE_57600;
    g_usart_test.usart_conf.parity_control    = USART_PARITY_CONTROL_DISABLE;
    g_usart_test.usart_conf.parity_select     = USART_PARITY_SEL_EVEN;
    g_usart_test.usart_conf.stop_bits         = USART_STOP_BITS_1;
    g_usart_test.usart_conf.word_length       = USART_WORD_LENGTH_8_DATA_BITS;
    usart_init(&g_usart_test);

#define READ

#ifdef WRITE
    uint8_t write_data[]  = "testing\r\n";
    uint32_t size_of_data = 0;
    size_of_data          = sizeof(write_data) - 1;
    while (1) {
        usart_write(g_usart_test.p_usartx, write_data, sizeof(write_data));
        for (uint32_t i = 0; i < 500000; i++); // Delay
    }
    UNUSED(size_of_data);
#endif

#ifdef READ
    uint8_t size_of_data;
    uint8_t data[50];

    while (1) {
        // Get length
        usart_read(g_usart_test.p_usartx, &size_of_data, 1);
        usart_read(g_usart_test.p_usartx, data, size_of_data);
        for (uint32_t i = 0; i < 500000; i++); // Delay
    }

#endif
}


void test_usart_it(void)
{
    usart_gpio_init();

    g_usart_test.p_usartx                     = USART2;
    g_usart_test.usart_conf.oversampling_mode = USART_OVERSAMPLING_16;
    g_usart_test.usart_conf.baudrate          = USART_BAUDRATE_57600;
    g_usart_test.usart_conf.parity_control    = USART_PARITY_CONTROL_DISABLE;
    g_usart_test.usart_conf.parity_select     = USART_PARITY_SEL_EVEN;
    g_usart_test.usart_conf.stop_bits         = USART_STOP_BITS_1;
    g_usart_test.usart_conf.word_length       = USART_WORD_LENGTH_8_DATA_BITS;
    g_usart_test.usart_conf.it_config         = ENABLE;
    usart_init(&g_usart_test);

#define IT_READ

#ifdef IT_WRITE

    static uint8_t write_data[] = "testing\r\n"; // Make static otherwise the data will get corrupted
    uint32_t size_of_data       = 0;
    size_of_data                = (sizeof(write_data) - 1);
    while (1) {
        usart_write_it(&g_usart_test, write_data, size_of_data);
        write_data[0] = 'b';
        // for (uint32_t i = 0; i < 500000; i++); // Delay
        usart_write_it(&g_usart_test, write_data, size_of_data);
        write_data[0] = 't';

        for (uint32_t i = 0; i < 500000; i++); // Delay
    }
#endif

#ifdef IT_READ
    static uint8_t read_data[3]; // Make static otherwise the data will get corrupted
    uint32_t size_of_data = 0;
    size_of_data          = sizeof(read_data);
    while (1) {
        usart_read_it(&g_usart_test, read_data, size_of_data);
        for (uint32_t i = 0; i < 5000000; i++); // Delay
        usart_write_it(&g_usart_test, read_data, size_of_data);
        for (uint32_t i = 0; i < 1000000; i++); // Delay
    }

#endif
}

int main(void)
{
    // TODO: Move testing functions to their own files

    //test_gpio_blink_led();
    //test_gpio_read_write();
    //test_gpio_it();
    //test_assert();
    //test_get_clock();
    //test_usart();
    test_usart_it();

    while (1);
    return 0;
}

void EXTI0_IRQHandler(void)
{
    gpio_toggle(GPIOD, PIN_NO_13);

    for (uint32_t i = 0; i < 500000; i++);

    gpio_it_handler(EXTI_LINE_NO_0);
}

void USART2_IRQHandler(void)
{
    usart_it_handler(&g_usart_test);
}
