#include "timer_test.h"
#include "assert_handler.h"
#include "common.h"
#include "debug_tools.h"
#include "printf.h"
#include "tim.h"

static void test_timer_auto(void);
static void test_delay(void);

void timer_tests(test_type_e test)
{
    test_delay();
    test_timer_auto();

    UNUSED(test);
}

static void test_timer_auto(void)
{
    tim_handler test_timer             = {0};
    test_timer.tim_conf.clock_sel      = TIM_CLK_SEL_INTERNAL;
    test_timer.tim_conf.one_pulse_mode = TIM_ONE_PULSE_MODE_DI;
    test_timer.tim_conf.preload        = TIM_ARR_PRELOAD_EN;
    tim_set_auto(&test_timer, 1, TIM_UNIT_S);

    while (1) {
        while (tim_read_status(TIM6) != TIM_UPDATE_FOUND);
        tim_reset_status(TIM6);
        toggle_debug_pin();
    }
}
static void test_delay(void)
{
    while (1) {
        tim_delay(1, TIM_UNIT_S);
        toggle_debug_pin();
        tim_delay(100, TIM_UNIT_MS);
        toggle_debug_pin();
        tim_delay(100, TIM_UNIT_US);
        toggle_debug_pin();
    }
}
