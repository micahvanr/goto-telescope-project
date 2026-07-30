#include "timer_test.h"
#include "assert_handler.h"
#include "common.h"
#include "debug_tools.h"
#include "printf.h"
#include "timer.h"

static void test_timing(void);
static void test_delay(void);

void timer_tests(test_type_e test)
{
    test_delay();
    test_timing();

    UNUSED(test);
}



static void test_timing(void) 
{
    timer_set_us(TIM6, 1);

    while (1) {
        while (timer_read_status(TIM6) != TIMER_UPDATE_FOUND);
        timer_reset_status(TIM6);
        toggle_debug_pin();
    }
}
static void test_delay(void)
{
    while (1) {
        timer_delay_sec(1);
        toggle_debug_pin();
        timer_delay_ms(100);
        toggle_debug_pin();
        timer_delay_us(100);
        toggle_debug_pin();
    }
}
