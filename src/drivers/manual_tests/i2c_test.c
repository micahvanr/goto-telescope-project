#include "i2c_test.h"
#include "assert_handler.h"
#include "gpio.h"
#include "i2c.h"
#include "rcc.h"

static void i2c1_gpio_init(void);
static void i2c1_init(void);
static void i2c1_master_transmit(void);
static void i2c1_slave_transmit(void);
static void i2c1_master_receive(void);
static void i2c1_slave_receive(void);
static void i2c1_master_transmit_it(void);
static void i2c1_slave_transmit_it(void);
static void i2c1_master_receive_it(void);
static void i2c1_slave_receive_it(void);

i2c_handle g_i2c_handle = {0};

void i2c_tests(test_type_e test)
{
    rcc_mco_config(RCC_MCO1_HSI_SRC, RCC_MCO_PRE_4);
    i2c1_gpio_init();
    i2c1_init();
    switch (test) {
    case I2C_TEST_MASTER_TRANSMIT:    i2c1_master_transmit(); break;
    case I2C_TEST_MASTER_RECEIVE:     i2c1_master_receive(); break;
    case I2C_TEST_SLAVE_TRANSMIT:     i2c1_slave_transmit(); break;
    case I2C_TEST_SLAVE_RECEIVE:      i2c1_slave_receive(); break;
    case I2C_TEST_MASTER_TRANSMIT_IT: i2c1_master_transmit_it(); break;
    case I2C_TEST_MASTER_RECEIVE_IT:  i2c1_master_receive_it(); break;
    case I2C_TEST_SLAVE_TRANSMIT_IT:  i2c1_slave_transmit_it(); break;
    case I2C_TEST_SLAVE_RECEIVE_IT:   i2c1_slave_receive_it(); break;
    default:                          ASSERT(false);
    }

    UNUSED(test);
}

static void i2c1_gpio_init(void)
{
    gpio_handle i2c3_pins            = {0};
    i2c3_pins.gpio_conf.it_trigger   = GPIO_IT_NA;
    i2c3_pins.gpio_conf.mode         = GPIO_MODE_ALT_FN;
    i2c3_pins.gpio_conf.output_speed = GPIO_OSPEED_MEDIUM;

    i2c3_pins.gpio_conf.alt_fn_no       = GPIO_ALT_FN_4;
    i2c3_pins.gpio_conf.output_type     = GPIO_OPTYPE_OPEN_DRAIN;
    i2c3_pins.gpio_conf.pullup_pulldown = GPIO_PULL_UP;

    // PB6 - SCL
    i2c3_pins.p_gpiox          = GPIOB;
    i2c3_pins.gpio_conf.pin_no = PIN_NO_6;
    gpio_init(&i2c3_pins);

    // PB7 - SDA
    i2c3_pins.p_gpiox          = GPIOB;
    i2c3_pins.gpio_conf.pin_no = PIN_NO_7;
    gpio_init(&i2c3_pins);
}

static void i2c1_init(void)
{
    g_i2c_handle.p_i2cx                 = I2C1;
    g_i2c_handle.i2c_conf.clock_freq_hz = I2C_CLK_FREQ_100KHZ;
    g_i2c_handle.i2c_conf.fm_duty_cycle = I2C_FM_DUTY_2;
    g_i2c_handle.i2c_conf.own_address   = 0x42;
    g_i2c_handle.i2c_conf.speed_mode    = I2C_SPEED_MODE_STANDARD;
    i2c_init(&g_i2c_handle);
}

static void i2c1_master_transmit(void)
{
    // uint8_t data          = 0x33;
    uint8_t string_test[] = "Testing string";

    while (1) {
        // i2c_master_transmit(I2C1, TARGET_ADDR, &data, 1, I2C_REPEATED_START_ENABLE);
        i2c_master_transmit(I2C1, TARGET_ADDR, string_test, sizeof(string_test) - 1, I2C_REPEATED_START_ENABLE);
    }
}

static void i2c1_slave_transmit(void)
{
    uint8_t command_read;
    uint8_t string_test[]  = "Testing string";
    uint8_t length_of_data = sizeof(string_test) - 1;
    while (1) {
        i2c_slave_receive(I2C1, &command_read, 1);

        switch ((i2c_test_enums)command_read) {
        case (COMMAND_LEN):  i2c_slave_transmit(I2C1, &length_of_data, 1); break;
        case (COMMAND_READ): i2c_slave_transmit(I2C1, string_test, length_of_data); break;
        default:             ASSERT(false);
        }
    }
}
static void i2c1_master_receive(void)
{
    // Get single byte
    // Get multiple bytes

    uint8_t command;
    uint8_t len_of_data    = 0;
    uint8_t multi_byte[50] = {0};
    while (1) {
        command = COMMAND_LEN;
        i2c_master_transmit(I2C1, TARGET_ADDR, &command, 1, I2C_REPEATED_START_DISABLE);
        i2c_master_receive(I2C1, TARGET_ADDR, &len_of_data, 1, I2C_REPEATED_START_DISABLE);
        if (len_of_data > 50) {
            ASSERT(false);
        }
        command = COMMAND_READ;
        i2c_master_transmit(I2C1, TARGET_ADDR, &command, 1, I2C_REPEATED_START_DISABLE);
        i2c_master_receive(I2C1, TARGET_ADDR, multi_byte, len_of_data, I2C_REPEATED_START_DISABLE);
    }
}
static void i2c1_slave_receive(void)
{
}
static void i2c1_master_transmit_it(void)
{
    static uint8_t string_test[] = "Testing string";

    while (1) {
        printf_("Testing printf from i2c");
        i2c_master_transmit_it(&g_i2c_handle, TARGET_ADDR, string_test, sizeof(string_test) - 1,
                               I2C_REPEATED_START_ENABLE);
        for (uint32_t i = 0; i < 50000; i++);
        i2c_master_transmit_it(&g_i2c_handle, TARGET_ADDR, string_test, sizeof(string_test) - 1,
                               I2C_REPEATED_START_DISABLE);
    }
}
static void i2c1_slave_transmit_it(void)
{
    uint8_t command_read;
    static uint8_t string_test[] = "Testing string";
    uint8_t length_of_data       = sizeof(string_test) - 1;
    while (1) {
        i2c_slave_receive(g_i2c_handle.p_i2cx, &command_read, 1);

        switch ((i2c_test_enums)command_read) {
        case (COMMAND_LEN):  i2c_slave_transmit_it(&g_i2c_handle, &length_of_data, 1); break;
        case (COMMAND_READ): i2c_slave_transmit_it(&g_i2c_handle, string_test, length_of_data); break;
        default:             ;
        }
    }
}

static void i2c1_master_receive_it(void)
{
    static uint8_t command;
    static uint8_t len_of_data    = 0;
    static uint8_t multi_byte[50] = {0};
    while (1) {
        command = COMMAND_LEN;
        i2c_master_transmit_it(&g_i2c_handle, TARGET_ADDR, &command, 1, I2C_REPEATED_START_DISABLE);
        i2c_master_receive_it(&g_i2c_handle, TARGET_ADDR, &len_of_data, 1, I2C_REPEATED_START_DISABLE);
        if (len_of_data > 50) {
            ASSERT(false);
        }
        command = COMMAND_READ;
        i2c_master_transmit_it(&g_i2c_handle, TARGET_ADDR, &command, 1, I2C_REPEATED_START_DISABLE);
        i2c_master_receive_it(&g_i2c_handle, TARGET_ADDR, multi_byte, len_of_data, I2C_REPEATED_START_DISABLE);
    }
}
static void i2c1_slave_receive_it(void)
{
}
void I2C1_EV_IRQHandler()
{
    i2c_it_handler(&g_i2c_handle);
}

void I2C1_ER_IRQHandler()
{
    ASSERT(false);
}
