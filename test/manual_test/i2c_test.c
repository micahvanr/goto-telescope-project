#include "i2c_test.h"
#include "assert_handler.h"
#include "gpio.h"
#include "i2c.h"
#include "i2c_test_arduino/i2c_com_config.h"

static void i2c1_gpio_init(void);
static void i2c1_init(void);
static void test_i2c_master_transmit(void);
static void test_i2c_slave_transmit(void);
static void test_i2c_master_receive(void);
static void test_i2c_slave_receive(void);
static void test_i2c_master_transmit_it(void);
static void test_i2c_slave_transmit_it(void);
static void test_i2c_master_receive_it(void);
static void test_i2c_slave_receive_it(void);

i2c_handle g_i2c_handle = {0};

void i2c_tests(test_type_e test)
{
    uint8_t test_mode = I2C_TEST_MODE;

    i2c1_gpio_init();
    i2c1_init();

    if (test == I2C_TEST_REG) {
        switch (test_mode) {
        case I2C_TEST_STM_MASTER_TX: test_i2c_master_transmit(); break;
        case I2C_TEST_STM_MASTER_RX: test_i2c_master_receive(); break;
        case I2C_TEST_STM_SLAVE_TX:  test_i2c_slave_transmit(); break;
        case I2C_TEST_STM_SLAVE_RX:  test_i2c_slave_receive(); break;
        }
    } else if (test == I2C_TEST_IT) {
        switch (test_mode) {
        case I2C_TEST_STM_MASTER_TX: test_i2c_master_transmit_it(); break;
        case I2C_TEST_STM_MASTER_RX: test_i2c_master_receive_it(); break;
        case I2C_TEST_STM_SLAVE_TX:  test_i2c_slave_transmit_it(); break;
        case I2C_TEST_STM_SLAVE_RX:  test_i2c_slave_receive_it(); break;
        }
    } else {
        ASSERT(false);
    }
}

static void i2c1_gpio_init(void)
{
    gpio_handle i2c1_pins            = {0};
    i2c1_pins.gpio_conf.it_trigger   = GPIO_IT_NA;
    i2c1_pins.gpio_conf.mode         = GPIO_MODE_ALT_FN;
    i2c1_pins.gpio_conf.output_speed = GPIO_OSPEED_MEDIUM;

    i2c1_pins.gpio_conf.alt_fn_no       = GPIO_ALT_FN_4;
    i2c1_pins.gpio_conf.output_type     = GPIO_OPTYPE_OPEN_DRAIN;
    i2c1_pins.gpio_conf.pullup_pulldown = GPIO_PULL_UP;

    // PB6 - SCL
    i2c1_pins.p_gpiox          = GPIOB;
    i2c1_pins.gpio_conf.pin_no = PIN_NO_6;
    gpio_init(&i2c1_pins);

    // PB7 - SDA
    i2c1_pins.p_gpiox          = GPIOB;
    i2c1_pins.gpio_conf.pin_no = PIN_NO_7;
    gpio_init(&i2c1_pins);
}

static void i2c1_init(void)
{
    g_i2c_handle.p_i2cx                 = I2C1;
    g_i2c_handle.i2c_conf.clock_freq_hz = I2C_CLK_FREQ_100KHZ;
    g_i2c_handle.i2c_conf.fm_duty_cycle = I2C_FM_DUTY_2;
    g_i2c_handle.i2c_conf.own_address   = I2C_STM_ADDR;
    g_i2c_handle.i2c_conf.speed_mode    = I2C_SPEED_MODE_STANDARD;
    i2c_init(&g_i2c_handle);
}

static void test_i2c_master_transmit(void)
{
    uint8_t tx_data[] = "STM master transmit";
    uint8_t tx_len    = sizeof(tx_data) - 1;

    while (1) {
        i2c_master_transmit(I2C1, I2C_ARDUINO_ADDR, tx_data, tx_len, I2C_REPEATED_START_DISABLE);
        for (uint32_t i = 0; i < 5000; i++);
    }
}

static void test_i2c_slave_transmit(void)
{
    uint8_t tx_data[] = "STM slave transmit";
    uint8_t tx_len    = sizeof(tx_data) - 1;

    while (1) {
        i2c_slave_transmit(I2C1, tx_data, tx_len);
    }
}

static void test_i2c_master_receive(void)
{
    uint8_t expected_string[] = "Arduino->STM";
    uint8_t rx_len            = sizeof(expected_string) - 1;
    uint8_t rx_data[32];

    while (1) {
        i2c_master_receive(I2C1, I2C_ARDUINO_ADDR, rx_data, rx_len, I2C_REPEATED_START_DISABLE);
        for (uint32_t i = 0; i < 5000; i++);
    }
}

static void test_i2c_slave_receive(void)
{
    uint8_t expected_string[] = "Arduino->STM";
    uint8_t rx_len            = sizeof(expected_string) - 1;
    uint8_t rx_data[32];

    while (1) {
        i2c_slave_receive(I2C1, rx_data, rx_len);
    }
}
static void test_i2c_master_transmit_it(void)
{
    static uint8_t tx_data[] = "STM master transmit";
    static uint8_t tx_len    = sizeof(tx_data) - 1;

    while (1) {
        i2c_master_transmit_it(&g_i2c_handle, I2C_ARDUINO_ADDR, tx_data, tx_len, I2C_REPEATED_START_DISABLE);
        for (uint32_t i = 0; i < 5000; i++);
    }
}
static void test_i2c_slave_transmit_it(void)
{
    static uint8_t tx_data[] = "STM slave transmit";
    static uint8_t tx_len    = sizeof(tx_data) - 1;

    while (1) {
        i2c_slave_transmit_it(&g_i2c_handle, tx_data, tx_len);
    }
}

static void test_i2c_master_receive_it(void)
{
    static uint8_t expected_string[] = "Arduino->STM";
    static uint8_t rx_len            = sizeof(expected_string) - 1;
    static uint8_t rx_data[32];

    while (1) {
        i2c_master_receive_it(&g_i2c_handle, I2C_ARDUINO_ADDR, rx_data, rx_len, I2C_REPEATED_START_DISABLE);
        for (uint32_t i = 0; i < 5000; i++);
    }
}

static void test_i2c_slave_receive_it(void)
{
    static uint8_t expected_string[] = "Arduino->STM";
    static uint8_t rx_len            = sizeof(expected_string) - 1;
    static uint8_t rx_data[32];

    while (1) {
        i2c_slave_receive_it(&g_i2c_handle, rx_data, rx_len);
    }
}
void I2C1_EV_IRQHandler()
{
    i2c_it_handler(&g_i2c_handle);
}

void I2C1_ER_IRQHandler()
{
    ASSERT(false);
}
