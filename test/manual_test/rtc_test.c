//======================================================================================//{
//  File: rtc_test.c
//  Overview: Tests some of the RTC functionality.
//  Tests:
//      - Base time keeping
//      - Alarms
//      - Timestamps
//      - Wakeup
//  Notes: None
//======================================================================================//}

//} Includes
//=========================================//{

#include "rtc_test.h"
#include "debug_tools.h"
#include "gpio.h"
#include "printf.h"
#include "rcc.h"
#include "rtc.h"
#include "stm32f4xx.h"
#include "tim.h"

//======================================================================================//}
//                  Private Constants
//======================================================================================//{

typedef enum : uint8_t {
    RTC_TEST_TIME = 30,
} rtc_test_e;

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

rtc_handler g_rtc_handle = {0};

//======================================================================================//}
//                  Private Function Prototypes
//======================================================================================//{

static void rtc_setup(void);
static void output_rtc_data(void);

//======================================================================================//}
//                  Public Test Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: test_rtc_base
//  Overview: Test RTC base counting with logic analyzer.
//  Test goals:
//      - RTC initialization
//      - RTC counting
//      - Time switches from PM to AM after 12 PM.
//  Setup:
//      - PA2 -> logic analyzer: read printf
//      - PA3 -> logic analyzer: read debug pin
//  Expectations: Use logic analyzer to view printf. Every second the RTC data
//      should be output.
//  Note: Time between each output is not important. What is important is that the
//      time is incrementing properly.
//============================================================================//}
void test_rtc_base(void)
{
    rtc_setup();
    while (1)
    {
        tim_delay(1, TIM_UNIT_S);
        output_rtc_data();
    }
}

//============================================================================//{
//  Function: test_rtc_alarm
//  Overview: Test RTC alarm with logic analyzer.
//  Test goals:
//      - RTC alarm trigger and data
//      - RTC interrupts with alarm
//  Setup:
//      - PA2 -> logic analyzer: read printf
//      - PA3 -> logic analyzer: read debug pin
//  Expectations: Use logic analyzer to view printf. Output time information every
//      second. Shortly after the 3rd or 4th time info output, there should be a
//      statement saying "Alarm triggered".
//  Note: Time between each output is not important. What is important is that the
//      time is incrementing properly.
//============================================================================//}
void test_rtc_alarm(void)
{
    rtc_setup();

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
    while (1)
    {
        tim_delay(1, TIM_UNIT_S);
        output_rtc_data();
    }
}

//============================================================================//{
//  Function: test_rtc_timestamp
//  Overview: Test RTC timestamp interrupt with logic analyzer and button.
//  Test goals:
//      - RTC timestamp triggering and data
//      - RTC interrupts with timestamps
//  Setup:
//      - PA2 -> logic analyzer: read printf
//      - PA3 -> logic analyzer: read debug pin
//      - Connect PC13 to push button switch. Connect other end of button to ground.
//  Expectations: Use logic analyzer to view printf and optionally to PC13/button.
//      When button is not pressed, PC13 line should be high. When button is pressed,
//      PC13 line should be pulled low and the timestamp data should be recorded and output.
//  Note: Time between each output is not important. What is important is that the
//      time is incrementing properly.
//============================================================================//}
void test_rtc_timestamp(void)
{
    rtc_setup();

    gpio_handle timestamp_pin               = {0};
    timestamp_pin.p_gpiox                   = GPIOC;
    timestamp_pin.gpio_conf.pin_num         = PIN_NUM_13;
    timestamp_pin.gpio_conf.mode            = GPIO_MODE_INPUT;
    timestamp_pin.gpio_conf.output_type     = GPIO_OPTYPE_OPEN_DRAIN;
    timestamp_pin.gpio_conf.pullup_pulldown = GPIO_PULL_UP;
    gpio_init(&timestamp_pin);

    rtc_timestamp_config timestamp_conf = {0};
    timestamp_conf.it_opt               = RTC_IT_OPT_TIMESTAMP;
    timestamp_conf.alt_fn_sel           = RTC_AF1_SEL;
    timestamp_conf.timestamp_edge       = RTC_TIMESTAMP_EDGE_FALLING;

    rtc_timestamp_init(timestamp_conf);

    while (1)
    {
        tim_delay(1, TIM_UNIT_S);
        output_rtc_data();
        if (RTC_TIMESTAMP_STATUS_TRIGGERED == rtc_timestamp_read_status())
        {
            rtc_timestamp_clear_status();
            for (uint32_t tick = 0; tick < 5000; tick++);
        }
    }
}

//============================================================================//{
//  Function: test_rtc_wakeup
//  Overview: Test RTC wakeup interrupt with logic analyzer.
//  Test goals:
//      - RTC wakeup triggering
//      - Wakeup triggers very closely to every second
//      - RTC time output is accurate
//      - RTC interrupts with wakeup
//  Setup:
//      - PA2 -> logic analyzer: read printf.
//      - PA3 -> logic analyzer: read debug pin.
//  Expectations: Use logic analyzer to view printf.
//  Note: Unlike other RTC tests here time accuracy between triggers is very important.
//============================================================================//}
void test_rtc_wakeup(void)
{
    rtc_setup();

    rtc_wakeup_config wakeup_conf = {0};
    wakeup_conf.clk_sel           = RTC_WAKEUP_SPRE_CLK;
    wakeup_conf.auto_reload_value = 0;
    wakeup_conf.it_opt            = RTC_IT_OPT_WAKEUP;
    toggle_debug_pin();
    rtc_wakeup_init(wakeup_conf);

    while (1)
    {
    }
}

//======================================================================================//}
//                  Private Function Implementation
//======================================================================================//{

static void rtc_setup(void)
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

static void output_rtc_data(void)
{
    uint8_t am_pm[]        = "AM";
    g_rtc_handle.time_data = rtc_get_time_data();
    g_rtc_handle.date_data = rtc_get_date_data();

    if (RTC_TIME_AM == g_rtc_handle.time_data.time_am_pm)
    {
        am_pm[0] = 'A';
    }
    else if (RTC_TIME_PM == g_rtc_handle.time_data.time_am_pm)
    {
        am_pm[0] = 'P';
    }
    printf_("Time: %d:%d:%d %s\n", g_rtc_handle.time_data.hours, g_rtc_handle.time_data.minutes,
            g_rtc_handle.time_data.seconds, am_pm);
    printf_("Date: %d/%d/%d\n", g_rtc_handle.date_data.month, g_rtc_handle.date_data.date, g_rtc_handle.date_data.year);
}

//======================================================================================//}
//                  ISR Handlers
//======================================================================================//{

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

    output_rtc_data();

    rtc_it_handler();
}

void RTC_Alarm_IRQHandler(void)
{
    if (RTC_ALARM_STATUS_TRIGGERED == rtc_alarm_get_status(RTC_ALARM_A_SEL))
    {
        printf_("Alarm triggered");
    }
    rtc_it_handler();
}

// *End of file*
