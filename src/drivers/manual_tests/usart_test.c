#include "usart_test.h"

static void usart_gpio_init(void);
static void usart2_init(void);
static void test_usart_transmit(void);
static void test_usart_receive(void);
static void test_usart_transmit_it(void);
static void test_usart_receive_it(void);

usart_handle g_usart_test = {0};

void usart_tests(test_type_e test)
{
    usart_gpio_init();
    usart2_init();

    switch (test) {
    case USART_TEST_WRITE:    test_usart_transmit(); break;
    case USART_TEST_READ:     test_usart_receive(); break;
    case USART_TEST_WRITE_IT: test_usart_transmit_it(); break;
    case USART_TEST_READ_IT:  test_usart_receive_it(); break;
    default:                  ASSERT(FALSE);
    }

    while (1);
}

static void usart_gpio_init(void)
{
    // PA2 - TX
    // PA3 - RX
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
}

static void usart2_init(void)
{
    g_usart_test.p_usartx                     = USART2;
    g_usart_test.usart_conf.oversampling_mode = USART_OVERSAMPLING_16;
    g_usart_test.usart_conf.baudrate          = USART_BAUDRATE_115200;
    g_usart_test.usart_conf.parity_control    = USART_PARITY_CONTROL_DISABLE;
    g_usart_test.usart_conf.parity_select     = USART_PARITY_SEL_EVEN;
    g_usart_test.usart_conf.stop_bits         = USART_STOP_BITS_1;
    g_usart_test.usart_conf.word_length       = USART_WORD_LENGTH_8_DATA_BITS;
    usart_init(&g_usart_test);
}

static void test_usart_transmit(void)
{
    uint8_t write_data[]  = "testing\r\n";
    uint32_t size_of_data = 0;
    size_of_data          = sizeof(write_data) - 1;
    while (1) {
        usart_transmit(g_usart_test.p_usartx, write_data, sizeof(write_data));
        for (uint32_t i = 0; i < 500000; i++); // Delay
    }
    UNUSED(size_of_data);
}

static void test_usart_receive(void)
{
    uint8_t size_of_data;
    uint8_t data[50];

    while (1) {
        // Get length
        usart_receive(g_usart_test.p_usartx, &size_of_data, 1);
        usart_receive(g_usart_test.p_usartx, data, size_of_data);
        for (uint32_t i = 0; i < 500000; i++); // Delay
    }
}

static void test_usart_transmit_it(void)
{
    static uint8_t write_data[] = "testing\r\n"; // Make static otherwise the data will get corrupted
    uint32_t size_of_data       = 0;
    size_of_data                = (sizeof(write_data) - 1);
    while (1) {
        usart_transmit_it(&g_usart_test, write_data, size_of_data);
        write_data[0] = 'b';
        // for (uint32_t i = 0; i < 500000; i++); // Delay
        usart_transmit_it(&g_usart_test, write_data, size_of_data);
        write_data[0] = 't';

        for (uint32_t i = 0; i < 500000; i++); // Delay
    }
}
static void test_usart_receive_it(void)
{
    static uint8_t read_data[3]; // Make static otherwise the data will get corrupted
    uint32_t size_of_data = 0;
    size_of_data          = sizeof(read_data);
    while (1) {
        usart_receive_it(&g_usart_test, read_data, size_of_data);
        for (uint32_t i = 0; i < 5000000; i++); // Delay
        usart_transmit_it(&g_usart_test, read_data, size_of_data);
        for (uint32_t i = 0; i < 1000000; i++); // Delay
    }
}

void USART2_IRQHandler(void)
{
    usart_it_handler(&g_usart_test);
}
