#include "timer_test.h"
#include "assert_handler.h"
#include "common.h"
#include "debug_tools.h"
#include "gpio.h"
#include "printf.h"
#include "rcc.h"
#include "stm32f4xx.h"
#include "tim.h"
#include <stdint.h>

static void test_timer_auto(void);
static void test_timer_manual(void);
static void test_timer_freq(void);
static void test_delay(void);
static void test_oc_pwm(void);
static void test_ic_it(void);
static void test_it(void);

static void timer8_init(void);
static void timer1_init(void);
static void timer_oc_gpio_pin_init(void);
static void timer_ic_gpio_pin_init(void);

uint32_t count_to_period(uint32_t count);

tim_handler g_tim1_handle = {0};
tim_handler g_tim8_handle = {0};

void timer_tests(test_type_e test)
{
    timer1_init();
    timer8_init();
    // while(1) {
    //     toggle_debug_pin();
    //     for (uint32_t i = 0; i < 5000; i++);
    // }

    switch (test) {
    case TIMER_TEST_AUTO:     test_timer_auto(); break;
    case TIMER_TEST_MANUAL:   test_timer_manual(); break;
    case TIMER_TEST_FREQ:     test_timer_freq(); break;
    case TIMER_TEST_DELAY:    test_delay(); break;
    case TIMER_TEST_OC_PWM:   test_oc_pwm(); break;
    case TIMER_TEST_IC_IT:    test_ic_it(); break;
    case TIMER_TEST_BASIC_IT: test_it(); break;

    default:                  ASSERT(false);
    }

    UNUSED(test);
}

static void timer8_init(void)
{
    g_tim8_handle.p_timx                  = TIM8;
    g_tim8_handle.tim_conf.clock_sel      = TIM_CLK_SEL_INTERNAL;
    g_tim8_handle.tim_conf.one_pulse_mode = TIM_ONE_PULSE_MODE_DI;
    g_tim8_handle.tim_conf.preload        = TIM_ARR_PRELOAD_EN;
    g_tim8_handle.tim_conf.align_mode     = TIM_ALIGN_EDGE;

    tim_init(&g_tim1_handle);
}

static void timer1_init(void)
{
    g_tim1_handle.p_timx                  = TIM1;
    g_tim1_handle.tim_conf.clock_sel      = TIM_CLK_SEL_INTERNAL;
    g_tim1_handle.tim_conf.one_pulse_mode = TIM_ONE_PULSE_MODE_DI;
    g_tim1_handle.tim_conf.preload        = TIM_ARR_PRELOAD_EN;
    g_tim1_handle.tim_conf.align_mode     = TIM_ALIGN_EDGE;

    tim_init(&g_tim1_handle);
}

static void timer_oc_gpio_pin_init(void)
{
    // PE9 - TIM1_CH1
    gpio_handle tim2_ch3_pin           = {0};
    tim2_ch3_pin.p_gpiox               = GPIOE;
    tim2_ch3_pin.gpio_conf.pin_no      = PIN_NO_9;
    tim2_ch3_pin.gpio_conf.output_type = GPIO_OPTYPE_PUSH_PULL;
    tim2_ch3_pin.gpio_conf.mode        = GPIO_MODE_ALT_FN;
    tim2_ch3_pin.gpio_conf.alt_fn_no   = GPIO_ALT_FN_1;
    gpio_init(&tim2_ch3_pin);
}

static void timer_ic_gpio_pin_init(void)
{
    // PC6 - TIM1_CH1
    gpio_handle tim2_ch3_pin         = {0};
    tim2_ch3_pin.p_gpiox             = GPIOC;
    tim2_ch3_pin.gpio_conf.pin_no    = PIN_NO_6;
    tim2_ch3_pin.gpio_conf.mode      = GPIO_MODE_ALT_FN;
    tim2_ch3_pin.gpio_conf.alt_fn_no = GPIO_ALT_FN_3;
    gpio_init(&tim2_ch3_pin);
}

static void test_timer_auto(void)
{
    g_tim1_handle.timing_conf.time = 100;
    g_tim1_handle.timing_conf.unit = TIM_UNIT_MS;

    tim_init(&g_tim1_handle);
    printf_("First count: %lu", g_tim1_handle.p_timx->CNT);
    tim_start(g_tim1_handle.p_timx);

    while (1) {
        while (tim_read_base_status(g_tim1_handle.p_timx) != TIM_UPDATE_FOUND);
        tim_reset_base_status(g_tim1_handle.p_timx);
        toggle_debug_pin();
    }
}
static void test_timer_manual(void)
{

    g_tim1_handle.timing_conf.auto_reload = 10000;
    g_tim1_handle.timing_conf.prescaler   = 1600;

    tim_init(&g_tim1_handle);
    tim_start(g_tim1_handle.p_timx);

    while (1) {
        while (tim_read_base_status(g_tim1_handle.p_timx) != TIM_UPDATE_FOUND);
        tim_reset_base_status(g_tim1_handle.p_timx);
        toggle_debug_pin();
    }
}

static void test_timer_freq(void)
{
    g_tim1_handle.timing_conf.time = 1;
    g_tim1_handle.timing_conf.unit = TIM_UNIT_KHZ;

    tim_init(&g_tim1_handle);
    tim_start(g_tim1_handle.p_timx);

    while (1) {
        while (tim_read_base_status(g_tim1_handle.p_timx) != TIM_UPDATE_FOUND);
        tim_reset_base_status(g_tim1_handle.p_timx);
        toggle_debug_pin();
    }
}

static void test_delay(void)
{
    while (1) {
        // tim_delay(1, TIM_UNIT_S);
        // toggle_debug_pin();
        tim_delay(100, TIM_UNIT_MS);
        toggle_debug_pin();
        // tim_delay(110, TIM_UNIT_US);
        // toggle_debug_pin();
    }
}

static void test_oc_pwm(void)
{
    timer_oc_gpio_pin_init();

    g_tim1_handle.timing_conf.auto_reload = 10000;
    g_tim1_handle.timing_conf.prescaler   = 1600;
    tim_init(&g_tim1_handle);

    tim_oc_config pwm_config = {0};
    pwm_config.arr_preload   = TIM_ARR_PRELOAD_EN;
    pwm_config.channel_num   = TIM_CHANNEL_SEL_1;
    pwm_config.output_mode   = TIM_OC_OPM_PWM_1;
    pwm_config.duty_cycle    = 75;
    tim_oc_init(g_tim1_handle.p_timx, pwm_config);

    tim_channel_start(g_tim1_handle.p_timx, pwm_config.channel_num);
    tim_start(g_tim1_handle.p_timx);

    while (1) {
        // while (tim_read_base_status(g_tim1_handle.p_timx) != TIM_UPDATE_FOUND);
        // tim_reset_base_status(g_tim1_handle.p_timx);
        // toggle_debug_pin();
    }
}

static void test_ic_it(void)
{
    timer_ic_gpio_pin_init();

    g_tim8_handle.timing_conf.auto_reload = 10000;
    g_tim8_handle.timing_conf.prescaler   = 800;
    tim_init(&g_tim8_handle);

    tim_ic_config ic_config  = {0};
    ic_config.channel_num    = TIM_CHANNEL_SEL_1;
    ic_config.edge_detection = TIM_IC_EDGE_DETECTION_RISING_FALLING;
    ic_config.input_map      = TIM_INPUT_MAP_MAIN;
    tim_ic_init(g_tim8_handle.p_timx, ic_config);

    tim_start(g_tim8_handle.p_timx);
    tim_channel_start_it(g_tim8_handle.p_timx, ic_config.channel_num);

    while (1);
}

static void test_it(void)
{
    g_tim1_handle.timing_conf.time = 100;
    g_tim1_handle.timing_conf.unit = TIM_UNIT_MS;
    tim_init(&g_tim1_handle);

    tim_start_it(g_tim1_handle.p_timx);
    while (1);
}

void TIM8_CC_IRQHandler(void)
{
    uint32_t curr_value;
    static uint32_t prev_value = 0;
    uint32_t arr_value;

    arr_value  = g_tim8_handle.p_timx->ARR;
    curr_value = tim_get_channel_ccr(g_tim8_handle.p_timx, TIM_CHANNEL_SEL_1);
    printf_("Prev: %lu", prev_value);
    printf_("Curr: %lu", curr_value);
    if (prev_value != 0) {
        uint32_t diff;
        if (prev_value > curr_value) {
            curr_value -= arr_value / 2;
            prev_value -= arr_value / 2;
        }
        diff = curr_value - prev_value;
        diff = count_to_period(diff);
        printf_("Diff: %lu", diff);
    }
    prev_value = curr_value;
    tim_reset_base_status(g_tim8_handle.p_timx);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
    toggle_debug_pin();
    tim_reset_base_status(g_tim1_handle.p_timx);
}

uint32_t count_to_period(uint32_t count)
{
    uint32_t base_freq_hz;
    uint32_t timer_prescaler;
    uint32_t timer_freq_hz;
    uint32_t period;
    uint32_t const TIME_MS = 1000;

    base_freq_hz    = rcc_get_timer_clock_freq_hz(get_tim_bus(g_tim8_handle.p_timx));
    timer_prescaler = g_tim8_handle.p_timx->PSC + 1;
    timer_freq_hz   = base_freq_hz / timer_prescaler;
    period          = (count * TIME_MS) / timer_freq_hz;

    return period;
}
