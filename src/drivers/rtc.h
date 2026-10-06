/*****************************************************************************************
File: rtc.h
Overview: Provides functions for initializing, configuring, and using the RTC peripheral.
Features:
    General time keeping
    Alarms (w/ interrupt capability)
    Timestamps (w/ interrupt capability)
    Wake-up (w/ interrupt capability)
Notes: None
******************************************************************************************/

#ifndef RTC_H
#define RTC_H

//} Include Statements
//=========================================//{

#include "common.h"
#include "exti.h"
#include "rcc.h"
#include "stm32f4xx.h"

//======================================================================================//}
//                  Public Constants
//======================================================================================//{

//} Address Definitions
//=========================================//{

typedef enum {
    RTC_BASE_ADDR = ((APB1_BASE_ADDR) + (0x2800U))
} rtc_base_addr_e;

//} Config Options
//=========================================//{

// Applications:
// - RTC CR FMT
typedef enum {
    RTC_TIME_FORMAT_24HR = 0b0,
    RTC_TIME_FORMAT_12HR = 0b1,
} rtc_time_format_e;

// Applications:
// - RTC TR PM
typedef enum {
    RTC_TIME_AM = 0b0,
    RTC_TIME_PM = 0b1,
} rtc_time_am_pm_e;

// Applications:
// - RTC DR WDU
typedef enum {
    RTC_WEEKDAY_MONDAY    = 0b001,
    RTC_WEEKDAY_TUESDAY   = 0b010,
    RTC_WEEKDAY_WEDNESDAY = 0b011,
    RTC_WEEKDAY_THURSDAY  = 0b100,
    RTC_WEEKDAY_FRIDAY    = 0b101,
    RTC_WEEKDAY_SATURDAY  = 0b110,
    RTC_WEEKDAY_SUNDAY    = 0b111,
} rtc_weekdays_e;

//} Alarm Config Options
//=========================================//{

typedef enum {
    RTC_ALARM_A_SEL,
    RTC_ALARM_B_SEL,
} rtc_alarm_sel_e;

// Applications:
// - RTC ALRMxR MSK1
typedef enum {
    RTC_ALARM_SECONDS_MATCH     = 0b0,
    RTC_ALARM_SECONDS_DONT_CARE = 0b1,
} rtc_alarm_seconds_match_e;

// Applications:
// - RTC ALRMxR MSK2
typedef enum {
    RTC_ALARM_MINUTES_MATCH     = 0b0,
    RTC_ALARM_MINUTES_DONT_CARE = 0b1,
} rtc_alarm_minutes_match_e;

// Applications:
// - RTC ALRMxR MSK3
typedef enum {
    RTC_ALARM_HOURS_MATCH     = 0b0,
    RTC_ALARM_HOURS_DONT_CARE = 0b1,
} rtc_alarm_hours_match_e;

// Applications:
// - RTC ALRMxR MSK4
typedef enum {
    RTC_ALARM_DATEDAY_MATCH     = 0b0,
    RTC_ALARM_DATEDAY_DONT_CARE = 0b1,
} rtc_alarm_dateday_match_e;

// Applications:
// - RTC ALRMxR WDSEL
typedef enum {
    RTC_ALARM_WEEKDAY_SEL_DATE    = 0b0,
    RTC_ALARM_WEEKDAY_SEL_WEEKDAY = 0b1,
} rtc_alarm_daydate_sel_e;

//} API Function Argument/Return Options
//=========================================//{

typedef enum {
    RTC_IT_OPT_NA,
    RTC_IT_OPT_ALARM,
    RTC_IT_OPT_WAKEUP,
    RTC_IT_OPT_TIMESTAMP,
    RTC_IT_OPT_TAMPER,
} rtc_it_options_e;

typedef enum {
    RTC_AF1_SEL = 0b0,
    RTC_AF2_SEL = 0b1,
} rtc_alt_fn_sel_e;

typedef enum {
    RTC_TIMESTAMP_EDGE_RISING  = 0b0,
    RTC_TIMESTAMP_EDGE_FALLING = 0b1,
} rtc_timestamp_edge_e;

typedef enum {
    RTC_TIMESTAMP_STATUS_NOT_TRIGGERED = 0b0,
    RTC_TIMESTAMP_STATUS_TRIGGERED     = 0b1,
} rtc_timestamp_status_e;

typedef enum {
    RTC_WAKEUP_RTC_CLK_DIV16      = 0b000,
    RTC_WAKEUP_RTC_CLK_DIV8       = 0b001,
    RTC_WAKEUP_RTC_CLK_DIV4       = 0b010,
    RTC_WAKEUP_RTC_CLK_DIV2       = 0b011,
    RTC_WAKEUP_SPRE_CLK           = 0b100, // Usually 1 hz
    RTC_WAKEUP_SPRE_CLK_EXTRA_BIT = 0b110, // Usually 1 hz + 2**16 is added to WUT counter value
} rtc_wakeup_clk_sel_e;

typedef enum {
    RTC_WAKEUP_STATUS_NOT_TRIGGERED = 0b0,
    RTC_WAKEUP_STATUS_TRIGGERED     = 0b1,
} rtc_wakeup_status_e;

//} Misc. Constants
//=========================================//{

// TODO: Add applications section for enums.
// Register names or function names

typedef enum {
    RTC_EXTI_NUM_ALARM     = EXTI_LINE_NUM_17,
    RTC_EXTI_NUM_WAKE_UP   = EXTI_LINE_NUM_22,
    RTC_EXTI_NUM_TIMESTAMP = EXTI_LINE_NUM_21,
} rtc_exti_line_no_e;

// Applications:
// - RTC ISR ALARxF
typedef enum {
    RTC_ALARM_STATUS_NOT_TRIGGERED = 0b0,
    RTC_ALARM_STATUS_TRIGGERED     = 0b1,
} rtc_alarm_status_e;

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

typedef volatile struct {
    uint32_t TR;         // time register                                             Offset: 0x0
    uint32_t DR;         // date register                                             Offset: 0x4
    uint32_t CR;         // control register                                          Offset: 0x8
    uint32_t ISR;        // initialization and status register                        Offset: 0xC
    uint32_t PRER;       // prescaler register                                        Offset: 0x10
    uint32_t WUTR;       // wakeup timer register                                     Offset: 0x14
    uint32_t CALIBR;     // calibration register                                      Offset: 0x18
    uint32_t ALRMAR;     // alarm A register                                          Offset: 0x1C
    uint32_t ALRMBR;     // alarm B register                                          Offset: 0x20
    uint32_t WPR;        // write protection register                                 Offset: 0x24
    uint32_t SSR;        // sub second register                                       Offset: 0x28
    uint32_t SHIFTR;     // shift control register                                    Offset: 0x2C
    uint32_t TSTR;       // time stamp time register                                  Offset: 0x30
    uint32_t TSDR;       // time stamp date register                                  Offset: 0x34
    uint32_t TSSSR;      // timestamp sub second register                             Offset: 0x38
    uint32_t CALR;       // calibration register                                      Offset: 0x3C
    uint32_t TAFCR;      // tamper and alternate function configuration register      Offset: 0x40
    uint32_t ALRMASSR;   // alarm A sub second register                               Offset: 0x44
    uint32_t ALRMBSSR;   // alarm B sub second register                               Offset: 0x48
    uint32_t reserved_1; // Reserved 0x4C
    uint32_t BKPR[20];   // backup register                                           Offset: 0x50
} rtc_reg_def;

// Time data definition (used to configure time)
typedef struct {
    uint8_t seconds;               // Default: 0
    uint8_t minutes;               // Default: 0
    uint8_t hours;                 // Default: 0
    rtc_time_format_e time_format; // Default: RTC_TIME_FORMAT_24HR
    rtc_time_am_pm_e time_am_pm;   // Default: RTC_TIME_AM
} rtc_time_data;

// Date data definition (used to configure date)
typedef struct {
    uint8_t month;          // Default: 0
    uint8_t date;           // Default: 0
    uint8_t year;           // Default: 0
    rtc_weekdays_e weekday; // Default: ERROR
} rtc_date_data;

// RTC clock configuration definition (used to configure the input clock to the RTC)
typedef struct {
    rcc_rtc_clk_src_e rtc_sel; // Default: RCC_RTC_CLK_SRC_NA
    rcc_rtc_hse_pre_e rtc_pre; // Default: RCC_RTC_HSE_PRE_NA
} rtc_config;

// RTC handle definition (used to initialize the base RTC)
typedef struct {
    rtc_time_data time_data;
    rtc_date_data date_data;
    rtc_config rtc_conf;
} rtc_handler;

// Alarm configuration definition
typedef struct {
    rtc_time_data time_data;
    rtc_date_data date_data;
    rtc_alarm_sel_e alarm_sel;               // Default: RTC_ALARM_A_SEL
    rtc_alarm_seconds_match_e seconds_match; // Default: RTC_ALARM_SECONDS_MATCH
    rtc_alarm_minutes_match_e minutes_match; // Default: RTC_ALARM_MINUTES_MATCH
    rtc_alarm_hours_match_e hours_match;     // Default: RTC_ALARM_HOURS_MATCH
    rtc_alarm_dateday_match_e date_match;    // Default: RTC_ALARM_DATE_MATCH
    rtc_alarm_daydate_sel_e weekday_sel;     // Default: RTC_ALARM_WEEKDAY_SEL_DATE
    togglable_e it_toggle;                   // Default: DISABLE
} rtc_alarm_config;

// Timestamp configuration definition
typedef struct {
    rtc_it_options_e it_opt;
    rtc_alt_fn_sel_e alt_fn_sel;
    rtc_timestamp_edge_e timestamp_edge;
} rtc_timestamp_config;

// Wakeup configuration defintion
typedef struct {
    rtc_wakeup_clk_sel_e clk_sel;
    rtc_it_options_e it_opt;
    uint16_t auto_reload_value;
} rtc_wakeup_config;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define RTC ((rtc_reg_def *) RTC_BASE_ADDR)

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

void rtc_init(rtc_handler *const p_rtc_handler);

rtc_time_am_pm_e rtc_get_time_format(void);
uint8_t rtc_get_hours(void);
uint8_t rtc_get_minutes(void);
uint8_t rtc_get_seconds(void);
rtc_time_data rtc_get_time_data(void);

rtc_weekdays_e rtc_get_weekday(void);
uint8_t rtc_get_year(void);
uint8_t rtc_get_month(void);
uint8_t rtc_get_date(void);
rtc_date_data rtc_get_date_data(void);

void rtc_alarm_set(rtc_alarm_config alarm_conf);
void rtc_alarm_set_seconds(rtc_alarm_sel_e alarm_sel, uint8_t seconds, rtc_alarm_seconds_match_e seconds_match);
void rtc_alarm_set_minutes(rtc_alarm_sel_e alarm_sel, uint8_t minutes, rtc_alarm_minutes_match_e minutes_match);
void rtc_alarm_set_hours(rtc_alarm_sel_e alarm_sel, uint8_t hours, rtc_alarm_hours_match_e hours_match);
void rtc_alarm_set_date(rtc_alarm_sel_e alarm_sel, uint8_t date, rtc_alarm_dateday_match_e date_match);
void rtc_alarm_set_day(rtc_alarm_sel_e alarm_sel, rtc_weekdays_e weekday, rtc_alarm_dateday_match_e date_match);
void rtc_alarm_set_am_pm(rtc_alarm_sel_e alarm_sel, uint8_t am_pm);
rtc_alarm_status_e rtc_alarm_get_status(rtc_alarm_sel_e alarm_sel);

void rtc_config_it(rtc_it_options_e it_opt, togglable_e toggle);

void rtc_timestamp_init(rtc_timestamp_config timestamp_conf);
rtc_timestamp_status_e rtc_timestamp_read_status(void);
void rtc_timestamp_clear_status(void);
rtc_date_data rtc_timestamp_read_date(void);
rtc_time_data rtc_timestamp_read_time(void);

void rtc_wakeup_init(rtc_wakeup_config wakeup_conf);
rtc_wakeup_status_e rtc_wakeup_get_status(void);
void rtc_wakeup_clear_status(void);

void rtc_it_handler(void);

#endif // RTC_H

// *End of file*
