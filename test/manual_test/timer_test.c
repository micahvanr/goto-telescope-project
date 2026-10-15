//======================================================================================//{
//  File: timer_test.c
//  Overview: Tests some of the timer's functionality.
//  Tests:
//      - Auto setup
//      - Manual setup
//      - Frequency setup
//      - Delay function
//      - Output compare PWM
//      - Input capture time difference (using interrupts)
//      - Basic interrupt
//  Notes:
//======================================================================================//}

//} Includes
//=========================================//{

#include "timer_test.h"
#include "debug_tools.h"
#include "gpio.h"
#include "printf.h"
#include "rcc.h"
#include "stm32f4xx.h"
#include "tim.h"
#include <stdint.h>

//======================================================================================//}
//                  Private Constants
//======================================================================================//{

#define TEST_TIMER_PORT    TIM1
#define TEST_CC_TIMER_PORT TIM8

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

tim_handle g_tim_handle    = {0};
tim_handle g_cc_tim_handle = {0};

//======================================================================================//}
//                  Private Function Prototypes
//======================================================================================//{

static void capture_compare_timer_setup(void);
static void advanced_timer_setup(void);
static void timer_oc_gpio_pin_setup(void);
static void timer_ic_gpio_pin_setup(void);

uint32_t count_to_period(uint32_t count);

//======================================================================================//}
//                  Public Test Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: test_timer_auto
//  Overview: Test timer auto set time functionality with logic analyzer.
//  Test goals:
//      - Timer initialization
//      - Auto time setup
//  Setup:
//      - PA2 -> logic analyzer: read printf
//      - PA3 -> logic analyzer: read debug pin
//  Expectations: Debug pin should toggle roughly every 100 ms.
//  Note: None
//============================================================================//}
void test_timer_auto(void)
{
    advanced_timer_setup();

    g_tim_handle.timing_conf.time = 100;
    g_tim_handle.timing_conf.unit = TIM_UNIT_MS;

    tim_init(&g_tim_handle);
    printf_("First count: %lu", g_tim_handle.p_timx->CNT);
    tim_start(g_tim_handle.p_timx);

    while (1)
    {
        while (tim_read_status(g_tim_handle.p_timx) != TIM_UPDATE_FOUND);
        tim_reset_status(g_tim_handle.p_timx);
        toggle_debug_pin();
    }
}

//============================================================================//{
//  Function: test_timer_manual
//  Overview: Test timer manual set time functionality with logic analyzer.
//  Test goals:
//      - Timer initialization with auto reload (count value).
//      - Manual time setup
//  Setup:
//      - PA2 -> logic analyzer: read printf
//      - PA3 -> logic analyzer: read debug pin
//  Expectations: Debug pin should toggle every roughly 1 second.
//  Note: None
//============================================================================//}
void test_timer_manual(void)
{
    advanced_timer_setup();

    g_tim_handle.timing_conf.auto_reload = 10000;
    g_tim_handle.timing_conf.prescaler   = 1600;

    tim_init(&g_tim_handle);
    tim_start(g_tim_handle.p_timx);

    while (1)
    {
        while (tim_read_status(g_tim_handle.p_timx) != TIM_UPDATE_FOUND);
        tim_reset_status(g_tim_handle.p_timx);
        toggle_debug_pin();
    }
}

//============================================================================//{
//  Function: test_timer_freq
//  Overview: Test timer frequency time set with logic analyzer.
//  Test goals:
//      - Timer initialization
//      - Frequency timing setup
//  Setup:
//      - PA2 -> logic analyzer: read printf
//      - PA3 -> logic analyzer: read debug pin
//  Expectations: Debug pin should toggle to create a 1 khz frequency.
//  Note: The frequency is how often it is toggled, not it's period.
//============================================================================//}
void test_timer_freq(void)
{
    advanced_timer_setup();

    g_tim_handle.timing_conf.time = 1;
    g_tim_handle.timing_conf.unit = TIM_UNIT_KHZ;

    tim_init(&g_tim_handle);
    tim_start(g_tim_handle.p_timx);

    while (1)
    {
        while (tim_read_status(g_tim_handle.p_timx) != TIM_UPDATE_FOUND);
        tim_reset_status(g_tim_handle.p_timx);
        toggle_debug_pin();
    }
}

//============================================================================//{
//  Function: test_timer_delay
//  Overview: Test timer delay functionwith logic analyzer.
//  Test goals:
//      - Timer delay function
//  Setup:
//      - PA3 -> logic analyzer: read debug pin
//  Expectations: Debug pin should toggle every 1 second.
//  Note: None
//============================================================================//}
void test_delay(void)
{
    while (1)
    {
        tim_delay(1, TIM_UNIT_S);
        toggle_debug_pin();
        // tim_delay(100, TIM_UNIT_MS);
        // toggle_debug_pin();
        // tim_delay(110, TIM_UNIT_US);
        // toggle_debug_pin();
    }
}

//============================================================================//{
//  Function: test_oc_pwm
//  Overview: Test timer output compare PWM mode with logic analyzer.
//  Test goals:
//      - Timer initialization with output compare
//      - Output compare PWM mode
//  Setup:
//      - PA3 -> logic analyzer: read debug pin
//  Expectations: Debug pin should output a PWM signal with 75% duty cycle and a period of 1 second.
//  Note: None
//============================================================================//}
void test_oc_pwm(void)
{
    advanced_timer_setup();

    timer_oc_gpio_pin_setup();

    g_tim_handle.timing_conf.auto_reload = 10000;
    g_tim_handle.timing_conf.prescaler   = 1600;
    tim_init(&g_tim_handle);

    tim_oc_config pwm_config = {0};
    pwm_config.arr_preload   = TIM_ARR_PRELOAD_EN;
    pwm_config.channel_num   = TIM_CHANNEL_SEL_1;
    pwm_config.output_mode   = TIM_OC_OPM_PWM_1;
    pwm_config.duty_cycle    = 75;
    tim_oc_init(g_tim_handle.p_timx, pwm_config);

    tim_channel_start(g_tim_handle.p_timx, pwm_config.channel_num);
    tim_start(g_tim_handle.p_timx);

    while (1)
    {
    }
}

//============================================================================//{
//  Function: test_ic_it
//  Overview: Test timer input capture interrupt with logic analyzer and Arduino.
//  Test goals:
//      - Timer initialization with input capture.
//      - Timer difference between input captures.
//  Setup:
//      - STM32:
//          - PA2 -> logic analyzer: read printf
//          - PA3 -> Arduino D0: get Arduino output
//          - PA3 -> logic analyzer: view Arduino output
//  Expectations: When pin is toggled, printf should output previous count, next count,
//  and the difference. Difference should be the 100 ms.
//  Note: None
//============================================================================//}
void test_ic_it(void)
{
    capture_compare_timer_setup();

    timer_ic_gpio_pin_setup();

    g_cc_tim_handle.timing_conf.auto_reload = 10000;
    g_cc_tim_handle.timing_conf.prescaler   = 800;
    tim_init(&g_cc_tim_handle);

    tim_ic_config ic_config  = {0};
    ic_config.channel_num    = TIM_CHANNEL_SEL_1;
    ic_config.edge_detection = TIM_IC_EDGE_DETECTION_RISING_FALLING;
    ic_config.input_map      = TIM_INPUT_MAP_MAIN;
    tim_ic_init(g_cc_tim_handle.p_timx, ic_config);

    tim_start(g_cc_tim_handle.p_timx);
    tim_channel_start_it(g_cc_tim_handle.p_timx, ic_config.channel_num);

    while (1);
}

//============================================================================//{
//  Function: test_it
//  Overview: Test timer interrupts with logic analyzer.
//  Test goals:
//      - Timer initialization with interrupts
//  Setup:
//      - PA3 -> logic analyzer: read debug pin
//  Expectations: Debug pin should toggle roughly every 100 ms.
//  Note: None
//============================================================================//}
void test_it(void)
{
    advanced_timer_setup();

    g_tim_handle.timing_conf.time = 100;
    g_tim_handle.timing_conf.unit = TIM_UNIT_MS;
    tim_init(&g_tim_handle);

    tim_start_it(g_tim_handle.p_timx);
    while (1);
}
//======================================================================================//}
//                  Private Function Implementation
//======================================================================================//{

static void capture_compare_timer_setup(void)
{
    g_cc_tim_handle.p_timx                  = TIM8;
    g_cc_tim_handle.tim_conf.clock_sel      = TIM_CLK_SEL_INTERNAL;
    g_cc_tim_handle.tim_conf.one_pulse_mode = TIM_ONE_PULSE_MODE_DI;
    g_cc_tim_handle.tim_conf.preload        = TIM_ARR_PRELOAD_EN;
    g_cc_tim_handle.tim_conf.align_mode     = TIM_ALIGN_EDGE;

    tim_init(&g_tim_handle);
}

static void advanced_timer_setup(void)
{
    g_tim_handle.p_timx                  = TIM1;
    g_tim_handle.tim_conf.clock_sel      = TIM_CLK_SEL_INTERNAL;
    g_tim_handle.tim_conf.one_pulse_mode = TIM_ONE_PULSE_MODE_DI;
    g_tim_handle.tim_conf.preload        = TIM_ARR_PRELOAD_EN;
    g_tim_handle.tim_conf.align_mode     = TIM_ALIGN_EDGE;

    tim_init(&g_tim_handle);
}

static void timer_oc_gpio_pin_setup(void)
{
    // PE9 - TIM1_CH1
    gpio_handle tim2_ch3_pin           = {0};
    tim2_ch3_pin.p_gpiox               = GPIOE;
    tim2_ch3_pin.gpio_conf.pin_num     = PIN_NUM_9;
    tim2_ch3_pin.gpio_conf.output_type = GPIO_OPTYPE_PUSH_PULL;
    tim2_ch3_pin.gpio_conf.mode        = GPIO_MODE_ALT_FN;
    tim2_ch3_pin.gpio_conf.alt_fn_num  = GPIO_ALT_FN_1;
    gpio_init(&tim2_ch3_pin);
}

static void timer_ic_gpio_pin_setup(void)
{
    // PC6 - TIM1_CH1
    gpio_handle tim2_ch3_pin          = {0};
    tim2_ch3_pin.p_gpiox              = GPIOC;
    tim2_ch3_pin.gpio_conf.pin_num    = PIN_NUM_6;
    tim2_ch3_pin.gpio_conf.mode       = GPIO_MODE_ALT_FN;
    tim2_ch3_pin.gpio_conf.alt_fn_num = GPIO_ALT_FN_3;
    gpio_init(&tim2_ch3_pin);
}

uint32_t count_to_period(uint32_t count)
{
    uint32_t base_freq_hz;
    uint32_t timer_prescaler;
    uint32_t timer_freq_hz;
    uint32_t period;
    uint32_t const TIME_MS = 1000;

    base_freq_hz    = rcc_get_timer_clock_freq_hz(tim_get_bus(g_cc_tim_handle.p_timx));
    timer_prescaler = g_cc_tim_handle.p_timx->PSC + 1;
    timer_freq_hz   = base_freq_hz / timer_prescaler;
    period          = (count * TIME_MS) / timer_freq_hz;

    return period;
}

//======================================================================================//}
//                  ISR Handlers
//======================================================================================//{

void TIM8_CC_IRQHandler(void)
{
    uint32_t curr_value;
    static uint32_t prev_value = 0;
    uint32_t arr_value;

    arr_value  = g_cc_tim_handle.p_timx->ARR;
    curr_value = tim_get_channel_ccr(g_cc_tim_handle.p_timx, TIM_CHANNEL_SEL_1);
    printf_("Prev: %lu", prev_value);
    printf_("Curr: %lu", curr_value);
    if (prev_value != 0)
    {
        uint32_t diff;
        if (prev_value > curr_value)
        {
            curr_value -= arr_value / 2;
            prev_value -= arr_value / 2;
        }
        diff = curr_value - prev_value;
        diff = count_to_period(diff);
        printf_("Diff: %lu", diff);
    }
    prev_value = curr_value;
    tim_reset_status(g_cc_tim_handle.p_timx);
}

void TIM1_UP_TIM10_IRQHandler(void)
{
    toggle_debug_pin();
    tim_reset_status(g_tim_handle.p_timx);
}

// *End of file*
