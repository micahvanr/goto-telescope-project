#ifndef RTC_H
#define RTC_H

#include "common.h"
#include "exti.h"
#include "rcc.h"
#include "stm32f4xx.h"

//======================================================================================//}
//                  Address Definitions
//======================================================================================//{

typedef enum {
    RTC_BASE_ADDR = ((APB1_BASE_ADDR) + (0x2800U))
} rtc_base_addr_e;

//======================================================================================//}
//                  Peripheral Constants
//======================================================================================//{

//} Misc. Constants
//=========================================//{

// TODO: Add applications section for enums
// Register names or function names

typedef enum {
    RTC_EXTI_NO_ALARM     = EXTI_LINE_NO_17,
    RTC_EXTI_NO_WAKE_UP   = EXTI_LINE_NO_22,
    RTC_EXTI_NO_TIMESTAMP = EXTI_LINE_NO_21,
} rtc_exti_line_no_e;

// Applications:
// - RTC ISR ALARxF
typedef enum {
    RTC_ALARM_STATUS_NOT_TRIGGERED = 0b0,
    RTC_ALARM_STATUS_TRIGGERED     = 0b1,
} rtc_alarm_status_e;

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

//======================================================================================//}
//                  Register Constants
//======================================================================================//{

// clang-format off
typedef enum : uint32_t {
    RTC_TR_PM_POS  = 22U,   // AM/PM notation
    RTC_TR_HT_POS  = 20U,   // Hour tens in BCD format
    RTC_TR_HU_POS  = 16U,   // Hour units in BCD format
    RTC_TR_MNT_POS = 12U,   // Minute tens in BCD format
    RTC_TR_MNU_POS = 8U,    // Minute units in BCD format
    RTC_TR_ST_POS  = 4U,    // Second tens in BCD format
    RTC_TR_SU_POS  = 0U,    // Second units in BCD format
} rtc_tr_pos_e;

typedef enum : uint32_t {
    RTC_TR_PM  = (1U << RTC_TR_PM_POS),    // AM/PM notation
    RTC_TR_HT  = (1U << RTC_TR_HT_POS),    // Hour tens in BCD format
    RTC_TR_HU  = (1U << RTC_TR_HU_POS),    // Hour units in BCD format
    RTC_TR_MNT = (1U << RTC_TR_MNT_POS),   // Minute tens in BCD format
    RTC_TR_MNU = (1U << RTC_TR_MNU_POS),   // Minute units in BCD format
    RTC_TR_ST  = (1U << RTC_TR_ST_POS),    // Second tens in BCD format
    RTC_TR_SU  = (1U << RTC_TR_SU_POS),    // Second units in BCD format
} rtc_tr_e;

typedef enum : uint32_t {
    RTC_TR_PM_MASK  = 0b1U,      // 1 bit(s)
    RTC_TR_HT_MASK  = 0b11U,     // 2 bit(s)
    RTC_TR_HU_MASK  = 0b1111U,   // 4 bit(s)
    RTC_TR_MNT_MASK = 0b111U,    // 3 bit(s)
    RTC_TR_MNU_MASK = 0b1111U,   // 4 bit(s)
    RTC_TR_ST_MASK  = 0b111U,    // 3 bit(s)
    RTC_TR_SU_MASK  = 0b1111U,   // 4 bit(s)
} rtc_tr_mask_e;

typedef enum : uint32_t {
    RTC_DR_YT_POS  = 20U,   // Year tens in BCD format
    RTC_DR_YU_POS  = 16U,   // Year units in BCD format
    RTC_DR_WDU_POS = 13U,   // Week day units
    RTC_DR_MT_POS  = 12U,   // Month tens in BCD format
    RTC_DR_MU_POS  = 8U,    // Month units in BCD format
    RTC_DR_DT_POS  = 4U,    // Date tens in BCD format
    RTC_DR_DU_POS  = 0U,    // Date units in BCD format
} rtc_dr_pos_e;

typedef enum : uint32_t {
    RTC_DR_YT  = (1U << RTC_DR_YT_POS),    // Year tens in BCD format
    RTC_DR_YU  = (1U << RTC_DR_YU_POS),    // Year units in BCD format
    RTC_DR_WDU = (1U << RTC_DR_WDU_POS),   // Week day units
    RTC_DR_MT  = (1U << RTC_DR_MT_POS),    // Month tens in BCD format
    RTC_DR_MU  = (1U << RTC_DR_MU_POS),    // Month units in BCD format
    RTC_DR_DT  = (1U << RTC_DR_DT_POS),    // Date tens in BCD format
    RTC_DR_DU  = (1U << RTC_DR_DU_POS),    // Date units in BCD format
} rtc_dr_e;

typedef enum : uint32_t {
    RTC_DR_YT_MASK  = 0b1111U,   // 4 bit(s)
    RTC_DR_YU_MASK  = 0b1111U,   // 4 bit(s)
    RTC_DR_WDU_MASK = 0b111U,    // 3 bit(s)
    RTC_DR_MT_MASK  = 0b1U,      // 1 bit(s)
    RTC_DR_MU_MASK  = 0b1111U,   // 4 bit(s)
    RTC_DR_DT_MASK  = 0b11U,     // 2 bit(s)
    RTC_DR_DU_MASK  = 0b1111U,   // 4 bit(s)
} rtc_dr_mask_e;

typedef enum : uint32_t {
    RTC_CR_COE_POS     = 23U,   // Calibration output enable
    RTC_CR_OSEL_POS    = 21U,   // Output selection
    RTC_CR_POL_POS     = 20U,   // Output polarity
    RTC_CR_BKP_POS     = 18U,   // Backup
    RTC_CR_SUB1H_POS   = 17U,   // Subtract 1 hour (winter time change)
    RTC_CR_ADD1H_POS   = 16U,   // Add 1 hour (summer time change)
    RTC_CR_TSIE_POS    = 15U,   // Time-stamp interrupt enable
    RTC_CR_WUTIE_POS   = 14U,   // Wakeup timer interrupt enable
    RTC_CR_ALRBIE_POS  = 13U,   // Alarm B interrupt enable
    RTC_CR_ALRAIE_POS  = 12U,   // Alarm A interrupt enable
    RTC_CR_TSE_POS     = 11U,   // Time stamp enable
    RTC_CR_WUTE_POS    = 10U,   // Wakeup timer enable
    RTC_CR_ALRBE_POS   = 9U,    // Alarm B enable
    RTC_CR_ALRAE_POS   = 8U,    // Alarm A enable
    RTC_CR_DCE_POS     = 7U,    // Coarse digital calibration enable
    RTC_CR_FMT_POS     = 6U,    // Hour format
    RTC_CR_REFCKON_POS = 4U,    // Reference clock detection enable (50 or 60 Hz)
    RTC_CR_TSEDGE_POS  = 3U,    // Time-stamp event active edge
    RTC_CR_WCKSEL_POS  = 0U,    // Wakeup clock selection
} rtc_cr_pos_e;

typedef enum : uint32_t {
    RTC_CR_COE     = (1U << RTC_CR_COE_POS),       // Calibration output enable
    RTC_CR_OSEL    = (1U << RTC_CR_OSEL_POS),      // Output selection
    RTC_CR_POL     = (1U << RTC_CR_POL_POS),       // Output polarity
    RTC_CR_BKP     = (1U << RTC_CR_BKP_POS),       // Backup
    RTC_CR_SUB1H   = (1U << RTC_CR_SUB1H_POS),     // Subtract 1 hour (winter time change)
    RTC_CR_ADD1H   = (1U << RTC_CR_ADD1H_POS),     // Add 1 hour (summer time change)
    RTC_CR_TSIE    = (1U << RTC_CR_TSIE_POS),      // Time-stamp interrupt enable
    RTC_CR_WUTIE   = (1U << RTC_CR_WUTIE_POS),     // Wakeup timer interrupt enable
    RTC_CR_ALRBIE  = (1U << RTC_CR_ALRBIE_POS),    // Alarm B interrupt enable
    RTC_CR_ALRAIE  = (1U << RTC_CR_ALRAIE_POS),    // Alarm A interrupt enable
    RTC_CR_TSE     = (1U << RTC_CR_TSE_POS),       // Time stamp enable
    RTC_CR_WUTE    = (1U << RTC_CR_WUTE_POS),      // Wakeup timer enable
    RTC_CR_ALRBE   = (1U << RTC_CR_ALRBE_POS),     // Alarm B enable
    RTC_CR_ALRAE   = (1U << RTC_CR_ALRAE_POS),     // Alarm A enable
    RTC_CR_DCE     = (1U << RTC_CR_DCE_POS),       // Coarse digital calibration enable
    RTC_CR_FMT     = (1U << RTC_CR_FMT_POS),       // Hour format
    RTC_CR_REFCKON = (1U << RTC_CR_REFCKON_POS),   // Reference clock detection enable (50 or 60 Hz)
    RTC_CR_TSEDGE  = (1U << RTC_CR_TSEDGE_POS),    // Time-stamp event active edge
    RTC_CR_WCKSEL  = (1U << RTC_CR_WCKSEL_POS),    // Wakeup clock selection
} rtc_cr_e;

typedef enum : uint32_t {
    RTC_CR_COE_MASK     = 0b1U,     // 1 bit(s)
    RTC_CR_OSEL_MASK    = 0b11U,    // 2 bit(s)
    RTC_CR_POL_MASK     = 0b1U,     // 1 bit(s)
    RTC_CR_BKP_MASK     = 0b1U,     // 1 bit(s)
    RTC_CR_SUB1H_MASK   = 0b1U,     // 1 bit(s)
    RTC_CR_ADD1H_MASK   = 0b1U,     // 1 bit(s)
    RTC_CR_TSIE_MASK    = 0b1U,     // 1 bit(s)
    RTC_CR_WUTIE_MASK   = 0b1U,     // 1 bit(s)
    RTC_CR_ALRBIE_MASK  = 0b1U,     // 1 bit(s)
    RTC_CR_ALRAIE_MASK  = 0b1U,     // 1 bit(s)
    RTC_CR_TSE_MASK     = 0b1U,     // 1 bit(s)
    RTC_CR_WUTE_MASK    = 0b1U,     // 1 bit(s)
    RTC_CR_ALRBE_MASK   = 0b1U,     // 1 bit(s)
    RTC_CR_ALRAE_MASK   = 0b1U,     // 1 bit(s)
    RTC_CR_DCE_MASK     = 0b1U,     // 1 bit(s)
    RTC_CR_FMT_MASK     = 0b1U,     // 1 bit(s)
    RTC_CR_REFCKON_MASK = 0b1U,     // 1 bit(s)
    RTC_CR_TSEDGE_MASK  = 0b1U,     // 1 bit(s)
    RTC_CR_WCKSEL_MASK  = 0b111U,   // 3 bit(s)
} rtc_cr_mask_e;

typedef enum : uint32_t {
    RTC_ISR_ALRAWF_POS  = 0U,    // Alarm A write flag
    RTC_ISR_ALRBWF_POS  = 1U,    // Alarm B write flag
    RTC_ISR_WUTWF_POS   = 2U,    // Wakeup timer write flag
    RTC_ISR_SHPF_POS    = 3U,    // Shift operation pending
    RTC_ISR_INITS_POS   = 4U,    // Initialization status flag
    RTC_ISR_RSF_POS     = 5U,    // Registers synchronization flag
    RTC_ISR_INITF_POS   = 6U,    // Initialization flag
    RTC_ISR_INIT_POS    = 7U,    // Initialization mode
    RTC_ISR_ALRAF_POS   = 8U,    // Alarm A flag
    RTC_ISR_ALRBF_POS   = 9U,    // Alarm B flag
    RTC_ISR_WUTF_POS    = 10U,   // Wakeup timer flag
    RTC_ISR_TSF_POS     = 11U,   // Time-stamp flag
    RTC_ISR_TSOVF_POS   = 12U,   // Time-stamp overflow flag
    RTC_ISR_TAMP1F_POS  = 13U,   // Tamper detection flag
    RTC_ISR_TAMP2F_POS  = 14U,   // TAMPER2 detection flag
    RTC_ISR_RECALPF_POS = 16U,   // Recalibration pending Flag
} rtc_isr_pos_e;

typedef enum : uint32_t {
    RTC_ISR_ALRAWF  = (1U << RTC_ISR_ALRAWF_POS),    // Alarm A write flag
    RTC_ISR_ALRBWF  = (1U << RTC_ISR_ALRBWF_POS),    // Alarm B write flag
    RTC_ISR_WUTWF   = (1U << RTC_ISR_WUTWF_POS),     // Wakeup timer write flag
    RTC_ISR_SHPF    = (1U << RTC_ISR_SHPF_POS),      // Shift operation pending
    RTC_ISR_INITS   = (1U << RTC_ISR_INITS_POS),     // Initialization status flag
    RTC_ISR_RSF     = (1U << RTC_ISR_RSF_POS),       // Registers synchronization flag
    RTC_ISR_INITF   = (1U << RTC_ISR_INITF_POS),     // Initialization flag
    RTC_ISR_INIT    = (1U << RTC_ISR_INIT_POS),      // Initialization mode
    RTC_ISR_ALRAF   = (1U << RTC_ISR_ALRAF_POS),     // Alarm A flag
    RTC_ISR_ALRBF   = (1U << RTC_ISR_ALRBF_POS),     // Alarm B flag
    RTC_ISR_WUTF    = (1U << RTC_ISR_WUTF_POS),      // Wakeup timer flag
    RTC_ISR_TSF     = (1U << RTC_ISR_TSF_POS),       // Time-stamp flag
    RTC_ISR_TSOVF   = (1U << RTC_ISR_TSOVF_POS),     // Time-stamp overflow flag
    RTC_ISR_TAMP1F  = (1U << RTC_ISR_TAMP1F_POS),    // Tamper detection flag
    RTC_ISR_TAMP2F  = (1U << RTC_ISR_TAMP2F_POS),    // TAMPER2 detection flag
    RTC_ISR_RECALPF = (1U << RTC_ISR_RECALPF_POS),   // Recalibration pending Flag
} rtc_isr_e;

typedef enum : uint32_t {
    RTC_ISR_ALRAWF_MASK  = 0b1U,   // 1 bit(s)
    RTC_ISR_ALRBWF_MASK  = 0b1U,   // 1 bit(s)
    RTC_ISR_WUTWF_MASK   = 0b1U,   // 1 bit(s)
    RTC_ISR_SHPF_MASK    = 0b1U,   // 1 bit(s)
    RTC_ISR_INITS_MASK   = 0b1U,   // 1 bit(s)
    RTC_ISR_RSF_MASK     = 0b1U,   // 1 bit(s)
    RTC_ISR_INITF_MASK   = 0b1U,   // 1 bit(s)
    RTC_ISR_INIT_MASK    = 0b1U,   // 1 bit(s)
    RTC_ISR_ALRAF_MASK   = 0b1U,   // 1 bit(s)
    RTC_ISR_ALRBF_MASK   = 0b1U,   // 1 bit(s)
    RTC_ISR_WUTF_MASK    = 0b1U,   // 1 bit(s)
    RTC_ISR_TSF_MASK     = 0b1U,   // 1 bit(s)
    RTC_ISR_TSOVF_MASK   = 0b1U,   // 1 bit(s)
    RTC_ISR_TAMP1F_MASK  = 0b1U,   // 1 bit(s)
    RTC_ISR_TAMP2F_MASK  = 0b1U,   // 1 bit(s)
    RTC_ISR_RECALPF_MASK = 0b1U,   // 1 bit(s)
} rtc_isr_mask_e;

typedef enum : uint32_t {
    RTC_PRER_PREDIV_A_POS = 16U,   // Asynchronous prescaler factor
    RTC_PRER_PREDIV_S_POS = 0U,    // Synchronous prescaler factor
} rtc_prer_pos_e;

typedef enum : uint32_t {
    RTC_PRER_PREDIV_A = (1U << RTC_PRER_PREDIV_A_POS),   // Asynchronous prescaler factor
    RTC_PRER_PREDIV_S = (1U << RTC_PRER_PREDIV_S_POS),   // Synchronous prescaler factor
} rtc_prer_e;

typedef enum : uint32_t {
    RTC_PRER_PREDIV_A_MASK = 0b1111111U,           // 7 bit(s)
    RTC_PRER_PREDIV_S_MASK = 0b111111111111111U,   // 15 bit(s)
} rtc_prer_mask_e;

typedef enum : uint32_t {
    RTC_WUTR_WUT_POS = 0U,   // Wakeup auto-reload value bits
} rtc_wutr_pos_e;

typedef enum : uint32_t {
    RTC_WUTR_WUT = (1U << RTC_WUTR_WUT_POS),   // Wakeup auto-reload value bits
} rtc_wutr_e;

typedef enum : uint32_t {
    RTC_WUTR_WUT_MASK = 0b1111111111111111U,   // 16 bit(s)
} rtc_wutr_mask_e;

typedef enum : uint32_t {
    RTC_CALIBR_DCS_POS = 7U,   // Digital calibration sign
    RTC_CALIBR_DC_POS  = 0U,   // Digital calibration
} rtc_calibr_pos_e;

typedef enum : uint32_t {
    RTC_CALIBR_DCS = (1U << RTC_CALIBR_DCS_POS),   // Digital calibration sign
    RTC_CALIBR_DC  = (1U << RTC_CALIBR_DC_POS),    // Digital calibration
} rtc_calibr_e;

typedef enum : uint32_t {
    RTC_CALIBR_DCS_MASK = 0b1U,       // 1 bit(s)
    RTC_CALIBR_DC_MASK  = 0b11111U,   // 5 bit(s)
} rtc_calibr_mask_e;

typedef enum : uint32_t {
    RTC_ALRMAR_MSK4_POS  = 31U,   // Alarm A date mask
    RTC_ALRMAR_WDSEL_POS = 30U,   // Week day selection
    RTC_ALRMAR_DT_POS    = 28U,   // Date tens in BCD format
    RTC_ALRMAR_DU_POS    = 24U,   // Date units or day in BCD format
    RTC_ALRMAR_MSK3_POS  = 23U,   // Alarm A hours mask
    RTC_ALRMAR_PM_POS    = 22U,   // AM/PM notation
    RTC_ALRMAR_HT_POS    = 20U,   // Hour tens in BCD format
    RTC_ALRMAR_HU_POS    = 16U,   // Hour units in BCD format
    RTC_ALRMAR_MSK2_POS  = 15U,   // Alarm A minutes mask
    RTC_ALRMAR_MNT_POS   = 12U,   // Minute tens in BCD format
    RTC_ALRMAR_MNU_POS   = 8U,    // Minute units in BCD format
    RTC_ALRMAR_MSK1_POS  = 7U,    // Alarm A seconds mask
    RTC_ALRMAR_ST_POS    = 4U,    // Second tens in BCD format
    RTC_ALRMAR_SU_POS    = 0U,    // Second units in BCD format
} rtc_alrmar_pos_e;

typedef enum : uint32_t {
    RTC_ALRMAR_MSK4  = (1U << RTC_ALRMAR_MSK4_POS),    // Alarm A date mask
    RTC_ALRMAR_WDSEL = (1U << RTC_ALRMAR_WDSEL_POS),   // Week day selection
    RTC_ALRMAR_DT    = (1U << RTC_ALRMAR_DT_POS),      // Date tens in BCD format
    RTC_ALRMAR_DU    = (1U << RTC_ALRMAR_DU_POS),      // Date units or day in BCD format
    RTC_ALRMAR_MSK3  = (1U << RTC_ALRMAR_MSK3_POS),    // Alarm A hours mask
    RTC_ALRMAR_PM    = (1U << RTC_ALRMAR_PM_POS),      // AM/PM notation
    RTC_ALRMAR_HT    = (1U << RTC_ALRMAR_HT_POS),      // Hour tens in BCD format
    RTC_ALRMAR_HU    = (1U << RTC_ALRMAR_HU_POS),      // Hour units in BCD format
    RTC_ALRMAR_MSK2  = (1U << RTC_ALRMAR_MSK2_POS),    // Alarm A minutes mask
    RTC_ALRMAR_MNT   = (1U << RTC_ALRMAR_MNT_POS),     // Minute tens in BCD format
    RTC_ALRMAR_MNU   = (1U << RTC_ALRMAR_MNU_POS),     // Minute units in BCD format
    RTC_ALRMAR_MSK1  = (1U << RTC_ALRMAR_MSK1_POS),    // Alarm A seconds mask
    RTC_ALRMAR_ST    = (1U << RTC_ALRMAR_ST_POS),      // Second tens in BCD format
    RTC_ALRMAR_SU    = (1U << RTC_ALRMAR_SU_POS),      // Second units in BCD format
} rtc_alrmar_e;

typedef enum : uint32_t {
    RTC_ALRMAR_MSK4_MASK  = 0b1U,      // 1 bit(s)
    RTC_ALRMAR_WDSEL_MASK = 0b1U,      // 1 bit(s)
    RTC_ALRMAR_DT_MASK    = 0b11U,     // 2 bit(s)
    RTC_ALRMAR_DU_MASK    = 0b1111U,   // 4 bit(s)
    RTC_ALRMAR_MSK3_MASK  = 0b1U,      // 1 bit(s)
    RTC_ALRMAR_PM_MASK    = 0b1U,      // 1 bit(s)
    RTC_ALRMAR_HT_MASK    = 0b11U,     // 2 bit(s)
    RTC_ALRMAR_HU_MASK    = 0b1111U,   // 4 bit(s)
    RTC_ALRMAR_MSK2_MASK  = 0b1U,      // 1 bit(s)
    RTC_ALRMAR_MNT_MASK   = 0b111U,    // 3 bit(s)
    RTC_ALRMAR_MNU_MASK   = 0b1111U,   // 4 bit(s)
    RTC_ALRMAR_MSK1_MASK  = 0b1U,      // 1 bit(s)
    RTC_ALRMAR_ST_MASK    = 0b111U,    // 3 bit(s)
    RTC_ALRMAR_SU_MASK    = 0b1111U,   // 4 bit(s)
} rtc_alrmar_mask_e;

typedef enum : uint32_t {
    RTC_ALRMBR_MSK4_POS  = 31U,   // Alarm B date mask
    RTC_ALRMBR_WDSEL_POS = 30U,   // Week day selection
    RTC_ALRMBR_DT_POS    = 28U,   // Date tens in BCD format
    RTC_ALRMBR_DU_POS    = 24U,   // Date units or day in BCD format
    RTC_ALRMBR_MSK3_POS  = 23U,   // Alarm B hours mask
    RTC_ALRMBR_PM_POS    = 22U,   // AM/PM notation
    RTC_ALRMBR_HT_POS    = 20U,   // Hour tens in BCD format
    RTC_ALRMBR_HU_POS    = 16U,   // Hour units in BCD format
    RTC_ALRMBR_MSK2_POS  = 15U,   // Alarm B minutes mask
    RTC_ALRMBR_MNT_POS   = 12U,   // Minute tens in BCD format
    RTC_ALRMBR_MNU_POS   = 8U,    // Minute units in BCD format
    RTC_ALRMBR_MSK1_POS  = 7U,    // Alarm B seconds mask
    RTC_ALRMBR_ST_POS    = 4U,    // Second tens in BCD format
    RTC_ALRMBR_SU_POS    = 0U,    // Second units in BCD format
} rtc_alrmbr_pos_e;

typedef enum : uint32_t {
    RTC_ALRMBR_MSK4  = (1U << RTC_ALRMBR_MSK4_POS),    // Alarm B date mask
    RTC_ALRMBR_WDSEL = (1U << RTC_ALRMBR_WDSEL_POS),   // Week day selection
    RTC_ALRMBR_DT    = (1U << RTC_ALRMBR_DT_POS),      // Date tens in BCD format
    RTC_ALRMBR_DU    = (1U << RTC_ALRMBR_DU_POS),      // Date units or day in BCD format
    RTC_ALRMBR_MSK3  = (1U << RTC_ALRMBR_MSK3_POS),    // Alarm B hours mask
    RTC_ALRMBR_PM    = (1U << RTC_ALRMBR_PM_POS),      // AM/PM notation
    RTC_ALRMBR_HT    = (1U << RTC_ALRMBR_HT_POS),      // Hour tens in BCD format
    RTC_ALRMBR_HU    = (1U << RTC_ALRMBR_HU_POS),      // Hour units in BCD format
    RTC_ALRMBR_MSK2  = (1U << RTC_ALRMBR_MSK2_POS),    // Alarm B minutes mask
    RTC_ALRMBR_MNT   = (1U << RTC_ALRMBR_MNT_POS),     // Minute tens in BCD format
    RTC_ALRMBR_MNU   = (1U << RTC_ALRMBR_MNU_POS),     // Minute units in BCD format
    RTC_ALRMBR_MSK1  = (1U << RTC_ALRMBR_MSK1_POS),    // Alarm B seconds mask
    RTC_ALRMBR_ST    = (1U << RTC_ALRMBR_ST_POS),      // Second tens in BCD format
    RTC_ALRMBR_SU    = (1U << RTC_ALRMBR_SU_POS),      // Second units in BCD format
} rtc_alrmbr_e;

typedef enum : uint32_t {
    RTC_ALRMBR_MSK4_MASK  = 0b1U,      // 1 bit(s)
    RTC_ALRMBR_WDSEL_MASK = 0b1U,      // 1 bit(s)
    RTC_ALRMBR_DT_MASK    = 0b11U,     // 2 bit(s)
    RTC_ALRMBR_DU_MASK    = 0b1111U,   // 4 bit(s)
    RTC_ALRMBR_MSK3_MASK  = 0b1U,      // 1 bit(s)
    RTC_ALRMBR_PM_MASK    = 0b1U,      // 1 bit(s)
    RTC_ALRMBR_HT_MASK    = 0b11U,     // 2 bit(s)
    RTC_ALRMBR_HU_MASK    = 0b1111U,   // 4 bit(s)
    RTC_ALRMBR_MSK2_MASK  = 0b1U,      // 1 bit(s)
    RTC_ALRMBR_MNT_MASK   = 0b111U,    // 3 bit(s)
    RTC_ALRMBR_MNU_MASK   = 0b1111U,   // 4 bit(s)
    RTC_ALRMBR_MSK1_MASK  = 0b1U,      // 1 bit(s)
    RTC_ALRMBR_ST_MASK    = 0b111U,    // 3 bit(s)
    RTC_ALRMBR_SU_MASK    = 0b1111U,   // 4 bit(s)
} rtc_alrmbr_mask_e;

typedef enum : uint32_t {
    RTC_WPR_KEY_POS = 0U,   // Write protection key
} rtc_wpr_pos_e;

typedef enum : uint32_t {
    RTC_WPR_KEY = (1U << RTC_WPR_KEY_POS),   // Write protection key
} rtc_wpr_e;

typedef enum : uint32_t {
    RTC_WPR_KEY_MASK = 0b11111111U,   // 8 bit(s)
} rtc_wpr_mask_e;

typedef enum : uint32_t {
    RTC_SSR_SS_POS = 0U,   // Sub second value
} rtc_ssr_pos_e;

typedef enum : uint32_t {
    RTC_SSR_SS = (1U << RTC_SSR_SS_POS),   // Sub second value
} rtc_ssr_e;

typedef enum : uint32_t {
    RTC_SSR_SS_MASK = 0b1111111111111111U,   // 16 bit(s)
} rtc_ssr_mask_e;

typedef enum : uint32_t {
    RTC_SHIFTR_ADD1S_POS = 31U,   // Add one second
    RTC_SHIFTR_SUBFS_POS = 0U,    // Subtract a fraction of a second
} rtc_shiftr_pos_e;

typedef enum : uint32_t {
    RTC_SHIFTR_ADD1S = (1U << RTC_SHIFTR_ADD1S_POS),   // Add one second
    RTC_SHIFTR_SUBFS = (1U << RTC_SHIFTR_SUBFS_POS),   // Subtract a fraction of a second
} rtc_shiftr_e;

typedef enum : uint32_t {
    RTC_SHIFTR_ADD1S_MASK = 0b1U,                 // 1 bit(s)
    RTC_SHIFTR_SUBFS_MASK = 0b111111111111111U,   // 15 bit(s)
} rtc_shiftr_mask_e;

typedef enum : uint32_t {
    RTC_TSTR_PM_POS  = 22U, // AM/PM notation
    RTC_TSTR_HT_POS  = 20U, // Hour tens in BCD format
    RTC_TSTR_HU_POS  = 16U, // Hour units in BCD format
    RTC_TSTR_MNT_POS = 12U, // Minute tens in BCD format
    RTC_TSTR_MNU_POS = 8U,  // Minute units in BCD format
    RTC_TSTR_ST_POS  = 4U,  // Second tens in BCD format
    RTC_TSTR_SU_POS  = 0U,  // Second units in BCD format
} rtc_tstr_pos_e;

typedef enum : uint32_t {
    RTC_TSTR_PM  = (1U << RTC_TSTR_PM_POS),  // AM/PM notation
    RTC_TSTR_HT  = (1U << RTC_TSTR_HT_POS),  // Hour tens in BCD format
    RTC_TSTR_HU  = (1U << RTC_TSTR_HU_POS),  // Hour units in BCD format
    RTC_TSTR_MNT = (1U << RTC_TSTR_MNT_POS), // Minute tens in BCD format
    RTC_TSTR_MNU = (1U << RTC_TSTR_MNU_POS), // Minute units in BCD format
    RTC_TSTR_ST  = (1U << RTC_TSTR_ST_POS),  // Second tens in BCD format
    RTC_TSTR_SU  = (1U << RTC_TSTR_SU_POS),  // Second units in BCD format
} rtc_tstr_e;

typedef enum : uint32_t {
    RTC_TSTR_PM_MASK  = 0b1U,    // 1 bit(s)
    RTC_TSTR_HT_MASK  = 0b11U,   // 2 bit(s)
    RTC_TSTR_HU_MASK  = 0b1111U, // 4 bit(s)
    RTC_TSTR_MNT_MASK = 0b111U,  // 3 bit(s)
    RTC_TSTR_MNU_MASK = 0b1111U, // 4 bit(s)
    RTC_TSTR_ST_MASK  = 0b111U,  // 3 bit(s)
    RTC_TSTR_SU_MASK  = 0b1111U, // 1 bit(s)
} rtc_tstr_mask_e;

typedef enum : uint32_t {
    RTC_TSDR_WDU_POS = 13U,   // Week day units
    RTC_TSDR_MT_POS  = 12U,   // Month tens in BCD format
    RTC_TSDR_MU_POS  = 8U,    // Month units in BCD format
    RTC_TSDR_DT_POS  = 4U,    // Date tens in BCD format
    RTC_TSDR_DU_POS  = 0U,    // Date units in BCD format
} rtc_tsdr_pos_e;

typedef enum : uint32_t {
    RTC_TSDR_WDU = (1U << RTC_TSDR_WDU_POS),   // Week day units
    RTC_TSDR_MT  = (1U << RTC_TSDR_MT_POS),    // Month tens in BCD format
    RTC_TSDR_MU  = (1U << RTC_TSDR_MU_POS),    // Month units in BCD format
    RTC_TSDR_DT  = (1U << RTC_TSDR_DT_POS),    // Date tens in BCD format
    RTC_TSDR_DU  = (1U << RTC_TSDR_DU_POS),    // Date units in BCD format
} rtc_tsdr_e;

typedef enum : uint32_t {
    RTC_TSDR_WDU_MASK = 0b111U,    // 3 bit(s)
    RTC_TSDR_MT_MASK  = 0b1U,      // 1 bit(s)
    RTC_TSDR_MU_MASK  = 0b1111U,   // 4 bit(s)
    RTC_TSDR_DT_MASK  = 0b11U,     // 2 bit(s)
    RTC_TSDR_DU_MASK  = 0b1111U,   // 4 bit(s)
} rtc_tsdr_mask_e;

typedef enum : uint32_t {
    RTC_TSSSR_SS_POS = 0U,   // Sub second value
} rtc_tsssr_pos_e;

typedef enum : uint32_t {
    RTC_TSSSR_SS = (1U << RTC_TSSSR_SS_POS),   // Sub second value
} rtc_tsssr_e;

typedef enum : uint32_t {
    RTC_TSSSR_SS_MASK = 0b1111111111111111U,   // 16 bit(s)
} rtc_tsssr_mask_e;

typedef enum : uint32_t {
    RTC_CALR_CALP_POS   = 15U,   // Increase frequency of RTC by 488.5 ppm
    RTC_CALR_CALW8_POS  = 14U,   // Use an 8-second calibration cycle period
    RTC_CALR_CALW16_POS = 13U,   // Use a 16-second calibration cycle period
    RTC_CALR_CALM_POS   = 0U,    // Calibration minus
} rtc_calr_pos_e;

typedef enum : uint32_t {
    RTC_CALR_CALP   = (1U << RTC_CALR_CALP_POS),     // Increase frequency of RTC by 488.5 ppm
    RTC_CALR_CALW8  = (1U << RTC_CALR_CALW8_POS),    // Use an 8-second calibration cycle period
    RTC_CALR_CALW16 = (1U << RTC_CALR_CALW16_POS),   // Use a 16-second calibration cycle period
    RTC_CALR_CALM   = (1U << RTC_CALR_CALM_POS),     // Calibration minus
} rtc_calr_e;

typedef enum : uint32_t {
    RTC_CALR_CALP_MASK   = 0b1U,           // 1 bit(s)
    RTC_CALR_CALW8_MASK  = 0b1U,           // 1 bit(s)
    RTC_CALR_CALW16_MASK = 0b1U,           // 1 bit(s)
    RTC_CALR_CALM_MASK   = 0b111111111U,   // 9 bit(s)
} rtc_calr_mask_e;

typedef enum : uint32_t {
    RTC_TAFCR_ALARMOUTTYPE_POS = 18U,   // AFO_ALARM output type
    RTC_TAFCR_TSINSEL_POS      = 17U,   // TIMESTAMP mapping
    RTC_TAFCR_TAMP1INSEL_POS   = 16U,   // TAMPER1 mapping
    RTC_TAFCR_TAMPPUDIS_POS    = 15U,   // TAMPER pull-up disable
    RTC_TAFCR_TAMPPRCH_POS     = 13U,   // Tamper precharge duration
    RTC_TAFCR_TAMPFLT_POS      = 11U,   // Tamper filter count
    RTC_TAFCR_TAMPFREQ_POS     = 8U,    // Tamper sampling frequency
    RTC_TAFCR_TAMPTS_POS       = 7U,    // Activate timestamp on tamper detection event
    RTC_TAFCR_TAMP2TRG_POS     = 4U,    // Active level for tamper 2
    RTC_TAFCR_TAMP2E_POS       = 3U,    // Tamper 2 detection enable
    RTC_TAFCR_TAMPIE_POS       = 2U,    // Tamper interrupt enable
    RTC_TAFCR_TAMP1TRG_POS     = 1U,    // Active level for tamper 1
    RTC_TAFCR_TAMP1E_POS       = 0U,    // Tamper 1 detection enable
} rtc_tafcr_pos_e;

typedef enum : uint32_t {
    RTC_TAFCR_ALARMOUTTYPE = (1U << RTC_TAFCR_ALARMOUTTYPE_POS),   // AFO_ALARM output type
    RTC_TAFCR_TSINSEL      = (1U << RTC_TAFCR_TSINSEL_POS),        // TIMESTAMP mapping
    RTC_TAFCR_TAMP1INSEL   = (1U << RTC_TAFCR_TAMP1INSEL_POS),     // TAMPER1 mapping
    RTC_TAFCR_TAMPPUDIS    = (1U << RTC_TAFCR_TAMPPUDIS_POS),      // TAMPER pull-up disable
    RTC_TAFCR_TAMPPRCH     = (1U << RTC_TAFCR_TAMPPRCH_POS),       // Tamper precharge duration
    RTC_TAFCR_TAMPFLT      = (1U << RTC_TAFCR_TAMPFLT_POS),        // Tamper filter count
    RTC_TAFCR_TAMPFREQ     = (1U << RTC_TAFCR_TAMPFREQ_POS),       // Tamper sampling frequency
    RTC_TAFCR_TAMPTS       = (1U << RTC_TAFCR_TAMPTS_POS),         // Activate timestamp on tamper detection event
    RTC_TAFCR_TAMP2TRG     = (1U << RTC_TAFCR_TAMP2TRG_POS),       // Active level for tamper 2
    RTC_TAFCR_TAMP2E       = (1U << RTC_TAFCR_TAMP2E_POS),         // Tamper 2 detection enable
    RTC_TAFCR_TAMPIE       = (1U << RTC_TAFCR_TAMPIE_POS),         // Tamper interrupt enable
    RTC_TAFCR_TAMP1TRG     = (1U << RTC_TAFCR_TAMP1TRG_POS),       // Active level for tamper 1
    RTC_TAFCR_TAMP1E       = (1U << RTC_TAFCR_TAMP1E_POS),         // Tamper 1 detection enable
} rtc_tafcr_e;

typedef enum : uint32_t {
    RTC_TAFCR_ALARMOUTTYPE_MASK = 0b1U,     // 1 bit(s)
    RTC_TAFCR_TSINSEL_MASK      = 0b1U,     // 1 bit(s)
    RTC_TAFCR_TAMP1INSEL_MASK   = 0b1U,     // 1 bit(s)
    RTC_TAFCR_TAMPPUDIS_MASK    = 0b1U,     // 1 bit(s)
    RTC_TAFCR_TAMPPRCH_MASK     = 0b11U,    // 2 bit(s)
    RTC_TAFCR_TAMPFLT_MASK      = 0b11U,    // 2 bit(s)
    RTC_TAFCR_TAMPFREQ_MASK     = 0b111U,   // 3 bit(s)
    RTC_TAFCR_TAMPTS_MASK       = 0b1U,     // 1 bit(s)
    RTC_TAFCR_TAMP2TRG_MASK     = 0b1U,     // 1 bit(s)
    RTC_TAFCR_TAMP2E_MASK       = 0b1U,     // 1 bit(s)
    RTC_TAFCR_TAMPIE_MASK       = 0b1U,     // 1 bit(s)
    RTC_TAFCR_TAMP1TRG_MASK     = 0b1U,     // 1 bit(s)
    RTC_TAFCR_TAMP1E_MASK       = 0b1U,     // 1 bit(s)
} rtc_tafcr_mask_e;

typedef enum : uint32_t {
    RTC_ALRMASSR_MASKSS_POS = 24U,   // Mask the most-significant bits starting at this bit
    RTC_ALRMASSR_SS_POS     = 0U,    // Sub seconds value
} rtc_alrmassr_pos_e;

typedef enum : uint32_t {
    RTC_ALRMASSR_MASKSS = (1U << RTC_ALRMASSR_MASKSS_POS),   // Mask the most-significant bits starting at this bit
    RTC_ALRMASSR_SS     = (1U << RTC_ALRMASSR_SS_POS),       // Sub seconds value
} rtc_alrmassr_e;

typedef enum : uint32_t {
    RTC_ALRMASSR_MASKSS_MASK = 0b1111U,              // 4 bit(s)
    RTC_ALRMASSR_SS_MASK     = 0b111111111111111U,   // 15 bit(s)
} rtc_alrmassr_mask_e;

typedef enum : uint32_t {
    RTC_ALRMBSSR_MASKSS_POS = 24U,   // Mask the most-significant bits starting at this bit
    RTC_ALRMBSSR_SS_POS     = 0U,    // Sub seconds value
} rtc_alrmbssr_pos_e;

typedef enum : uint32_t {
    RTC_ALRMBSSR_MASKSS = (1U << RTC_ALRMBSSR_MASKSS_POS),   // Mask the most-significant bits starting at this bit
    RTC_ALRMBSSR_SS     = (1U << RTC_ALRMBSSR_SS_POS),       // Sub seconds value
} rtc_alrmbssr_e;

typedef enum : uint32_t {
    RTC_ALRMBSSR_MASKSS_MASK = 0b1111U,              // 4 bit(s)
    RTC_ALRMBSSR_SS_MASK     = 0b111111111111111U,   // 15 bit(s)
} rtc_alrmbssr_mask_e;

typedef enum : uint32_t {
    RTC_BKP0R_BKP_POS = 0U,   // BKP
} rtc_bkp0r_pos_e;

typedef enum : uint32_t {
    RTC_BKP0R_BKP = (1U << RTC_BKP0R_BKP_POS),   // BKP
} rtc_bkp0r_e;

typedef enum : uint32_t {
    RTC_BKP0R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp0r_mask_e;

typedef enum : uint32_t {
    RTC_BKP1R_BKP_POS = 0U,   // BKP
} rtc_bkp1r_pos_e;

typedef enum : uint32_t {
    RTC_BKP1R_BKP = (1U << RTC_BKP1R_BKP_POS),   // BKP
} rtc_bkp1r_e;

typedef enum : uint32_t {
    RTC_BKP1R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp1r_mask_e;

typedef enum : uint32_t {
    RTC_BKP2R_BKP_POS = 0U,   // BKP
} rtc_bkp2r_pos_e;

typedef enum : uint32_t {
    RTC_BKP2R_BKP = (1U << RTC_BKP2R_BKP_POS),   // BKP
} rtc_bkp2r_e;

typedef enum : uint32_t {
    RTC_BKP2R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp2r_mask_e;

typedef enum : uint32_t {
    RTC_BKP3R_BKP_POS = 0U,   // BKP
} rtc_bkp3r_pos_e;

typedef enum : uint32_t {
    RTC_BKP3R_BKP = (1U << RTC_BKP3R_BKP_POS),   // BKP
} rtc_bkp3r_e;

typedef enum : uint32_t {
    RTC_BKP3R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp3r_mask_e;

typedef enum : uint32_t {
    RTC_BKP4R_BKP_POS = 0U,   // BKP
} rtc_bkp4r_pos_e;

typedef enum : uint32_t {
    RTC_BKP4R_BKP = (1U << RTC_BKP4R_BKP_POS),   // BKP
} rtc_bkp4r_e;

typedef enum : uint32_t {
    RTC_BKP4R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp4r_mask_e;

typedef enum : uint32_t {
    RTC_BKP5R_BKP_POS = 0U,   // BKP
} rtc_bkp5r_pos_e;

typedef enum : uint32_t {
    RTC_BKP5R_BKP = (1U << RTC_BKP5R_BKP_POS),   // BKP
} rtc_bkp5r_e;

typedef enum : uint32_t {
    RTC_BKP5R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp5r_mask_e;

typedef enum : uint32_t {
    RTC_BKP6R_BKP_POS = 0U,   // BKP
} rtc_bkp6r_pos_e;

typedef enum : uint32_t {
    RTC_BKP6R_BKP = (1U << RTC_BKP6R_BKP_POS),   // BKP
} rtc_bkp6r_e;

typedef enum : uint32_t {
    RTC_BKP6R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp6r_mask_e;

typedef enum : uint32_t {
    RTC_BKP7R_BKP_POS = 0U,   // BKP
} rtc_bkp7r_pos_e;

typedef enum : uint32_t {
    RTC_BKP7R_BKP = (1U << RTC_BKP7R_BKP_POS),   // BKP
} rtc_bkp7r_e;

typedef enum : uint32_t {
    RTC_BKP7R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp7r_mask_e;

typedef enum : uint32_t {
    RTC_BKP8R_BKP_POS = 0U,   // BKP
} rtc_bkp8r_pos_e;

typedef enum : uint32_t {
    RTC_BKP8R_BKP = (1U << RTC_BKP8R_BKP_POS),   // BKP
} rtc_bkp8r_e;

typedef enum : uint32_t {
    RTC_BKP8R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp8r_mask_e;

typedef enum : uint32_t {
    RTC_BKP9R_BKP_POS = 0U,   // BKP
} rtc_bkp9r_pos_e;

typedef enum : uint32_t {
    RTC_BKP9R_BKP = (1U << RTC_BKP9R_BKP_POS),   // BKP
} rtc_bkp9r_e;

typedef enum : uint32_t {
    RTC_BKP9R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp9r_mask_e;

typedef enum : uint32_t {
    RTC_BKP10R_BKP_POS = 0U,   // BKP
} rtc_bkp10r_pos_e;

typedef enum : uint32_t {
    RTC_BKP10R_BKP = (1U << RTC_BKP10R_BKP_POS),   // BKP
} rtc_bkp10r_e;

typedef enum : uint32_t {
    RTC_BKP10R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp10r_mask_e;

typedef enum : uint32_t {
    RTC_BKP11R_BKP_POS = 0U,   // BKP
} rtc_bkp11r_pos_e;

typedef enum : uint32_t {
    RTC_BKP11R_BKP = (1U << RTC_BKP11R_BKP_POS),   // BKP
} rtc_bkp11r_e;

typedef enum : uint32_t {
    RTC_BKP11R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp11r_mask_e;

typedef enum : uint32_t {
    RTC_BKP12R_BKP_POS = 0U,   // BKP
} rtc_bkp12r_pos_e;

typedef enum : uint32_t {
    RTC_BKP12R_BKP = (1U << RTC_BKP12R_BKP_POS),   // BKP
} rtc_bkp12r_e;

typedef enum : uint32_t {
    RTC_BKP12R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp12r_mask_e;

typedef enum : uint32_t {
    RTC_BKP13R_BKP_POS = 0U,   // BKP
} rtc_bkp13r_pos_e;

typedef enum : uint32_t {
    RTC_BKP13R_BKP = (1U << RTC_BKP13R_BKP_POS),   // BKP
} rtc_bkp13r_e;

typedef enum : uint32_t {
    RTC_BKP13R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp13r_mask_e;

typedef enum : uint32_t {
    RTC_BKP14R_BKP_POS = 0U,   // BKP
} rtc_bkp14r_pos_e;

typedef enum : uint32_t {
    RTC_BKP14R_BKP = (1U << RTC_BKP14R_BKP_POS),   // BKP
} rtc_bkp14r_e;

typedef enum : uint32_t {
    RTC_BKP14R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp14r_mask_e;

typedef enum : uint32_t {
    RTC_BKP15R_BKP_POS = 0U,   // BKP
} rtc_bkp15r_pos_e;

typedef enum : uint32_t {
    RTC_BKP15R_BKP = (1U << RTC_BKP15R_BKP_POS),   // BKP
} rtc_bkp15r_e;

typedef enum : uint32_t {
    RTC_BKP15R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp15r_mask_e;

typedef enum : uint32_t {
    RTC_BKP16R_BKP_POS = 0U,   // BKP
} rtc_bkp16r_pos_e;

typedef enum : uint32_t {
    RTC_BKP16R_BKP = (1U << RTC_BKP16R_BKP_POS),   // BKP
} rtc_bkp16r_e;

typedef enum : uint32_t {
    RTC_BKP16R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp16r_mask_e;

typedef enum : uint32_t {
    RTC_BKP17R_BKP_POS = 0U,   // BKP
} rtc_bkp17r_pos_e;

typedef enum : uint32_t {
    RTC_BKP17R_BKP = (1U << RTC_BKP17R_BKP_POS),   // BKP
} rtc_bkp17r_e;

typedef enum : uint32_t {
    RTC_BKP17R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp17r_mask_e;

typedef enum : uint32_t {
    RTC_BKP18R_BKP_POS = 0U,   // BKP
} rtc_bkp18r_pos_e;

typedef enum : uint32_t {
    RTC_BKP18R_BKP = (1U << RTC_BKP18R_BKP_POS),   // BKP
} rtc_bkp18r_e;

typedef enum : uint32_t {
    RTC_BKP18R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp18r_mask_e;

typedef enum : uint32_t {
    RTC_BKP19R_BKP_POS = 0U,   // BKP
} rtc_bkp19r_pos_e;

typedef enum : uint32_t {
    RTC_BKP19R_BKP = (1U << RTC_BKP19R_BKP_POS),   // BKP
} rtc_bkp19r_e;

typedef enum : uint32_t {
    RTC_BKP19R_BKP_MASK = 0b11111111111111111111111111111111U,   // 32 bit(s)
} rtc_bkp19r_mask_e;

// clang-format on

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

typedef __vo struct {
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

#define RTC ((rtc_reg_def *)RTC_BASE_ADDR)

//======================================================================================//}
//                  Function API Prototypes
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

void rtc_it_config(rtc_it_options_e it_opt, togglable_e toggle);

void rtc_timestamp_init(rtc_timestamp_config timestamp_conf);
rtc_timestamp_status_e rtc_timestamp_read_status(void);
void rtc_timestamp_clear_status(void);
rtc_date_data rtc_timestamp_read_date(void);
rtc_time_data rtc_timestamp_read_time(void);

void rtc_wakeup_init(rtc_wakeup_config wakeup_conf);
rtc_wakeup_status_e rtc_wakeup_get_status(void);
void rtc_wakeup_clear_status(void);

void rtc_it_handler(void);

#endif
