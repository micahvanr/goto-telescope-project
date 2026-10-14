//======================================================================================//{
//  File: usart_test.c
//  Overview: Test some of the USART functionality.
//  Tests:
//      - Transmit/receive (w/ interrupts)
//  Notes: 
//======================================================================================//}

//} Includes
//=========================================//{

#include "usart_test.h"
#include "assert_handler.h"
#include "gpio.h"
#include "printf.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "usart_test_arduino/usart_com_config.h"
#include <stdbool.h>
#include <string.h>

//======================================================================================//}
//                  Private Constants
//======================================================================================//{

// WARNING: May need to change ISR and SCL/SDA pins depending on I2C port used.
#define TEST_USART_PORT      USART2
#define TEST_USART_GPIO_PORT GPIOD

typedef enum {
    TEST_USART_TX_PIN = PIN_NUM_5,
    TEST_USART_RX_PIN = PIN_NUM_6,
} test_usart_pins_e;

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

usart_handle g_usart_test_handle = {0};

//======================================================================================//}
//                  Private Function Prototypes
//======================================================================================//{

static void setup_usart_pin(void);
static void setup_usart_port(void);

//======================================================================================//}
//                  Public Test Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: test_usart_txrx
//  Overview: Test timer transmit and receive with Arduino and logic analyzer.
//  Test goals: 
//      - USART initialization
//      - USART transmit
//      - USART receive
//  Setup: 
//      - PD5 (TX) -> Arduino D0 (RX)
//      - PD6 (RX) -> Arduino D1 (TX)
//      - PD5 -> logic analyzer: read TX info
//      - PD6 -> logic analyzer: read RX info
//  Expectations: STM board should send length and data info, then Arduino should send back
//      length and data info. Data will be compared to what is expect and assert will be triggered
//      if incorrect.
//  Note: None
//============================================================================//}
void test_usart_txrx(void)
{
    setup_usart_pin();
    setup_usart_port();

    uint8_t tx_data[] = "STM->Arduino";
    uint8_t tx_length = sizeof(tx_data) - 1;

    uint8_t const expected_rx[20] = "Arduino->STM";
    uint8_t rx_length;
    uint8_t rx_data[20] = {0};

    while (1)
    {
        // Send string
        usart_transmit(g_usart_test_handle.p_usartx, &tx_length, 1);
        usart_transmit(g_usart_test_handle.p_usartx, tx_data, tx_length);

        // Receive string back
        usart_receive(g_usart_test_handle.p_usartx, &rx_length, 1);
        usart_receive(g_usart_test_handle.p_usartx, (uint8_t *) rx_data, rx_length);

        // Ensure string returns expected value
        if (strcmp((char *) expected_rx, (char *) rx_data) != 0)
        {
            ASSERT(false);
        }
        for (uint32_t i = 0; i < 10000; i++);
    }
}

//============================================================================//{
//  Function: test_usart_txrx_it
//  Overview: Test timer transmit and receive interrupts with Arduino and logic analyzer.
//  Test goals: 
//      - USART initialization
//      - USART transmit
//      - USART receive
//      - USART interrupts
//  Setup: 
//      - PD5 (TX) -> Arduino D0 (RX) via logic level converter
//      - PD6 (RX) -> Arduino D1 (TX) via logic level converter
//      - PD5 -> logic analyzer: read TX info
//      - PD6 -> logic analyzer: read RX info
//  Expectations: STM board should send length and data info, then Arduino should send back
//      length and data info. Data will be compared to what is expect and assert will be triggered
//      if incorrect.
//  Note: None
//============================================================================//}
void test_usart_txrx_it(void)
{
    setup_usart_pin();
    setup_usart_port();

    static uint8_t tx_data[] = "STM->Arduino";
    static uint8_t tx_length = sizeof(tx_data) - 1;

    static uint8_t const expected_rx[20] = "Arduino->STM";
    static uint8_t rx_length             = 0;
    static uint8_t rx_data[20]           = {0};

    while (1)
    {
        // Send string
        usart_transmit_it(&g_usart_test_handle, &tx_length, 1);
        usart_transmit_it(&g_usart_test_handle, tx_data, tx_length);

        // Receive string back
        usart_receive_it(&g_usart_test_handle, &rx_length, 1);
        usart_receive_it(&g_usart_test_handle, (uint8_t *) rx_data, rx_length);

        // Ensure string returns expected value
        if (strcmp((char *) expected_rx, (char *) rx_data) != 0)
        {
            ASSERT(false);
        }
        for (uint32_t i = 0; i < 10000; i++);
    }
}
//======================================================================================//}
//                  Private Function Implementation
//======================================================================================//{

static void setup_usart_pin(void)
{
    gpio_handle gpio_usart           = {0};
    gpio_usart.gpio_conf.alt_fn_num  = GPIO_ALT_FN_7;
    gpio_usart.gpio_conf.mode        = GPIO_MODE_ALT_FN;
    gpio_usart.gpio_conf.output_type = GPIO_OPTYPE_PUSH_PULL;
    gpio_usart.gpio_conf.it_trigger  = GPIO_IT_NA;

    // PD6 - RX
    gpio_usart.p_gpiox           = TEST_USART_GPIO_PORT;
    gpio_usart.gpio_conf.pin_num = (pin_number_e) TEST_USART_RX_PIN;
    gpio_init(&gpio_usart);

    // PD5 - TX
    gpio_usart.p_gpiox           = TEST_USART_GPIO_PORT;
    gpio_usart.gpio_conf.pin_num = (pin_number_e) TEST_USART_TX_PIN;
    gpio_init(&gpio_usart);
}

static void setup_usart_port(void)
{
    g_usart_test_handle.p_usartx                     = TEST_USART_PORT;
    g_usart_test_handle.usart_conf.oversampling_mode = USART_OVERSAMPLING_16;
    g_usart_test_handle.usart_conf.baudrate          = (usart_baudrate_e) USART_COM_SPEED;
    g_usart_test_handle.usart_conf.parity_control    = USART_PARITY_CONTROL_DISABLE;
    g_usart_test_handle.usart_conf.parity_select     = USART_PARITY_SEL_EVEN;
    g_usart_test_handle.usart_conf.stop_bits         = USART_STOP_BITS_1;
    g_usart_test_handle.usart_conf.word_length       = USART_WORD_LENGTH_8_DATA_BITS;
    usart_init(&g_usart_test_handle);
}

//======================================================================================//}
//                  ISR Handlers
//======================================================================================//{

// WARNING: May need to change ISR depending on USART port used.
void USART2_IRQHandler(void)
{
    usart_it_handler(&g_usart_test_handle);
}

// *End of file*
