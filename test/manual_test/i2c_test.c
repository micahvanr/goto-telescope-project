//======================================================================================//{
//  File: i2c_test.c
//  Overview: Tests some of the I2C functionality.
//  Tests:
//      - Master/slave transmit/receive (w/ interrupts)
//  Notes: Check ./i2c_test_arduino/i2c_com_config.h file for more information on I2C
//      comminication settings.
//======================================================================================//}

//} Includes
//=========================================//{

#include "i2c_test.h"
#include "assert_handler.h"
#include "gpio.h"
#include "i2c.h"
#include "i2c_test_arduino/i2c_com_config.h"
#include "stm32f4xx.h"
#include <stdbool.h>
#include <string.h>

//======================================================================================//}
//                  Private Constants
//======================================================================================//{

// WARNING: May need to change ISR and SCL/SDA pins depending on I2C port used.
#define TEST_I2C_PORT      I2C1
#define TEST_I2C_GPIO_PORT GPIOB

typedef enum {
    TEST_I2C_SCL_PIN = PIN_NUM_6,
    TEST_I2C_SDA_PIN = PIN_NUM_7,
} test_i2c_pins_e;

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

i2c_handle g_i2c_handle = {0};

//======================================================================================//}
//                  Private Function Prototypes
//======================================================================================//{

static void i2c_pin_setup(void);
static void i2c_port_setup(void);

//======================================================================================//}
//                  Public Test Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: test_i2c_master_transmit
//  Overview: Transmit data as master on I2C bus.
//  Test goals:
//      - Act as master and send data on bus.
//  Setup: Reference constants for more info
//      - PB6 -> SCL
//      - PB7 -> SDA
//      - Arduino with test code connected via logic level converter.
//          - A5 -> SCL
//          - A4 -> SDA
//  Expectations: Arduino terminal should output "Received:STM master transmit".
//      Optionally use logic analyzer to view communication.
//  Note: None
//============================================================================//}
void test_i2c_master_transmit(void)
{
    i2c_pin_setup();
    i2c_port_setup();

    uint8_t tx_data[] = "STM master transmit";
    uint8_t tx_len    = sizeof(tx_data) - 1;

    while (1)
    {
        i2c_master_transmit(TEST_I2C_PORT, I2C_ARDUINO_ADDR, tx_data, tx_len, I2C_REPEATED_START_DISABLE);
        for (uint32_t i = 0; i < 5000; i++);
    }
}

//============================================================================//{
//  Function: test_i2c_slave_transmit
//  Overview: Transmit data as slave on I2C bus.
//  Test goals:
//      - Act as slave and send data on bus.
//  Setup: Reference constants for more info
//      - PB6 -> SCL
//      - PB7 -> SDA
//      - Arduino with test code connected via logic level converter.
//          - A5 -> SCL
//          - A4 -> SDA
//  Expectations: Use Arduino terminal to trigger request from STM board. Arduino
//      should output "Received:STM slave transmit".
//      Optionally use logic analyzer to view communication.
//  Note: None
//============================================================================//}
void test_i2c_slave_transmit(void)
{
    i2c_pin_setup();
    i2c_port_setup();

    uint8_t tx_data[] = "STM slave transmit";
    uint8_t tx_len    = sizeof(tx_data) - 1;

    while (1)
    {
        i2c_slave_transmit(TEST_I2C_PORT, tx_data, tx_len);
    }
}

//============================================================================//{
//  Function: test_i2c_master_receive
//  Overview: Receive data as master on I2C bus.
//  Test goals:
//      - Act as master and receive data on bus.
//  Setup: Reference constants for more info
//      - PB6 -> SCL
//      - PB7 -> SDA
//      - Arduino with test code connected via logic level converter.
//          - A5 -> SCL
//          - A4 -> SDA
//  Expectations: STM board should run without triggering assert. Assert will trigger
//      if the string received is not correct. Use logic analyzer to ensure proper
//      communication.
//  Note: None
//============================================================================//}
void test_i2c_master_receive(void)
{
    i2c_pin_setup();
    i2c_port_setup();

    uint8_t expected_string[] = "Arduino->STM";
    uint8_t rx_len            = sizeof(expected_string) - 1;
    uint8_t rx_data[32];

    while (1)
    {
        i2c_master_receive(TEST_I2C_PORT, I2C_ARDUINO_ADDR, rx_data, rx_len, I2C_REPEATED_START_DISABLE);
        // Ensure string returns expected value
        if (strcmp((char *) expected_string, (char *) rx_data) != 0)
        {
            ASSERT(false);
        }
        for (uint32_t i = 0; i < 5000; i++);
    }
}

//============================================================================//{
//  Function: test_i2c_slave_receive
//  Overview: Receive data as slave on I2C bus.
//  Test goals:
//      - Act as slave and receive data on bus.
//  Setup: Reference constants for more info
//      - PB6 -> SCL
//      - PB7 -> SDA
//      - Arduino with test code connected via logic level converter.
//          - A5 -> SCL
//          - A4 -> SDA
//  Expectations: Use Arduino terminal to trigger request from STM board.
//      STM board should run without triggering assert. Assert will trigger
//      if the string received is not correct. Use logic analyzer to ensure proper
//      communication.
//  Note: None
//============================================================================//}
void test_i2c_slave_receive(void)
{
    i2c_pin_setup();
    i2c_port_setup();

    uint8_t expected_string[] = "Arduino->STM";
    uint8_t rx_len            = sizeof(expected_string) - 1;
    uint8_t rx_data[32];

    while (1)
    {
        i2c_slave_receive(TEST_I2C_PORT, rx_data, rx_len);
        // Ensure string returns expected value
        if (strcmp((char *) expected_string, (char *) rx_data) != 0)
        {
            ASSERT(false);
        }
    }
}

//============================================================================//{
//  Function: test_i2c_master_transmit_it
//  Overview: Transmit data as master on I2C bus using interrupts.
//  Test goals:
//      - Act as master and send data on bus.
//      - Use interrupts for communication.
//  Setup: Reference constants for more info
//      - PB6 -> SCL
//      - PB7 -> SDA
//      - Arduino with test code connected via logic level converter.
//          - A5 -> SCL
//          - A4 -> SDA
//  Expectations: Arduino terminal should output "Received:STM master transmit".
//      Optionally use logic analyzer to view communication.
//  Note: None
//============================================================================//}
void test_i2c_master_transmit_it(void)
{
    i2c_pin_setup();
    i2c_port_setup();

    static uint8_t tx_data[] = "STM master transmit";
    static uint8_t tx_len    = sizeof(tx_data) - 1;

    while (1)
    {
        i2c_master_transmit_it(&g_i2c_handle, I2C_ARDUINO_ADDR, tx_data, tx_len, I2C_REPEATED_START_DISABLE);
        for (uint32_t i = 0; i < 5000; i++);
    }
}

//============================================================================//{
//  Function: test_i2c_slave_transmit_it
//  Overview: Transmit data as slave on I2C bus using interrupts.
//  Test goals:
//      - Act as slave and send data on bus.
//      - Use interrupts for communication.
//  Setup: Reference constants for more info
//      - PB6 -> SCL
//      - PB7 -> SDA
//      - Arduino with test code connected via logic level converter.
//          - A5 -> SCL
//          - A4 -> SDA
//  Expectations: Use Arduino terminal to trigger request from STM board. Arduino
//      should output "Received:STM slave transmit".
//      Optionally use logic analyzer to view communication.
//  Note: None
//============================================================================//}
void test_i2c_slave_transmit_it(void)
{
    i2c_pin_setup();
    i2c_port_setup();

    static uint8_t tx_data[] = "STM slave transmit";
    static uint8_t tx_len    = sizeof(tx_data) - 1;

    while (1)
    {
        i2c_slave_transmit_it(&g_i2c_handle, tx_data, tx_len);
    }
}

//============================================================================//{
//  Function: test_i2c_master_receive_it
//  Overview: Receive data as master on I2C bus using interrupts.
//  Test goals:
//      - Act as master and receive data on bus.
//      - Use interrupts for communication.
//  Setup: Reference constants for more info
//      - PB6 -> SCL
//      - PB7 -> SDA
//      - Arduino with test code connected via logic level converter.
//          - A5 -> SCL
//          - A4 -> SDA
//  Expectations: STM board should run without triggering assert. Assert will trigger
//      if the string received is not correct. Use logic analyzer to ensure proper
//      communication.
//  Note: None
//============================================================================//}
void test_i2c_master_receive_it(void)
{
    i2c_pin_setup();
    i2c_port_setup();

    static uint8_t expected_string[] = "Arduino->STM";
    static uint8_t rx_len            = sizeof(expected_string) - 1;
    static uint8_t rx_data[32];

    while (1)
    {
        i2c_master_receive_it(&g_i2c_handle, I2C_ARDUINO_ADDR, rx_data, rx_len, I2C_REPEATED_START_DISABLE);
        for (uint32_t i = 0; i < 5000; i++);
    }
}

//============================================================================//{
//  Function: test_i2c_slave_receive_it
//  Overview: Receive data as slave on I2C bus using interrupts.
//  Test goals:
//      - Act as slave and receive data on bus.
//      - Use interrupts for communication.
//  Setup: Reference constants for more info
//      - PB6 -> SCL
//      - PB7 -> SDA
//      - Arduino with test code connected via logic level converter.
//          - A5 -> SCL
//          - A4 -> SDA
//  Expectations: Use Arduino terminal to trigger request from STM board.
//      STM board should run without triggering assert. Assert will trigger
//      if the string received is not correct. Use logic analyzer to ensure proper
//      communication.
//  Note: None
//============================================================================//}
void test_i2c_slave_receive_it(void)
{
    i2c_pin_setup();
    i2c_port_setup();

    static uint8_t expected_string[] = "Arduino->STM";
    static uint8_t rx_len            = sizeof(expected_string) - 1;
    static uint8_t rx_data[32];

    while (1)
    {
        i2c_slave_receive_it(&g_i2c_handle, rx_data, rx_len);
    }
}

//======================================================================================//}
//                  Private Function Implementation
//======================================================================================//{

static void i2c_pin_setup(void)
{
    gpio_handle i2c_pins            = {0};
    i2c_pins.gpio_conf.it_trigger   = GPIO_IT_NA;
    i2c_pins.gpio_conf.mode         = GPIO_MODE_ALT_FN;
    i2c_pins.gpio_conf.output_speed = GPIO_OSPEED_MEDIUM;

    i2c_pins.gpio_conf.alt_fn_num      = GPIO_ALT_FN_4;
    i2c_pins.gpio_conf.output_type     = GPIO_OPTYPE_OPEN_DRAIN;
    i2c_pins.gpio_conf.pullup_pulldown = GPIO_PULL_UP;

    // PB6 - SCL
    i2c_pins.p_gpiox           = TEST_I2C_GPIO_PORT;
    i2c_pins.gpio_conf.pin_num = (pin_number_e) TEST_I2C_SCL_PIN;
    gpio_init(&i2c_pins);

    // PB7 - SDA
    i2c_pins.p_gpiox           = TEST_I2C_GPIO_PORT;
    i2c_pins.gpio_conf.pin_num = (pin_number_e) TEST_I2C_SDA_PIN;
    gpio_init(&i2c_pins);
}

static void i2c_port_setup(void)
{
    g_i2c_handle.p_i2cx                 = TEST_I2C_PORT;
    g_i2c_handle.i2c_conf.clock_freq_hz = I2C_CLK_FREQ_100KHZ;
    g_i2c_handle.i2c_conf.fm_duty_cycle = I2C_FM_DUTY_2;
    g_i2c_handle.i2c_conf.own_address   = I2C_STM_ADDR;
    g_i2c_handle.i2c_conf.speed_mode    = I2C_SPEED_MODE_STANDARD;
    i2c_init(&g_i2c_handle);
}

//======================================================================================//}
//                  ISR Handlers
//======================================================================================//{

// WARNING: May need to change ISR depending on I2C port used.
void I2C1_EV_IRQHandler()
{
    i2c_it_handler(&g_i2c_handle);
}

void I2C1_ER_IRQHandler()
{
    ASSERT(false);
}

// *End of file*
