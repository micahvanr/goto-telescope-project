#include "rtc_test.h"
#include "assert_handler.h"
#include "common.h"
#include "debug_tools.h"
#include "gpio.h"
#include "printf.h"
#include "rcc.h"
#include "rtc.h"
#include "stm32f4xx.h"
#include "test_types.h"
#include "tim.h"
#include <time.h>

enum {
    RTC_TEST_TIME = 30,
} rtc_test_e;

rtc_handler g_rtc_handle = {0};

static void standard_rtc_init(void);
static void standard_output_rtc_data(void);

static void test_rtc_base(void);
static void test_rtc_alarm(void);
static void test_rtc_timestamp(void);
static void test_rtc_wakeup(void);

void rtc_tests(test_type_e test)
{
    switch (test) {
    case RTC_TEST_BASE:      test_rtc_base(); break;
    case RTC_TEST_ALARM:     test_rtc_alarm(); break;
    case RTC_TEST_TIMESTAMP: test_rtc_timestamp(); break;
    case RTC_TEST_WAKEUP:    test_rtc_wakeup(); break;
    default:                 ASSERT(false);
    }
}

static void standard_rtc_init(void)
{
    g_rtc_handle.rtc_conf.rtc_sel = RCC_RTC_CLK_SRC_LSI;

    g_rtc_handle.date_data.date    = 1;
    g_rtc_handle.date_data.month   = 1;
    g_rtc_handle.date_data.year    = 26;
    g_rtc_handle.date_data.weekday = RTC_WEEKDAY_THURSDAY;

    g_rtc_handle.time_data.time_format = RTC_TIME_FORMAT_12HR;
    g_rtc_handle.time_data.time_am_pm  = RTC_TIME_PM;
    g_rtc_handle.time_data.hours       = 11;
    g_rtc_handle.time_data.minutes     = 59;
    g_rtc_handle.time_data.seconds     = RTC_TEST_TIME;
    rtc_init(&g_rtc_handle);
}

static void standard_output_rtc_data(void)
{
    uint8_t am_pm[]        = "AM";
    g_rtc_handle.time_data = rtc_get_time_data();
    g_rtc_handle.date_data = rtc_get_date_data();

    if (g_rtc_handle.time_data.time_am_pm == RTC_TIME_AM) {
        am_pm[0] = 'A';
    } else if (g_rtc_handle.time_data.time_am_pm == RTC_TIME_PM) {
        am_pm[0] = 'P';
    }
    printf_("Time: %d:%d:%d %s\n", g_rtc_handle.time_data.hours, g_rtc_handle.time_data.minutes,
            g_rtc_handle.time_data.seconds, am_pm);
    printf_("Date: %d/%d/%d\n", g_rtc_handle.date_data.month, g_rtc_handle.date_data.date, g_rtc_handle.date_data.year);
}

static void test_rtc_base(void)
{

    while (1) {
        tim_delay(1, TIM_UNIT_S);
        standard_output_rtc_data();
    }
}

static void test_rtc_alarm(void)
{
    standard_rtc_init();

    rtc_alarm_config alarm_conf = {0};

    alarm_conf.date_match    = RTC_ALARM_DATEDAY_DONT_CARE;
    alarm_conf.hours_match   = RTC_ALARM_HOURS_DONT_CARE;
    alarm_conf.minutes_match = RTC_ALARM_MINUTES_DONT_CARE;

    alarm_conf.alarm_sel         = RTC_ALARM_A_SEL;
    alarm_conf.seconds_match     = RTC_ALARM_SECONDS_MATCH;
    alarm_conf.time_data.seconds = RTC_TEST_TIME + 3;

    alarm_conf.weekday_sel       = RTC_ALARM_WEEKDAY_SEL_DATE;
    alarm_conf.date_data.weekday = RTC_WEEKDAY_MONDAY;

    alarm_conf.it_toggle = ENABLE;

    rtc_alarm_set(alarm_conf);
    while (1) {
        tim_delay(1, TIM_UNIT_S);
        standard_output_rtc_data();
    }
}

static void test_rtc_timestamp(void)
{
    standard_rtc_init();

    gpio_handle timestamp_pin               = {0};
    timestamp_pin.p_gpiox                   = GPIOC;
    timestamp_pin.gpio_conf.pin_no          = PIN_NO_13;
    timestamp_pin.gpio_conf.mode            = GPIO_MODE_INPUT;
    timestamp_pin.gpio_conf.output_type     = GPIO_OPTYPE_OPEN_DRAIN;
    timestamp_pin.gpio_conf.pullup_pulldown = GPIO_PULL_UP;
    gpio_init(&timestamp_pin);

    rtc_timestamp_config timestamp_conf = {0};
    timestamp_conf.it_opt               = RTC_IT_OPT_TIMESTAMP;
    timestamp_conf.alt_fn_sel           = RTC_AF1_SEL;
    timestamp_conf.timestamp_edge       = RTC_TIMESTAMP_EDGE_FALLING;

    rtc_timestamp_init(timestamp_conf);

    while (1) {
        tim_delay(1, TIM_UNIT_S);
        standard_output_rtc_data();
        if (rtc_timestamp_read_status() == RTC_TIMESTAMP_STATUS_TRIGGERED) {
            rtc_timestamp_clear_status();
            for (uint32_t i = 0; i < 5000; i++);
        }
    }
}

static void test_rtc_wakeup(void)
{
    standard_rtc_init();

    rtc_wakeup_config wakeup_conf = {0};
    wakeup_conf.clk_sel           = RTC_WAKEUP_SPRE_CLK;
    wakeup_conf.auto_reload_value = 0;
    wakeup_conf.it_opt            = RTC_IT_OPT_WAKEUP;
    toggle_debug_pin();
    rtc_wakeup_init(wakeup_conf);

    while (1) {
        // standard_output_rtc_data();
    }
}

// Interrupts
void TAMP_STAMP_IRQHandler(void)
{
    rtc_time_data ts_time_data = {0};
    ts_time_data               = rtc_timestamp_read_time();
    printf_("Timestamp triggered at %d:%d:%d", ts_time_data.hours, ts_time_data.minutes, ts_time_data.seconds);
    rtc_it_handler();
    toggle_debug_pin();
    toggle_debug_pin();
}

void RTC_WKUP_IRQHandler(void)
{
    toggle_debug_pin();

    standard_output_rtc_data();

    rtc_it_handler();
}

void RTC_Alarm_IRQHandler(void)
{
    if (rtc_alarm_get_status(RTC_ALARM_A_SEL) == RTC_ALARM_STATUS_TRIGGERED) {
        printf_("Alarm triggered");
    }
    rtc_it_handler();
}
