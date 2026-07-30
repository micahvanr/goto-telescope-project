#include "usart_test.h"
#include "assert_handler.h"
#include "gpio.h"
#include "printf.h"
#include "usart.h"
#include "usart_test_arduino/usart_com_config.h"
#include <string.h>

static void usart_gpio_init(void);
static void usart2_init(void);
static void test_usart_txrx(void);
static void test_usart_txrx_it(void);

usart_handle g_usart_test_handle = {0};

void usart_tests(test_type_e test)
{
    usart_gpio_init();
    usart2_init();

    switch (test) {
    case USART_TEST_RXTX:    test_usart_txrx(); break;
    case USART_TEST_RXTX_IT: test_usart_txrx_it(); break;
    default:                 ASSERT(false);
    }
    ASSERT(false);
}

static void usart_gpio_init(void)
{
    gpio_handle gpio_usart           = {0};
    gpio_usart.gpio_conf.alt_fn_no   = GPIO_ALT_FN_7;
    gpio_usart.gpio_conf.mode        = GPIO_MODE_ALT_FN;
    gpio_usart.gpio_conf.output_type = GPIO_OPTYPE_PUSH_PULL;
    gpio_usart.gpio_conf.it_trigger  = GPIO_IT_NA;

    // PD6 - RX
    gpio_usart.p_gpiox          = GPIOD;
    gpio_usart.gpio_conf.pin_no = PIN_NO_6;
    gpio_init(&gpio_usart);

    // PD5 - TX
    gpio_usart.p_gpiox          = GPIOD;
    gpio_usart.gpio_conf.pin_no = PIN_NO_5;
    gpio_init(&gpio_usart);
}

static void usart2_init(void)
{
    g_usart_test_handle.p_usartx                     = USART2;
    g_usart_test_handle.usart_conf.oversampling_mode = USART_OVERSAMPLING_16;
    g_usart_test_handle.usart_conf.baudrate          = (usart_baudrate_e)USART_COM_SPEED;
    g_usart_test_handle.usart_conf.parity_control    = USART_PARITY_CONTROL_DISABLE;
    g_usart_test_handle.usart_conf.parity_select     = USART_PARITY_SEL_EVEN;
    g_usart_test_handle.usart_conf.stop_bits         = USART_STOP_BITS_1;
    g_usart_test_handle.usart_conf.word_length       = USART_WORD_LENGTH_8_DATA_BITS;
    usart_init(&g_usart_test_handle);
}

static void test_usart_txrx(void)
{
    uint8_t tx_data[] = "STM->Arduino";
    uint8_t tx_length = sizeof(tx_data) - 1;

    uint8_t const expected_rx[20] = "Arduino->STM";
    uint8_t rx_length;
    uint8_t rx_data[20] = {0};

    while (1) {
        // Send string
        usart_transmit(g_usart_test_handle.p_usartx, &tx_length, 1);
        usart_transmit(g_usart_test_handle.p_usartx, tx_data, tx_length);

        // Receive string back
        usart_receive(g_usart_test_handle.p_usartx, &rx_length, 1);
        usart_receive(g_usart_test_handle.p_usartx, (uint8_t *)rx_data, rx_length);

        // Ensure string returns expected value
        if (strcmp((char *)expected_rx, (char *)rx_data) != 0) {
            ASSERT(false);
        }
        for (uint32_t i = 0; i < 10000; i++);
    }
}

static void test_usart_txrx_it(void)
{
    static uint8_t tx_data[] = "STM->Arduino";
    static uint8_t tx_length = sizeof(tx_data) - 1;

    static uint8_t const expected_rx[20] = "Arduino->STM";
    static uint8_t rx_length             = 0;
    static uint8_t rx_data[20]           = {0};

    while (1) {
        // Send string
        usart_transmit_it(&g_usart_test_handle, &tx_length, 1);
        usart_transmit_it(&g_usart_test_handle, tx_data, tx_length);

        // Receive string back
        usart_receive_it(&g_usart_test_handle, &rx_length, 1);
        usart_receive_it(&g_usart_test_handle, (uint8_t *)rx_data, rx_length);

        // Ensure string returns expected value
        if (strcmp((char *)expected_rx, (char *)rx_data) != 0) {
            ASSERT(false);
        }
        for (uint32_t i = 0; i < 10000; i++);
    }
}

void USART2_IRQHandler(void)
{
    usart_it_handler(&g_usart_test_handle);
}
