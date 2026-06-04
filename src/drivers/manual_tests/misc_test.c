#include "misc_test.h"
#include "assert_handler.h"
#include "rcc.h"

static void test_assert(void);
static void test_get_clock(void);
static void test_printf(void);

void misc_tests(test_type_e test)
{
    switch (test) {
    case MISC_TEST_ASSERT:    test_assert(); break;
    case MISC_TEST_GET_CLOCK: test_get_clock(); break;
    case MISC_TEST_PRINTF:    test_printf(); break;
    default:                  ASSERT(FALSE);
    }
}

static void test_assert(void)
{
    ASSERT(1);
    ASSERT(0);
}

static void test_get_clock(void)
{
    uint32_t ahb1_freq = rcc_get_bus_clock_freq_hz(AHB1_BUS);
    uint32_t apb1_freq = rcc_get_bus_clock_freq_hz(APB1_BUS);
    uint32_t apb2_freq = rcc_get_bus_clock_freq_hz(APB2_BUS);
    UNUSED(ahb1_freq);
    UNUSED(apb1_freq);
    UNUSED(apb2_freq);
}

static void test_printf(void)
{
    while (1) {
        printf_("Test");
        for (uint32_t i = 0; i < 50000; i++);
    }
}
