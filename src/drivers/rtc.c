#include "rtc.h"
#include "assert_handler.h"
#include "common.h"
#include "exti.h"
#include "nvic.h"
#include "pwr.h"
#include "rcc.h"
#include "stm32f4xx.h"
#include <stdint.h>
#include <time.h>

//======================================================================================//}
//                  Helper Function Prototypes
//======================================================================================//{

static void congifure_rtc_clock(rtc_config rtc_conf);
static void congifure_rtc_clock_asserts(rtc_config rtc_conf);
static void set_rtc_time(rtc_time_data time_data);
static void time_data_asserts(rtc_time_data time_data);
static void set_rtc_date(rtc_date_data date_data);
static void date_data_asserts(rtc_date_data time_data);
static inline uint8_t convert_bcd_to_dec(uint8_t tens, uint8_t units);
static inline void unlock_rtc(void);
static void set_rtc_pre(void);
static void enable_alarm(rtc_alarm_sel_e alarm_sel, togglable_e it_toggle);
static void disable_alarm(rtc_alarm_sel_e alarm_sel);

static inline void assert_seconds(uint8_t seconds);
static inline void assert_minutes(uint8_t minutes);
static inline void assert_hours(uint8_t hours);
static inline void assert_am_pm(rtc_time_am_pm_e am_pm);
static inline void assert_time_format(rtc_time_format_e hours);

static inline void assert_date(uint8_t date);
static inline void assert_month(uint8_t month);
static inline void assert_year(uint8_t year);
static inline void assert_weekday(rtc_weekdays_e weekday);

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

//======================================================================================//}
//                  Peripheral Function API Implementation
//======================================================================================//{

/***************************************************************************
Function: rtc_init 
Overview: Initializes the RTC peripheral inside the handle with the settings in the configuration structure
Parameters:
    p_rtc_handle: Structure with the corresponding settings to configure the RTC peripheral
Return: 
    None
Note: None
***************************************************************************/
void rtc_init(rtc_handler *const p_rtc_handler)
{
    uint8_t const UPDATE_ALLOWED = (1 << 6);

    // NOTE: PWR has to be disable backup write protection in order to write to certain parts of RCC
    pwr_clock_enable();
    pwr_backup_write_protection_dis();

    // RCC Config
    // Enable clock to RTC
    rcc_rtc_clock_enable();

    congifure_rtc_clock(p_rtc_handler->rtc_conf);

    // NOTE: RTC is locked be default, must be unlocked first to change most registers
    unlock_rtc();

    RTC->ISR |= RTC_ISR_INIT;
    while ((RTC->ISR & RTC_ISR_INITF) != UPDATE_ALLOWED);

    set_rtc_pre();

    set_rtc_time(p_rtc_handler->time_data);
    set_rtc_date(p_rtc_handler->date_data);

    RTC->ISR &= ~RTC_ISR_INIT;
}

/***************************************************************************
Function: rtc_get_time_format
Overview: Gets the time format (AM/PM)
Parameters: 
    None
Return: 
    rtc_time_format_e: Time format of current time 
        RTC_TIME_FORMAT_AM_24HR
        RTC_TIME_FORMAT_PM_12HR
Note: None
***************************************************************************/
rtc_time_am_pm_e rtc_get_time_format(void)
{
    return (RTC->TR >> RTC_TR_PM_POS) & RTC_TR_PM_MASK;
}

/***************************************************************************
Function: rtc_get_hours
Overview: Gets the RTC hours
Parameters: 
    None
Return: 
    RTC hours
Note: None
***************************************************************************/
uint8_t rtc_get_hours(void)
{
    return convert_bcd_to_dec((RTC->TR >> RTC_TR_HT_POS) & RTC_TR_HT_MASK, (RTC->TR >> RTC_TR_HU_POS) & RTC_TR_HU_MASK);
}

/***************************************************************************
Function: rtc_get_minutes
Overview: Gets the RTC minutes
Parameters: 
    None
Return: 
    RTC minutes
Note: None
***************************************************************************/
uint8_t rtc_get_minutes(void)
{
    return convert_bcd_to_dec((RTC->TR >> RTC_TR_MNT_POS) & RTC_TR_MNT_MASK,
                              (RTC->TR >> RTC_TR_MNU_POS) & RTC_TR_MNU_MASK);
}

/***************************************************************************
Function: rtc_get_seconds
Overview: Gets the RTC seconds
Parameters: 
    None
Return: 
    RTC seconds
Note: None
***************************************************************************/
uint8_t rtc_get_seconds(void)
{
    return convert_bcd_to_dec((RTC->TR >> RTC_TR_ST_POS) & RTC_TR_ST_MASK, (RTC->TR >> RTC_TR_SU_POS) & RTC_TR_SU_MASK);
}

/***************************************************************************
Function: rtc_get_time_data
Overview: Gets the RTC time and returns a structure
Parameters: 
    None
Return: 
    Structure with RTC time data
Note: None
***************************************************************************/
rtc_time_data rtc_get_time_data(void)
{
    rtc_time_data time_data;
    uint32_t const TEMP_TR = RTC->TR;
    uint32_t const TEMP_CR = RTC->CR;
    time_data.time_format  = (TEMP_CR >> RTC_CR_FMT_POS) & RTC_CR_FMT_MASK;
    time_data.time_am_pm   = (TEMP_TR >> RTC_TR_PM_POS) & RTC_TR_PM_MASK;
    // clang-format off
    time_data.hours   = convert_bcd_to_dec((TEMP_TR >> RTC_TR_HT_POS) & RTC_TR_HT_MASK, (TEMP_TR >> RTC_TR_HU_POS) & RTC_TR_HU_MASK);
    time_data.minutes = convert_bcd_to_dec((TEMP_TR >> RTC_TR_MNT_POS) & RTC_TR_MNT_MASK, (TEMP_TR >> RTC_TR_MNU_POS) & RTC_TR_MNU_MASK);
    time_data.seconds = convert_bcd_to_dec((TEMP_TR >> RTC_TR_ST_POS) & RTC_TR_ST_MASK, (TEMP_TR >> RTC_TR_SU_POS) & RTC_TR_SU_MASK);
    // clang-format on

    return time_data;
}

/***************************************************************************
Function: rtc_get_weekday
Overview: Gets the RTC weekday
Parameters: 
    None
Return: 
    RTC weekday
Note: None
***************************************************************************/
rtc_weekdays_e rtc_get_weekday(void)
{
    return (RTC->DR >> RTC_DR_WDU_POS) & RTC_DR_WDU_MASK;
}

/***************************************************************************
Function: rtc_get_year
Overview: Gets the RTC year
Parameters: 
    None
Return: 
    RTC year
Note: None
***************************************************************************/
uint8_t rtc_get_year(void)
{
    return convert_bcd_to_dec((RTC->DR >> RTC_DR_YT_POS) & RTC_DR_YT_MASK, (RTC->DR >> RTC_DR_YU_POS) & RTC_DR_YU_MASK);
}

/***************************************************************************
Function: rtc_get_month
Overview: Gets the RTC month
Parameters: 
    None
Return: 
    RTC month
Note: None
***************************************************************************/
uint8_t rtc_get_month(void)
{
    return convert_bcd_to_dec((RTC->DR >> RTC_DR_MT_POS) & RTC_DR_MT_MASK, (RTC->DR >> RTC_DR_MU_POS) & RTC_DR_MU_MASK);
}

/***************************************************************************
Function: rtc_get_date
Overview: Gets the RTC date
Parameters: 
    None
Return: 
    RTC date
Note: None
***************************************************************************/
uint8_t rtc_get_date(void)
{
    return convert_bcd_to_dec((RTC->DR >> RTC_DR_DT_POS) & RTC_DR_DT_MASK, (RTC->DR >> RTC_DR_DU_POS) & RTC_DR_DU_MASK);
}

/***************************************************************************
Function: rtc_get_date_data
Overview: Gets the RTC date and returns a structure
Parameters: 
    None
Return: 
    Structure with RTC date data
Note: None
***************************************************************************/
rtc_date_data rtc_get_date_data(void)
{
    rtc_date_data date_data;
    uint32_t const TEMP_DR = RTC->DR;
    date_data.weekday      = (TEMP_DR >> RTC_DR_WDU_POS) & RTC_DR_WDU_MASK;
    // clang-format off
    date_data.year  = convert_bcd_to_dec((TEMP_DR >> RTC_DR_YT_POS) & RTC_DR_YT_MASK, (TEMP_DR >> RTC_DR_YU_POS) & RTC_DR_YU_MASK);
    date_data.month = convert_bcd_to_dec((TEMP_DR >> RTC_DR_MT_POS) & RTC_DR_MT_MASK, (TEMP_DR >> RTC_DR_MU_POS) & RTC_DR_MU_MASK);
    date_data.date  = convert_bcd_to_dec((TEMP_DR >> RTC_DR_DT_POS) & RTC_DR_DT_MASK, (TEMP_DR >> RTC_DR_DU_POS) & RTC_DR_DU_MASK);
    // clang-format on

    return date_data;
}

/***************************************************************************
Function: rtc_alarm_set
Overview: Sets the alarm with the given settings in the configuration structure
Parameters: 
    alarm_conf: Contains settings for configuring the alarm
Return: 
    None
Note: None
***************************************************************************/
void rtc_alarm_set(rtc_alarm_config const alarm_conf)
{
    // Initialize to 0 and don't bother resetting each field as all bits will be set
    rtc_alarm_sel_e const alarm_sel = alarm_conf.alarm_sel;

    time_data_asserts(alarm_conf.time_data);
    date_data_asserts(alarm_conf.date_data);

    disable_alarm(alarm_sel);

    switch (alarm_conf.weekday_sel) {
    case RTC_ALARM_WEEKDAY_SEL_DATE:
        rtc_alarm_set_date(alarm_sel, alarm_conf.date_data.date, alarm_conf.date_match);
        break;
    case RTC_ALARM_WEEKDAY_SEL_WEEKDAY:
        rtc_alarm_set_day(alarm_sel, alarm_conf.date_data.weekday, alarm_conf.date_match);
        break;
    }

    // Seconds config
    rtc_alarm_set_seconds(alarm_sel, alarm_conf.time_data.seconds, alarm_conf.seconds_match);

    // Minutes config
    rtc_alarm_set_minutes(alarm_sel, alarm_conf.time_data.minutes, alarm_conf.minutes_match);

    // Hours config
    rtc_alarm_set_hours(alarm_sel, alarm_conf.time_data.hours, alarm_conf.hours_match);

    // AM/PM config
    rtc_alarm_set_am_pm(alarm_sel, alarm_conf.time_data.time_am_pm);

    enable_alarm(alarm_sel, alarm_conf.it_toggle);

    rtc_it_config(RTC_IT_OPT_ALARM, alarm_conf.it_toggle);
}

void rtc_alarm_set_seconds(rtc_alarm_sel_e const alarm_sel, uint8_t const seconds,
                           rtc_alarm_seconds_match_e const seconds_match)
{
    assert_seconds(seconds);

    // NOTE: If the seconds field is set, the synchonous prescaler in RTC_PRER might need to be at least 3. See ref manual pg 805

    uint32_t temp_alrmxr = 0;

    temp_alrmxr |= seconds_match << RTC_ALRMAR_MSK1_POS;
    temp_alrmxr |= (seconds / 10) << RTC_ALRMAR_ST_POS;
    temp_alrmxr |= (seconds % 10) << RTC_ALRMAR_SU_POS;

    switch (alarm_sel) {
    case RTC_ALARM_A_SEL:
        RTC->ALRMAR &= ~((RTC_ALRMAR_MSK1_MASK << RTC_ALRMAR_MSK1_POS) | (RTC_ALRMAR_ST_MASK << RTC_ALRMAR_ST_POS)
                         | (RTC_ALRMAR_SU_MASK << RTC_ALRMAR_SU_POS));
        RTC->ALRMAR |= temp_alrmxr;
        break;

    case RTC_ALARM_B_SEL:
        RTC->ALRMBR &= ~((RTC_ALRMBR_MSK1_MASK << RTC_ALRMBR_MSK1_POS) | (RTC_ALRMBR_ST_MASK << RTC_ALRMBR_ST_POS)
                         | (RTC_ALRMBR_SU_MASK << RTC_ALRMBR_SU_POS));
        RTC->ALRMBR |= temp_alrmxr;
        break;
    }
}

void rtc_alarm_set_minutes(rtc_alarm_sel_e const alarm_sel, uint8_t const minutes,
                           rtc_alarm_minutes_match_e const minutes_match)
{
    assert_minutes(minutes);

    uint32_t temp_alrmxr = 0;

    temp_alrmxr |= minutes_match << RTC_ALRMAR_MSK2_POS;
    temp_alrmxr |= (minutes / 10) << RTC_ALRMAR_MNT_POS;
    temp_alrmxr |= (minutes % 10) << RTC_ALRMAR_MNU_POS;

    switch (alarm_sel) {
    case RTC_ALARM_A_SEL:
        RTC->ALRMAR &= ~((RTC_ALRMAR_MSK2_MASK << RTC_ALRMAR_MSK2_POS) | (RTC_ALRMAR_MNT_MASK << RTC_ALRMAR_MNT_POS)
                         | (RTC_ALRMAR_MNU_MASK << RTC_ALRMAR_MNU_POS));
        RTC->ALRMAR |= temp_alrmxr;
        break;

    case RTC_ALARM_B_SEL:
        RTC->ALRMBR &= ~((RTC_ALRMBR_MSK2_MASK << RTC_ALRMBR_MSK2_POS) | (RTC_ALRMBR_MNT_MASK << RTC_ALRMBR_MNT_POS)
                         | (RTC_ALRMBR_MNU_MASK << RTC_ALRMBR_MNU_POS));
        RTC->ALRMBR |= temp_alrmxr;
        break;
    }
}

void rtc_alarm_set_hours(rtc_alarm_sel_e const alarm_sel, uint8_t const hours,
                         rtc_alarm_hours_match_e const hours_match)
{
    assert_hours(hours);

    uint32_t temp_alrmxr = 0;

    temp_alrmxr |= hours_match << RTC_ALRMAR_MSK3_POS;
    temp_alrmxr |= (hours / 10) << RTC_ALRMAR_HT_POS;
    temp_alrmxr |= (hours % 10) << RTC_ALRMAR_HU_POS;

    switch (alarm_sel) {
    case RTC_ALARM_A_SEL:
        RTC->ALRMAR &= ~((RTC_ALRMAR_MSK3_MASK << RTC_ALRMAR_MSK3_POS) | (RTC_ALRMAR_HT_MASK << RTC_ALRMAR_HT_POS)
                         | (RTC_ALRMAR_HU_MASK << RTC_ALRMAR_HU_POS));
        RTC->ALRMAR |= temp_alrmxr;
        break;

    case RTC_ALARM_B_SEL:
        RTC->ALRMBR &= ~((RTC_ALRMBR_MSK3_MASK << RTC_ALRMBR_MSK3_POS) | (RTC_ALRMBR_HT_MASK << RTC_ALRMBR_HT_POS)
                         | (RTC_ALRMBR_HU_MASK << RTC_ALRMBR_HU_POS));
        RTC->ALRMBR |= temp_alrmxr;
        break;
    }
}

void rtc_alarm_set_date(rtc_alarm_sel_e const alarm_sel, uint8_t const date, rtc_alarm_dateday_match_e const date_match)
{
    assert_date(date);

    uint32_t temp_alrmxr = 0;
    // Date/day alarm
    temp_alrmxr |= date_match << RTC_ALRMAR_MSK4_POS;
    temp_alrmxr |= RTC_ALARM_WEEKDAY_SEL_DATE << RTC_ALRMAR_WDSEL_POS;
    temp_alrmxr |= (date / 10) << RTC_ALRMAR_DT_POS;
    temp_alrmxr |= (date % 10) << RTC_ALRMAR_DU_POS;

    // Clear RTC date
    switch (alarm_sel) {
    case RTC_ALARM_A_SEL:
        RTC->ALRMAR &= ~((RTC_ALRMAR_MSK4_MASK << RTC_ALRMAR_MSK4_POS) | (RTC_ALRMAR_WDSEL_MASK << RTC_ALRMAR_WDSEL_POS)
                         | (RTC_ALRMAR_DT_MASK << RTC_ALRMAR_DT_POS) | (RTC_ALRMAR_DU_MASK << RTC_ALRMAR_DU_POS));
        RTC->ALRMAR |= temp_alrmxr;
        break;

    case RTC_ALARM_B_SEL:
        RTC->ALRMBR &= ~((RTC_ALRMAR_MSK4_MASK << RTC_ALRMAR_MSK4_POS) | (RTC_ALRMAR_WDSEL_MASK << RTC_ALRMAR_WDSEL_POS)
                         | (RTC_ALRMAR_DT_MASK << RTC_ALRMAR_DT_POS) | (RTC_ALRMAR_DU_MASK << RTC_ALRMAR_DU_POS));
        RTC->ALRMBR |= temp_alrmxr;
        break;
    }
}
void rtc_alarm_set_day(rtc_alarm_sel_e const alarm_sel, rtc_weekdays_e const weekday,
                       rtc_alarm_dateday_match_e const date_match)
{
    assert_weekday(weekday);

    uint32_t temp_alrmxr = 0;
    temp_alrmxr |= date_match << RTC_ALRMAR_MSK4_POS;
    temp_alrmxr |= RTC_ALARM_WEEKDAY_SEL_WEEKDAY << RTC_ALRMAR_WDSEL_POS;
    temp_alrmxr |= weekday << RTC_ALRMAR_DU_POS;

    // Clear RTC date
    switch (alarm_sel) {
    case RTC_ALARM_A_SEL:
        RTC->ALRMAR &= ~((RTC_ALRMAR_MSK4_MASK << RTC_ALRMAR_MSK4_POS) | (RTC_ALRMAR_WDSEL_MASK << RTC_ALRMAR_WDSEL_POS)
                         | (RTC_ALRMAR_DT_MASK << RTC_ALRMAR_DT_POS) | (RTC_ALRMAR_DU_MASK << RTC_ALRMAR_DU_POS));
        RTC->ALRMAR |= temp_alrmxr;
        break;

    case RTC_ALARM_B_SEL:
        RTC->ALRMBR &= ~((RTC_ALRMAR_MSK4_MASK << RTC_ALRMAR_MSK4_POS) | (RTC_ALRMAR_WDSEL_MASK << RTC_ALRMAR_WDSEL_POS)
                         | (RTC_ALRMAR_DT_MASK << RTC_ALRMAR_DT_POS) | (RTC_ALRMAR_DU_MASK << RTC_ALRMAR_DU_POS));
        RTC->ALRMBR |= temp_alrmxr;
        break;
    }
}

void rtc_alarm_set_am_pm(rtc_alarm_sel_e const alarm_sel, uint8_t const am_pm)
{
    assert_am_pm(am_pm);

    switch (alarm_sel) {
    case RTC_ALARM_A_SEL:
        RTC->ALRMAR &= ~(RTC_ALRMAR_PM_MASK << RTC_ALRMAR_PM_POS);
        RTC->ALRMAR |= am_pm << RTC_ALRMAR_PM_POS;
        break;

    case RTC_ALARM_B_SEL:
        RTC->ALRMBR &= ~(RTC_ALRMAR_PM_MASK << RTC_ALRMAR_PM_POS);
        RTC->ALRMBR |= am_pm << RTC_ALRMAR_PM_POS;
        break;
    }
}

/***************************************************************************
Function: rtc_alarm_x_get_status
Overview: Gets alarm "x" status
Parameters: 
    None
Return: 
    rtc_alarm_status_e:
        RTC_ALARM_STATUS_TRIGGERED 
        RTC_ALARM_STATUS_MATCH     
Note: None
***************************************************************************/
rtc_alarm_status_e rtc_alarm_get_status(rtc_alarm_sel_e const alarm_sel)
{
    rtc_alarm_status_e alarm_status = RTC_ALARM_STATUS_NOT_TRIGGERED;
    switch (alarm_sel) {
    case RTC_ALARM_A_SEL: alarm_status = 0b1 & ((RTC->ISR >> RTC_ISR_ALRAF_POS) & RTC_ISR_ALRAF_MASK); break;
    case RTC_ALARM_B_SEL: alarm_status = 0b1 & ((RTC->ISR >> RTC_ISR_ALRBF_POS) & RTC_ISR_ALRBF_MASK); break;
    }
    return alarm_status;
}

/***************************************************************************
Function: rtc_it_config
Overview: Configures the given RTC related interrupt
Parameters: 
    it_opt: RTC interrupt options
        RTC_IT_OPT_ALARM
        RTC_IT_OPT_WAKE_UP
        RTC_IT_OPT_TIMESTAMP
        RTC_IT_OPT_TAMPER
    toggle:
        ENABLE  [1]
        DISABLE [0]
Return: 
    None
Note: None
***************************************************************************/
void rtc_it_config(rtc_it_options_e it_opt, togglable_e toggle)
{
    switch (it_opt) {
    case RTC_IT_OPT_ALARM:
        exti_it_config((exti_lines_e)RTC_EXTI_NO_ALARM, toggle);
        exti_rising_edge_config((exti_lines_e)RTC_EXTI_NO_ALARM, toggle);
        nvic_irq_config(RTC_ALARM_IRQ_NO_41, toggle);
        break;

    case RTC_IT_OPT_WAKEUP:
        exti_it_config((exti_lines_e)RTC_EXTI_NO_WAKE_UP, toggle);
        exti_rising_edge_config((exti_lines_e)RTC_EXTI_NO_WAKE_UP, toggle);
        nvic_irq_config(RTC_WKUP_IRQ_NO_3, toggle);
        break;

    case RTC_IT_OPT_TIMESTAMP:
        exti_it_config((exti_lines_e)RTC_EXTI_NO_TIMESTAMP, toggle);
        exti_rising_edge_config((exti_lines_e)RTC_EXTI_NO_TIMESTAMP, toggle);
        nvic_irq_config(TAMP_STAMP_IRQ_NO_2, toggle);
        break;

    case RTC_IT_OPT_NA:     break;
    case RTC_IT_OPT_TAMPER: ASSERT(false); break;
    }
}

/***************************************************************************
Function: rtc_timestamp_init
Overview: Initializes the RTC timestamp with the given settings in the timestamp_conf structure
Parameters: 
    timestamp_conf: Settings for timestamps
Return: 
    None
Note: None
***************************************************************************/
void rtc_timestamp_init(rtc_timestamp_config timestamp_conf)
{
    RTC->CR |= RTC_CR_TSE;

    if (timestamp_conf.it_opt == RTC_IT_OPT_TIMESTAMP) {
        RTC->CR |= RTC_CR_TSIE;
        rtc_it_config(timestamp_conf.it_opt, ENABLE);
    }

    // Map TIMESTAMP to AF
    RTC->TAFCR &= ~(RTC_TAFCR_TSINSEL_MASK << RTC_TAFCR_TSINSEL_POS);
    RTC->TAFCR |= timestamp_conf.alt_fn_sel << RTC_TAFCR_TSINSEL_POS;

    RTC->CR &= ~(RTC_CR_TSEDGE_MASK << RTC_CR_TSEDGE_POS);
    RTC->CR |= timestamp_conf.timestamp_edge << RTC_CR_TSEDGE_POS;
}

/***************************************************************************
Function: rtc_timestamp_read_status
Overview: Read the RTC timestamp status. Whether a timestamp has occured
Parameters: 
    None
Return: 
    rtc_timestamp_status_e: 
        RTC_TIMESTAMP_STATUS_NOT_TRIGGERED 
        RTC_TIMESTAMP_STATUS_TRIGGERED     
Note: None
***************************************************************************/
rtc_timestamp_status_e rtc_timestamp_read_status(void)
{
    return RTC_ISR_TSF_MASK & (RTC->ISR >> RTC_ISR_TSF_POS);
}

/***************************************************************************
Function: rtc_timestamp_clear_status
Overview: Clear the RTC timestamp status
Parameters: 
    None
Return: 
    None
Note: None
***************************************************************************/
void rtc_timestamp_clear_status(void)
{
    RTC->ISR &= ~(RTC_ISR_TSF);
    if (RTC->ISR & RTC_ISR_TSOVF) {
        RTC->ISR &= ~(RTC_ISR_TSOVF);
    }
}

/***************************************************************************
Function: rtc_timestamp_read_date
Overview: Reads the timestamp date information and returns it into a structure
Parameters: 
    None
Return: 
    rtc_date_data: Structure of date info. Including the date, month, year, and weekday
Note: None
***************************************************************************/
rtc_date_data rtc_timestamp_read_date(void)
{
    rtc_date_data timestamp_data = {0};

    timestamp_data.date  = convert_bcd_to_dec((RTC->TSDR >> RTC_TSDR_DT_POS) & RTC_TSDR_DT_MASK,
                                              (RTC->TSDR >> RTC_TSDR_DU_POS) & RTC_TSDR_DU_MASK);
    timestamp_data.month = convert_bcd_to_dec((RTC->TSDR >> RTC_TSDR_MT_POS) & RTC_TSDR_MT_MASK,
                                              (RTC->TSDR >> RTC_TSDR_MU_POS) & RTC_TSDR_MU_MASK);
    timestamp_data.year  = 0;

    timestamp_data.weekday = (RTC->TSDR >> RTC_TSDR_WDU_POS) & RTC_TSDR_WDU_MASK;

    return timestamp_data;
}

/***************************************************************************
Function: rtc_timestamp_read_time
Overview: Reads the timestamp time information and returns it into a structure.
Parameters: 
    None
Return: 
    rtc_time_data: Structure of time info. Including the seconds, minutes, hours, and whether it is am/pm.
Note: The time format is not directly returned. It can be derived from the am/pm data.
***************************************************************************/
rtc_time_data rtc_timestamp_read_time(void)
{
    rtc_time_data timestamp_data = {0};

    timestamp_data.seconds    = convert_bcd_to_dec((RTC->TSTR >> RTC_TSTR_ST_POS) & RTC_TSTR_ST_MASK,
                                                   (RTC->TSTR >> RTC_TSTR_SU_POS) & RTC_TSTR_SU_MASK);
    timestamp_data.minutes    = convert_bcd_to_dec((RTC->TSTR >> RTC_TSTR_MNT_POS) & RTC_TSTR_MNT_MASK,
                                                   (RTC->TSTR >> RTC_TSTR_MNU_POS) & RTC_TSTR_MNU_MASK);
    timestamp_data.hours      = convert_bcd_to_dec((RTC->TSTR >> RTC_TSTR_HT_POS) & RTC_TSTR_HT_MASK,
                                                   (RTC->TSTR >> RTC_TSTR_HU_POS) & RTC_TSTR_HU_MASK);
    timestamp_data.time_am_pm = (RTC->TSTR >> RTC_TSTR_PM_POS) & RTC_TSTR_PM_MASK;

    return timestamp_data;
}

/***************************************************************************
Function: rtc_wakeup_init
Overview: Initializes the RTC wakeup feature with the settings in the wakeup_conf structure
Parameters: 
    wakeup_conf: Settings for wakeup 
Return: 
    None
Note: None
***************************************************************************/
void rtc_wakeup_init(rtc_wakeup_config wakeup_conf)
{
    RTC->CR &= ~RTC_CR_WUTE;

    // Wait until updates allowed
    while (!(RTC->ISR & RTC_ISR_WUTWF));

    RTC->CR &= ~(RTC_CR_WCKSEL_MASK << RTC_CR_WCKSEL_POS);
    RTC->CR |= wakeup_conf.clk_sel << RTC_CR_WCKSEL_POS;

    RTC->WUTR &= ~RTC_WUTR_WUT_MASK;
    RTC->WUTR |= wakeup_conf.auto_reload_value;

    if (wakeup_conf.it_opt == RTC_IT_OPT_WAKEUP) {
        RTC->CR |= RTC_CR_WUTIE;
        rtc_it_config(wakeup_conf.it_opt, ENABLE);
    }

    rtc_wakeup_clear_status();

    RTC->CR |= RTC_CR_WUTE;
}

/***************************************************************************
Function: rtc_wakeup_get_status
Overview: Reads the status of the wakeup and whether it has been triggered
Parameters: 
    None
Return: 
    rtc_wakeup_status_e: 
        RTC_WAKEUP_STATUS_NOT_TRIGGERED 
        RTC_WAKEUP_STATUS_TRIGGERED     
Note: None
***************************************************************************/
rtc_wakeup_status_e rtc_wakeup_get_status(void)
{
    return ((RTC->ISR >> RTC_ISR_WUTF_POS) & RTC_ISR_WUTF_MASK);
}

/***************************************************************************
Function: rtc_wakeup_clear_status
Overview: Clears the status of the wakeup section of RTC
Parameters: 
    None
Return: 
    None
Note: None
***************************************************************************/
void rtc_wakeup_clear_status(void)
{
    RTC->ISR &= ~(RTC_ISR_WUTF_MASK << RTC_ISR_WUTF_POS);
}

/***************************************************************************
Function: rtc_it_handler
Overview: Interrupt handler for RTC interrupts. Clears flags and ITs. 
Parameters: 
    None
Return: 
    None
Note: Should be called within user written RTC specific IT handler
***************************************************************************/
void rtc_it_handler(void)
{
    uint32_t temp_isr = RTC->ISR;

    if (temp_isr & RTC_ISR_ALRAF) {
        RTC->ISR &= ~RTC_ISR_ALRAF;
        exti_clear_pending((exti_lines_e)RTC_EXTI_NO_ALARM);
    }

    if (temp_isr & RTC_ISR_ALRBF) {
        RTC->ISR &= ~RTC_ISR_ALRBF;
        exti_clear_pending((exti_lines_e)RTC_EXTI_NO_ALARM);
    }

    if (temp_isr & RTC_ISR_WUTF) {
        RTC->ISR &= ~RTC_ISR_WUTF;
        exti_clear_pending((exti_lines_e)RTC_EXTI_NO_WAKE_UP);
    }

    if (temp_isr & RTC_ISR_TSF) {
        RTC->ISR &= ~RTC_ISR_TSF;
        rtc_timestamp_clear_status();
        exti_clear_pending((exti_lines_e)RTC_EXTI_NO_TIMESTAMP);
    }
}

//======================================================================================//}
//                  Helper Function Implementation
//======================================================================================//{

static void congifure_rtc_clock(rtc_config const rtc_conf)
{
    congifure_rtc_clock_asserts(rtc_conf);

    rcc_set_rtc_clk_src(rtc_conf.rtc_sel);
    if (rtc_conf.rtc_sel == RCC_RTC_CLK_SRC_LSI) {
        rcc_lsi_enable();
        while (!rcc_lsi_rdy());
    }
    if (rtc_conf.rtc_sel == RCC_RTC_CLK_SRC_HSE) {
        rcc_set_rtc_pre(rtc_conf.rtc_pre);
    }
}

static void congifure_rtc_clock_asserts(rtc_config const rtc_conf)
{
    uint8_t found_setting = false;
    switch (rtc_conf.rtc_sel) {
    case RCC_RTC_CLK_SRC_NA:  found_setting = true; break;
    case RCC_RTC_CLK_SRC_LSE: found_setting = true; break;
    case RCC_RTC_CLK_SRC_LSI: found_setting = true; break;
    case RCC_RTC_CLK_SRC_HSE: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (rtc_conf.rtc_pre) {
    case RCC_RTC_HSE_PRE_NA: found_setting = true; break;
    case RCC_RTC_HSE_PRE_2:  found_setting = true; break;
    case RCC_RTC_HSE_PRE_3:  found_setting = true; break;
    case RCC_RTC_HSE_PRE_4:  found_setting = true; break;
    case RCC_RTC_HSE_PRE_5:  found_setting = true; break;
    case RCC_RTC_HSE_PRE_6:  found_setting = true; break;
    case RCC_RTC_HSE_PRE_7:  found_setting = true; break;
    case RCC_RTC_HSE_PRE_8:  found_setting = true; break;
    case RCC_RTC_HSE_PRE_9:  found_setting = true; break;
    case RCC_RTC_HSE_PRE_10: found_setting = true; break;
    case RCC_RTC_HSE_PRE_11: found_setting = true; break;
    case RCC_RTC_HSE_PRE_12: found_setting = true; break;
    case RCC_RTC_HSE_PRE_13: found_setting = true; break;
    case RCC_RTC_HSE_PRE_14: found_setting = true; break;
    case RCC_RTC_HSE_PRE_15: found_setting = true; break;
    case RCC_RTC_HSE_PRE_16: found_setting = true; break;
    case RCC_RTC_HSE_PRE_17: found_setting = true; break;
    case RCC_RTC_HSE_PRE_18: found_setting = true; break;
    case RCC_RTC_HSE_PRE_19: found_setting = true; break;
    case RCC_RTC_HSE_PRE_20: found_setting = true; break;
    case RCC_RTC_HSE_PRE_21: found_setting = true; break;
    case RCC_RTC_HSE_PRE_22: found_setting = true; break;
    case RCC_RTC_HSE_PRE_23: found_setting = true; break;
    case RCC_RTC_HSE_PRE_24: found_setting = true; break;
    case RCC_RTC_HSE_PRE_25: found_setting = true; break;
    case RCC_RTC_HSE_PRE_26: found_setting = true; break;
    case RCC_RTC_HSE_PRE_27: found_setting = true; break;
    case RCC_RTC_HSE_PRE_28: found_setting = true; break;
    case RCC_RTC_HSE_PRE_29: found_setting = true; break;
    case RCC_RTC_HSE_PRE_30: found_setting = true; break;
    case RCC_RTC_HSE_PRE_31: found_setting = true; break;
    }
    ASSERT(found_setting);
}

static void set_rtc_time(rtc_time_data const time_data)
{
    uint32_t temp_tr = RTC->TR;
    uint32_t temp_cr = RTC->CR;

    time_data_asserts(time_data);

    // Set general time format
    temp_cr &= ~(RTC_CR_FMT_MASK << RTC_CR_FMT_POS);
    temp_cr |= time_data.time_format << RTC_CR_FMT_POS;

    // Set current time format
    temp_tr &= ~(RTC_TR_PM_MASK << RTC_TR_PM_POS);
    temp_tr |= time_data.time_am_pm << RTC_TR_PM_POS;

    // Set seconds units
    temp_tr &= ~(RTC_TR_SU_MASK << RTC_TR_SU_POS);
    temp_tr |= (time_data.seconds % 10) << RTC_TR_SU_POS;

    // Set seconds tens
    temp_tr &= ~(RTC_TR_ST_MASK << RTC_TR_ST_POS);
    temp_tr |= (time_data.seconds / 10) << RTC_TR_ST_POS;

    // Set minutes units
    temp_tr &= ~(RTC_TR_MNU_MASK << RTC_TR_MNU_POS);
    temp_tr |= (time_data.minutes % 10) << RTC_TR_MNU_POS;

    // Set minutes tens
    temp_tr &= ~(RTC_TR_MNT_MASK << RTC_TR_MNT_POS);
    temp_tr |= (time_data.minutes / 10) << RTC_TR_MNT_POS;

    // Set hours units
    temp_tr &= ~(RTC_TR_HU_MASK << RTC_TR_HU_POS);
    temp_tr |= (time_data.hours % 10) << RTC_TR_HU_POS;

    // Set hours tens
    temp_tr &= ~(RTC_TR_HT_MASK << RTC_TR_HT_POS);
    temp_tr |= (time_data.hours / 10) << RTC_TR_HT_POS;

    RTC->CR = temp_cr;
    RTC->TR = temp_tr;
}

static void time_data_asserts(rtc_time_data const time_data)
{
    assert_time_format(time_data.time_format);
    assert_am_pm(time_data.time_am_pm);
    assert_hours(time_data.hours);
    assert_minutes(time_data.minutes);
    assert_seconds(time_data.seconds);
}

static void set_rtc_date(rtc_date_data const date_data)
{
    uint32_t temp_dr = RTC->DR;

    date_data_asserts(date_data);

    // Set weekday
    temp_dr &= ~(RTC_DR_WDU_MASK << RTC_DR_WDU_POS);
    temp_dr |= date_data.weekday << RTC_DR_WDU_POS;

    // Set date units
    temp_dr &= ~(RTC_DR_DU_MASK << RTC_DR_DU_POS);
    temp_dr |= (date_data.date % 10) << RTC_DR_DU_POS;

    // Set date tens
    temp_dr &= ~(RTC_DR_DT_MASK << RTC_DR_DT_POS);
    temp_dr |= (date_data.date / 10) << RTC_DR_DT_POS;

    // Set month units
    temp_dr &= ~(RTC_DR_MU_MASK << RTC_DR_MU_POS);
    temp_dr |= (date_data.month % 10) << RTC_DR_MU_POS;

    // Set month tens
    temp_dr &= ~(RTC_DR_MT_MASK << RTC_DR_MT_POS);
    temp_dr |= (date_data.month / 10) << RTC_DR_MT_POS;

    // Set year units
    temp_dr &= ~(RTC_DR_YU_MASK << RTC_DR_YU_POS);
    temp_dr |= (date_data.year % 10) << RTC_DR_YU_POS;

    // Set year tens
    temp_dr &= ~(RTC_DR_YT_MASK << RTC_DR_YT_POS);
    temp_dr |= (date_data.year / 10) << RTC_DR_YT_POS;

    RTC->DR = temp_dr;
}

static void date_data_asserts(rtc_date_data const date_data)
{
    assert_weekday(date_data.weekday);
    assert_date(date_data.date);
    assert_month(date_data.month);
    assert_year(date_data.year);
}

static inline uint8_t convert_bcd_to_dec(uint8_t const tens, uint8_t const units)
{
    uint8_t decimal = 0;
    decimal         = (tens * 10) + units;
    return decimal;
}

static inline void unlock_rtc(void)
{
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
}

static void set_rtc_pre(void)
{
    uint8_t const FACTOR_2 = 2;
    uint32_t synch_pre;
    uint32_t asynch_pre;
    uint32_t rtc_clk = 0;

    // Procedure:
    // Calculate asynch prescaler
    // Calculate synch prescaler from *asynch clock*
    // Set synch prescaler in reg
    // Set asynch prescaler in reg

    rtc_clk = rcc_get_rtc_clock_freq_hz();

    asynch_pre = 128; // Highest 7-bit number
    // If dividing RTC input clock results in remainder, then reduce prescaler by factor of 2
    while (rtc_clk % asynch_pre) {
        asynch_pre /= FACTOR_2;
    }

    synch_pre = (rtc_clk / asynch_pre);

    synch_pre -= 1;
    RTC->PRER &= ~(RTC_PRER_PREDIV_S_MASK << RTC_PRER_PREDIV_S_POS);
    RTC->PRER |= synch_pre << RTC_PRER_PREDIV_S_POS;

    asynch_pre -= 1;
    RTC->PRER &= ~(RTC_PRER_PREDIV_A_MASK << RTC_PRER_PREDIV_A_POS);
    RTC->PRER |= asynch_pre << RTC_PRER_PREDIV_A_POS;
}

static void enable_alarm(rtc_alarm_sel_e const alarm_sel, togglable_e const it_toggle)
{
    switch (alarm_sel) {
    case RTC_ALARM_A_SEL:
        RTC->CR |= RTC_CR_ALRAE;
        if (it_toggle == ENABLE) {
            RTC->CR |= RTC_CR_ALRAIE;
        }
        break;
    case RTC_ALARM_B_SEL:
        RTC->CR |= RTC_CR_ALRBE;
        if (it_toggle == ENABLE) {
            RTC->CR |= RTC_CR_ALRBIE;
        }
        break;
    }
}

static void disable_alarm(rtc_alarm_sel_e const alarm_sel)
{
    // Turn off interrupts so we can modify the alarm
    switch (alarm_sel) {
    case RTC_ALARM_A_SEL:
        // Wait till alarm update is allowed
        while (!(RTC->ISR & RTC_ISR_ALRAWF));
        RTC->CR &= ~RTC_CR_ALRAE;
        RTC->CR &= ~RTC_CR_ALRAIE;
        break;

    case RTC_ALARM_B_SEL:
        // Wait till alarm update is allowed
        while (!(RTC->ISR & RTC_ISR_ALRBWF));
        RTC->CR &= ~RTC_CR_ALRBE;
        RTC->CR &= ~RTC_CR_ALRBIE;
        break;
    }
}

static inline void assert_seconds(uint8_t const seconds)
{
    ASSERT(seconds <= 59);
}

static inline void assert_minutes(uint8_t const minutes)
{
    ASSERT(minutes <= 59);
}

static inline void assert_hours(uint8_t const hours)
{
    ASSERT(hours <= 23);
}

static inline void assert_am_pm(rtc_time_am_pm_e const am_pm)
{
    ASSERT((am_pm == RTC_TIME_AM) || (am_pm == RTC_TIME_PM));
}

static inline void assert_time_format(rtc_time_format_e const time_format)
{
    ASSERT((time_format == RTC_TIME_FORMAT_12HR) || (time_format == RTC_TIME_FORMAT_24HR));
}

static inline void assert_date(uint8_t const date)
{
    ASSERT(date <= 31);
}

static inline void assert_month(uint8_t const month)
{
    ASSERT(month <= 12);
}

static inline void assert_year(uint8_t const year)
{
    ASSERT(year <= 99);
}

static inline void assert_weekday(rtc_weekdays_e const weekday)
{
    uint8_t found_setting = false;
    switch (weekday) {
    case RTC_WEEKDAY_MONDAY:    found_setting = true; break;
    case RTC_WEEKDAY_TUESDAY:   found_setting = true; break;
    case RTC_WEEKDAY_WEDNESDAY: found_setting = true; break;
    case RTC_WEEKDAY_THURSDAY:  found_setting = true; break;
    case RTC_WEEKDAY_FRIDAY:    found_setting = true; break;
    case RTC_WEEKDAY_SATURDAY:  found_setting = true; break;
    case RTC_WEEKDAY_SUNDAY:    found_setting = true; break;
    }
    ASSERT(found_setting);
}
