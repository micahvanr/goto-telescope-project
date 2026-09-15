#include "rtc.h"
#include "assert_handler.h"
#include "common.h"
#include "pwr.h"
#include "rcc.h"
#include <stdint.h>

// Outline:
// Get and configure RTC clock
//  Make Asynch clock as high as possible

//======================================================================================//}
//                  Helper Function Prototypes
//======================================================================================//{

static void congifure_rtc_clock(rtc_config rtc_conf);
static void congifure_rtc_clock_asserts(rtc_config const rtc_conf);
static void set_rtc_time(rtc_time_data time_data);
static void set_rtc_time_asserts(rtc_time_data const time_data);
static void set_rtc_date(rtc_date_data date_data);
static void set_rtc_date_asserts(rtc_date_data const time_data);
static inline uint8_t convert_bcd_to_dec(uint8_t tens, uint8_t units);
static inline void unlock_rtc(void);
static void set_rtc_pre(void);

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
    time_data.time_format  = (TEMP_TR >> RTC_TR_PM_POS) & RTC_TR_PM_MASK;
    time_data.time_am_pm   = (TEMP_CR >> RTC_CR_FMT_POS) & RTC_CR_FMT_MASK;
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

    set_rtc_time_asserts(time_data);

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

static void set_rtc_time_asserts(rtc_time_data const time_data)
{
    ASSERT((time_data.time_am_pm == RTC_TIME_AM) || (time_data.time_am_pm == RTC_TIME_PM));
    ASSERT((time_data.time_format == RTC_TIME_FORMAT_12HR) || (time_data.time_format == RTC_TIME_FORMAT_24HR));
    ASSERT(time_data.hours <= 23);
    ASSERT(time_data.minutes <= 59);
    ASSERT(time_data.seconds <= 59);
}

static void set_rtc_date(rtc_date_data const date_data)
{
    uint32_t temp_dr = RTC->DR;

    set_rtc_date_asserts(date_data);

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

static void set_rtc_date_asserts(rtc_date_data const date_data)
{
    uint8_t found_setting = false;
    switch (date_data.weekday) {
    case RTC_WEEKDAY_MONDAY:    found_setting = true; break;
    case RTC_WEEKDAY_TUESDAY:   found_setting = true; break;
    case RTC_WEEKDAY_WEDNESDAY: found_setting = true; break;
    case RTC_WEEKDAY_THURSDAY:  found_setting = true; break;
    case RTC_WEEKDAY_FRIDAY:    found_setting = true; break;
    case RTC_WEEKDAY_SATURDAY:  found_setting = true; break;
    case RTC_WEEKDAY_SUNDAY:    found_setting = true; break;
    }
    ASSERT(found_setting);

    ASSERT(date_data.year <= 99);
    ASSERT(date_data.month <= 12);
    ASSERT(date_data.date <= 31);
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
