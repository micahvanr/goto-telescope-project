//======================================================================================//{
//  File: rtc.c
//  Overview: Provides functions for initializing, configuring, and using the RTC peripheral.
//  Features:
//      General time keeping
//      Alarms (w/ interrupt capability)
//      Timestamps (w/ interrupt capability)
//      Wake-up (w/ interrupt capability)
//  Notes: None
//======================================================================================//}

//} Include Statements
//=========================================//{
#include "rtc.h"
#include "assert_handler.h"
#include "exti.h"
#include "nvic.h"
#include "pwr.h"
#include "rcc.h"
#include "stm32f4xx.h"
#include <stdint.h>

//======================================================================================//}
//                  Private Constants
//======================================================================================//{

//========================================================//}
//          Register Constants
//========================================================//{

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
//                  Private Function Prototypes
//======================================================================================//{

static void set_rtc_clock(rtc_config rtc_conf);
static void set_rtc_time(rtc_time_data time_data);
static void set_rtc_date(rtc_date_data date_data);

static void assert_rtc_config_param(rtc_config rtc_conf);
static void assert_time_data_param(rtc_time_data time_data);
static void assert_date_data_param(rtc_date_data time_data);

static inline uint8_t convert_bcd_to_dec(uint8_t tens, uint8_t units);
static inline void unlock_rtc(void);
static void set_rtc_pre(void);
static void enable_alarm(rtc_alarm_sel_e alarm_sel, togglable_e it_toggle);
static void disable_alarm(rtc_alarm_sel_e alarm_sel);

static inline void assert_alarm_sel_param(rtc_alarm_sel_e alarm_sel);
static inline void assert_it_opt_param(rtc_it_options_e it_option);

static inline void assert_seconds_param(uint8_t seconds);
static inline void assert_minutes_param(uint8_t minutes);
static inline void assert_hours_param(uint8_t hours);
static inline void assert_am_pm_param(rtc_time_am_pm_e am_pm);
static inline void assert_time_format_param(rtc_time_format_e hours);

static inline void assert_date_param(uint8_t date);
static inline void assert_month_param(uint8_t month);
static inline void assert_year_param(uint8_t year);
static inline void assert_weekday_param(rtc_weekdays_e weekday);

static inline void assert_wakeup_clk_sel_param(rtc_wakeup_clk_sel_e clk_sel);

//======================================================================================//}
//                  Public Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: rtc_init
//  Overview: Initialize the RTC peripheral according to settings in handle.
//  Parameters:
//      p_rtc_handle: Structure with the corresponding settings to configure the RTC pin.
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_init(rtc_handler *const p_rtc_handler)
{
    uint8_t const UPDATE_ALLOWED = (1 << 6);

    // NOTE: PWR has to be disable backup write protection in order to write to certain parts of RCC
    rcc_pwr_togglable_clock(ENABLE);
    pwr_disable_backup_write_prot();

    // RCC Config
    // Enable clock to RTC
    rcc_rtc_togglable_clock(ENABLE);

    set_rtc_clock(p_rtc_handler->rtc_conf);

    // NOTE: RTC is locked be default, must be unlocked first to change most registers
    unlock_rtc();

    RTC->ISR |= RTC_ISR_INIT;
    while ((RTC->ISR & RTC_ISR_INITF) != UPDATE_ALLOWED);

    set_rtc_pre();

    set_rtc_time(p_rtc_handler->time_data);
    set_rtc_date(p_rtc_handler->date_data);

    RTC->ISR &= ~RTC_ISR_INIT;
}

//============================================================================//{
//  Function: rtc_get_time_format
//  Overview: Get the time format (AM/PM).
//  Parameters:
//      None
//  Return:
//      rtc_time_format_e: Time format of current time.
//          RTC_TIME_FORMAT_AM_24HR
//          RTC_TIME_FORMAT_PM_12HR
//  Note: None
//============================================================================//}
rtc_time_am_pm_e rtc_get_time_format(void)
{
    return (RTC->TR >> RTC_TR_PM_POS) & RTC_TR_PM_MASK;
}

//============================================================================//{
//  Function: rtc_get_hours
//  Overview: Get the RTC hours.
//  Parameters:
//      None
//  Return:
//      RTC hours
//  Note: None
//============================================================================//}
uint8_t rtc_get_hours(void)
{
    return convert_bcd_to_dec((RTC->TR >> RTC_TR_HT_POS) & RTC_TR_HT_MASK, (RTC->TR >> RTC_TR_HU_POS) & RTC_TR_HU_MASK);
}

//============================================================================//{
//  Function: rtc_get_minutes
//  Overview: Get the RTC minutes.
//  Parameters:
//      None
//  Return:
//      RTC minutes
//  Note: None
//============================================================================//{
uint8_t rtc_get_minutes(void)
{
    return convert_bcd_to_dec((RTC->TR >> RTC_TR_MNT_POS) & RTC_TR_MNT_MASK,
                              (RTC->TR >> RTC_TR_MNU_POS) & RTC_TR_MNU_MASK);
}

//============================================================================//{
//  Function: rtc_get_seconds
//  Overview: Get the RTC seconds.
//  Parameters:
//      None
//  Return:
//      RTC seconds
//  Note: None
//============================================================================//}
uint8_t rtc_get_seconds(void)
{
    return convert_bcd_to_dec((RTC->TR >> RTC_TR_ST_POS) & RTC_TR_ST_MASK, (RTC->TR >> RTC_TR_SU_POS) & RTC_TR_SU_MASK);
}

//============================================================================//{
//  Function: rtc_get_time_data
//  Overview: Get the RTC time and returns a structure.
//  Parameters:
//      None
//  Return:
//      rtc_time_data: Structure with RTC time data.
//  Note: None
//============================================================================//}
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

//============================================================================//{
//  Function: rtc_get_weekday
//  Overview: Get the RTC weekday.
//  Parameters:
//      None
//  Return:
//      RTC weekday
//  Note: None
//============================================================================//}
rtc_weekdays_e rtc_get_weekday(void)
{
    return (RTC->DR >> RTC_DR_WDU_POS) & RTC_DR_WDU_MASK;
}

//============================================================================//{
//  Function: rtc_get_year
//  Overview: Get the RTC year.
//  Parameters:
//      None
//  Return:
//      RTC year
//  Note: None
//============================================================================//}
uint8_t rtc_get_year(void)
{
    return convert_bcd_to_dec((RTC->DR >> RTC_DR_YT_POS) & RTC_DR_YT_MASK, (RTC->DR >> RTC_DR_YU_POS) & RTC_DR_YU_MASK);
}

//============================================================================//{
//  Function: rtc_get_month
//  Overview: Get the RTC month.
//  Parameters:
//      None
//  Return:
//      RTC month
//  Note: None
//============================================================================//}
uint8_t rtc_get_month(void)
{
    return convert_bcd_to_dec((RTC->DR >> RTC_DR_MT_POS) & RTC_DR_MT_MASK, (RTC->DR >> RTC_DR_MU_POS) & RTC_DR_MU_MASK);
}

//============================================================================//{
//  Function: rtc_get_date
//  Overview: Get the RTC date.
//  Parameters:
//      None
//  Return:
//      RTC date
//  Note: None
//============================================================================//}
uint8_t rtc_get_date(void)
{
    return convert_bcd_to_dec((RTC->DR >> RTC_DR_DT_POS) & RTC_DR_DT_MASK, (RTC->DR >> RTC_DR_DU_POS) & RTC_DR_DU_MASK);
}

//============================================================================//{
//  Function: rtc_get_date_data
//  Overview: Get the RTC date and returns a structure.
//  Parameters:
//      None
//  Return:
//      rtc_date_data: Structure with RTC date data.
//  Note: None
//============================================================================//}
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

//============================================================================//{
//  Function: rtc_alarm_set
//  Overview: Set the alarm according to settings in config structure.
//  Parameters:
//      alarm_conf: Structure with the corresponding settings to configure the alarm.
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_alarm_set(rtc_alarm_config const alarm_conf)
{
    // Initialize to 0 and don't bother resetting each field as all bits will be set.
    rtc_alarm_sel_e const alarm_sel = alarm_conf.alarm_sel;

    assert_alarm_sel_param(alarm_sel);
    assert_time_data_param(alarm_conf.time_data);
    assert_date_data_param(alarm_conf.date_data);

    disable_alarm(alarm_sel);

    switch (alarm_conf.weekday_sel)
    {
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

    rtc_toggable_it(RTC_IT_OPT_ALARM, alarm_conf.it_toggle);
}

//============================================================================//{
//  Function: rtc_alarm_set_seconds
//  Overview: Set the RTC seconds.
//  Parameters:
//      alarm_sel:
//          RTC_ALARM_A_SEL
//          RTC_ALARM_B_SEL
//      seconds: time to count to
//      seconds_match:
//          RTC_ALARM_SECONDS_MATCH
//          RTC_ALARM_SECONDS_DONT_CARE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_alarm_set_seconds(rtc_alarm_sel_e const alarm_sel, uint8_t const seconds,
                           rtc_alarm_seconds_match_e const seconds_match)
{
    assert_alarm_sel_param(alarm_sel);
    assert_seconds_param(seconds);

    // NOTE: If the seconds field is set, the synchonous prescaler in RTC_PRER might need to be at least 3. See ref manual pg 805

    uint32_t temp_alrmxr = 0;

    temp_alrmxr |= seconds_match << RTC_ALRMAR_MSK1_POS;
    temp_alrmxr |= (seconds / 10) << RTC_ALRMAR_ST_POS;
    temp_alrmxr |= (seconds % 10) << RTC_ALRMAR_SU_POS;

    switch (alarm_sel)
    {
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

//============================================================================//{
//  Function: rtc_alarm_set_minutes
//  Overview: Set the RTC minutes.
//  Parameters:
//      alarm_sel:
//          RTC_ALARM_A_SEL
//          RTC_ALARM_B_SEL
//      minutes: time to count to
//      minutes_match:
//          RTC_ALARM_MINUTES_MATCH
//          RTC_ALARM_MINUTES_DONT_CARE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_alarm_set_minutes(rtc_alarm_sel_e const alarm_sel, uint8_t const minutes,
                           rtc_alarm_minutes_match_e const minutes_match)
{
    assert_alarm_sel_param(alarm_sel);
    assert_minutes_param(minutes);

    uint32_t temp_alrmxr = 0;

    temp_alrmxr |= minutes_match << RTC_ALRMAR_MSK2_POS;
    temp_alrmxr |= (minutes / 10) << RTC_ALRMAR_MNT_POS;
    temp_alrmxr |= (minutes % 10) << RTC_ALRMAR_MNU_POS;

    switch (alarm_sel)
    {
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

//============================================================================//{
//  Function: rtc_alarm_set_hours
//  Overview: Set the RTC hours.
//  Parameters:
//      alarm_sel:
//          RTC_ALARM_A_SEL
//          RTC_ALARM_B_SEL
//      hours: time to count to
//      minutes_match:
//          RTC_ALARM_HOURS_MATCH
//          RTC_ALARM_HOURS_DONT_CARE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_alarm_set_hours(rtc_alarm_sel_e const alarm_sel, uint8_t const hours,
                         rtc_alarm_hours_match_e const hours_match)
{
    assert_alarm_sel_param(alarm_sel);
    assert_hours_param(hours);

    uint32_t temp_alrmxr = 0;

    temp_alrmxr |= hours_match << RTC_ALRMAR_MSK3_POS;
    temp_alrmxr |= (hours / 10) << RTC_ALRMAR_HT_POS;
    temp_alrmxr |= (hours % 10) << RTC_ALRMAR_HU_POS;

    switch (alarm_sel)
    {
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

//============================================================================//{
//  Function: rtc_alarm_set_date
//  Overview: Set the RTC date.
//  Parameters:
//      alarm_sel:
//          RTC_ALARM_A_SEL
//          RTC_ALARM_B_SEL
//      hours: time to count to
//      date_match:
//          RTC_ALARM_DATEDAY_MATCH
//          RTC_ALARM_DATEDAY_DONT_CARE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_alarm_set_date(rtc_alarm_sel_e const alarm_sel, uint8_t const date, rtc_alarm_dateday_match_e const date_match)
{
    assert_alarm_sel_param(alarm_sel);
    assert_date_param(date);

    uint32_t temp_alrmxr = 0;
    // Date/day alarm
    temp_alrmxr |= date_match << RTC_ALRMAR_MSK4_POS;
    temp_alrmxr |= RTC_ALARM_WEEKDAY_SEL_DATE << RTC_ALRMAR_WDSEL_POS;
    temp_alrmxr |= (date / 10) << RTC_ALRMAR_DT_POS;
    temp_alrmxr |= (date % 10) << RTC_ALRMAR_DU_POS;

    // Clear RTC date
    switch (alarm_sel)
    {
        case RTC_ALARM_A_SEL:
            RTC->ALRMAR &=
                ~((RTC_ALRMAR_MSK4_MASK << RTC_ALRMAR_MSK4_POS) | (RTC_ALRMAR_WDSEL_MASK << RTC_ALRMAR_WDSEL_POS)
                  | (RTC_ALRMAR_DT_MASK << RTC_ALRMAR_DT_POS) | (RTC_ALRMAR_DU_MASK << RTC_ALRMAR_DU_POS));
            RTC->ALRMAR |= temp_alrmxr;
            break;

        case RTC_ALARM_B_SEL:
            RTC->ALRMBR &=
                ~((RTC_ALRMAR_MSK4_MASK << RTC_ALRMAR_MSK4_POS) | (RTC_ALRMAR_WDSEL_MASK << RTC_ALRMAR_WDSEL_POS)
                  | (RTC_ALRMAR_DT_MASK << RTC_ALRMAR_DT_POS) | (RTC_ALRMAR_DU_MASK << RTC_ALRMAR_DU_POS));
            RTC->ALRMBR |= temp_alrmxr;
            break;
    }
}

//============================================================================//{
//  Function: rtc_alarm_set_day
//  Overview: Set the RTC day.
//  Parameters:
//      alarm_sel:
//          RTC_ALARM_A_SEL
//          RTC_ALARM_B_SEL
//      weekday:
//          RTC_WEEKDAY_MONDAY
//          RTC_WEEKDAY_TUESDAY
//          RTC_WEEKDAY_WEDNESDAY
//          RTC_WEEKDAY_THURSDAY
//          RTC_WEEKDAY_FRIDAY
//          RTC_WEEKDAY_SATURDAY
//          RTC_WEEKDAY_SUNDAY
//      date_match:
//          RTC_ALARM_DATEDAY_MATCH
//          RTC_ALARM_DATEDAY_DONT_CARE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_alarm_set_day(rtc_alarm_sel_e const alarm_sel, rtc_weekdays_e const weekday,
                       rtc_alarm_dateday_match_e const date_match)
{
    assert_alarm_sel_param(alarm_sel);
    assert_weekday_param(weekday);

    uint32_t temp_alrmxr = 0;
    temp_alrmxr |= date_match << RTC_ALRMAR_MSK4_POS;
    temp_alrmxr |= RTC_ALARM_WEEKDAY_SEL_WEEKDAY << RTC_ALRMAR_WDSEL_POS;
    temp_alrmxr |= weekday << RTC_ALRMAR_DU_POS;

    // Clear RTC date
    switch (alarm_sel)
    {
        case RTC_ALARM_A_SEL:
            RTC->ALRMAR &=
                ~((RTC_ALRMAR_MSK4_MASK << RTC_ALRMAR_MSK4_POS) | (RTC_ALRMAR_WDSEL_MASK << RTC_ALRMAR_WDSEL_POS)
                  | (RTC_ALRMAR_DT_MASK << RTC_ALRMAR_DT_POS) | (RTC_ALRMAR_DU_MASK << RTC_ALRMAR_DU_POS));
            RTC->ALRMAR |= temp_alrmxr;
            break;

        case RTC_ALARM_B_SEL:
            RTC->ALRMBR &=
                ~((RTC_ALRMAR_MSK4_MASK << RTC_ALRMAR_MSK4_POS) | (RTC_ALRMAR_WDSEL_MASK << RTC_ALRMAR_WDSEL_POS)
                  | (RTC_ALRMAR_DT_MASK << RTC_ALRMAR_DT_POS) | (RTC_ALRMAR_DU_MASK << RTC_ALRMAR_DU_POS));
            RTC->ALRMBR |= temp_alrmxr;
            break;
    }
}

//============================================================================//{
//  Function: rtc_alarm_set_am_pm
//  Overview: Set the RTC AM or PM.
//  Parameters:
//      alarm_sel:
//          RTC_ALARM_A_SEL
//          RTC_ALARM_B_SEL
//      am_pm:
//          RTC_TIME_AM
//          RTC_TIME_PM
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_alarm_set_am_pm(rtc_alarm_sel_e const alarm_sel, uint8_t const am_pm)
{
    assert_alarm_sel_param(alarm_sel);
    assert_am_pm_param(am_pm);

    switch (alarm_sel)
    {
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

//============================================================================//{
//  Function: rtc_alarm_is_triggered
//  Overview: Get the specified alarm's status.
//  Parameters:
//      alarm_sel:
//          RTC_ALARM_A_SEL
//          RTC_ALARM_B_SEL
//  Return:
//      rtc_alarm_status_e:
//          RTC_ALARM_STATUS_TRIGGERED
//          RTC_ALARM_STATUS_MATCH
//  Note: None
//============================================================================//}
bool rtc_alarm_is_triggered(rtc_alarm_sel_e const alarm_sel)
{
    assert_alarm_sel_param(alarm_sel);

    bool b_is_alarm_triggered = false;
    switch (alarm_sel)
    {
        case RTC_ALARM_A_SEL:
            b_is_alarm_triggered = 0b1 & ((RTC->ISR >> RTC_ISR_ALRAF_POS) & RTC_ISR_ALRAF_MASK);
            break;
        case RTC_ALARM_B_SEL:
            b_is_alarm_triggered = 0b1 & ((RTC->ISR >> RTC_ISR_ALRBF_POS) & RTC_ISR_ALRBF_MASK);
            break;
    }
    return b_is_alarm_triggered;
}

//============================================================================//{
//  Function: rtc_toggable_it
//  Overview: Enable or disable the specified RTC related interrupt.
//  Parameters:
//      it_opt: RTC interrupt options.
//          RTC_IT_OPT_ALARM
//          RTC_IT_OPT_WAKE_UP
//          RTC_IT_OPT_TIMESTAMP
//          RTC_IT_OPT_TAMPER
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_toggable_it(rtc_it_options_e it_opt, togglable_e toggle)
{
    assert_it_opt_param(it_opt);

    switch (it_opt)
    {
        case RTC_IT_OPT_ALARM:
            exti_toggable_it((exti_lines_e) RTC_EXTI_NUM_ALARM, toggle);
            exti_toggable_rising_edge((exti_lines_e) RTC_EXTI_NUM_ALARM, toggle);
            nvic_toggable_irq(RTC_ALARM_IRQ_NUM_41, toggle);
            break;

        case RTC_IT_OPT_WAKEUP:
            exti_toggable_it((exti_lines_e) RTC_EXTI_NUM_WAKE_UP, toggle);
            exti_toggable_rising_edge((exti_lines_e) RTC_EXTI_NUM_WAKE_UP, toggle);
            nvic_toggable_irq(RTC_WKUP_IRQ_NUM_3, toggle);
            break;

        case RTC_IT_OPT_TIMESTAMP:
            exti_toggable_it((exti_lines_e) RTC_EXTI_NUM_TIMESTAMP, toggle);
            exti_toggable_rising_edge((exti_lines_e) RTC_EXTI_NUM_TIMESTAMP, toggle);
            nvic_toggable_irq(TAMP_STAMP_IRQ_NUM_2, toggle);
            break;

        case RTC_IT_OPT_NA:
            break;
        case RTC_IT_OPT_TAMPER:
            ASSERT(false);
            break;
    }
}

//============================================================================//{
//  Function: rtc_timestamp_init
//  Overview: Initialize the timestamp according to settings in config structure.
//  Parameters:
//      timestamp_conf: Structure with the corresponding settings to configure the timestamp.
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_timestamp_init(rtc_timestamp_config timestamp_conf)
{
    RTC->CR |= RTC_CR_TSE;

    if (RTC_IT_OPT_TIMESTAMP == timestamp_conf.it_opt)
    {
        RTC->CR |= RTC_CR_TSIE;
        rtc_toggable_it(timestamp_conf.it_opt, ENABLE);
    }

    // Map TIMESTAMP to AF
    RTC->TAFCR &= ~(RTC_TAFCR_TSINSEL_MASK << RTC_TAFCR_TSINSEL_POS);
    RTC->TAFCR |= timestamp_conf.alt_fn_sel << RTC_TAFCR_TSINSEL_POS;

    RTC->CR &= ~(RTC_CR_TSEDGE_MASK << RTC_CR_TSEDGE_POS);
    RTC->CR |= timestamp_conf.timestamp_edge << RTC_CR_TSEDGE_POS;
}

//============================================================================//{
//  Function: rtc_timestamp_read_status
//  Overview: Get the RTC timestamp status. Whether a timestamp has occured.
//  Parameters:
//      None
//  Return:
//      rtc_timestamp_status_e:
//          RTC_TIMESTAMP_STATUS_NOT_TRIGGERED
//          RTC_TIMESTAMP_STATUS_TRIGGERED
//  Note: None
//============================================================================//}
rtc_timestamp_status_e rtc_timestamp_read_status(void)
{
    return RTC_ISR_TSF_MASK & (RTC->ISR >> RTC_ISR_TSF_POS);
}

//============================================================================//{
//  Function: rtc_timestamp_clear_status
//  Overview: Clear the RTC timestamp status.
//  Parameters:
//      None
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_timestamp_clear_status(void)
{
    RTC->ISR &= ~(RTC_ISR_TSF);
    if (RTC->ISR & RTC_ISR_TSOVF)
    {
        RTC->ISR &= ~(RTC_ISR_TSOVF);
    }
}

//============================================================================//{
//  Function: rtc_timestamp_read_date
//  Overview: Get the timestamp date information.
//  Parameters:
//      None
//  Return:
//      rtc_date_data: Structure of date info. Including the date, month, year, and weekday.
//  Note: None
//============================================================================//}
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

//============================================================================//{
//  Function: rtc_timestamp_read_time
//  Overview: Get the timestamp time information.
//  Parameters:
//      None
//  Return:
//      rtc_time_data: Structure of time info. Including the seconds, minutes, hours, and whether it is am/pm.
//  Note: The time format is not directly returned. It can be derived from the am/pm data.
//============================================================================//}
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

//============================================================================//{
//  Function: rtc_wakeup_init
//  Overview: Initialize the wakeup according to settings in config structure.
//  Parameters:
//      wakeup_conf: Structure with the corresponding settings to configure the wakeup.
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_wakeup_init(rtc_wakeup_config wakeup_conf)
{
    assert_wakeup_clk_sel_param(wakeup_conf.clk_sel);
    assert_it_opt_param(wakeup_conf.it_opt);

    RTC->CR &= ~RTC_CR_WUTE;

    // Wait until updates allowed.
    while (!(RTC->ISR & RTC_ISR_WUTWF));

    RTC->CR &= ~(RTC_CR_WCKSEL_MASK << RTC_CR_WCKSEL_POS);
    RTC->CR |= wakeup_conf.clk_sel << RTC_CR_WCKSEL_POS;

    RTC->WUTR &= ~RTC_WUTR_WUT_MASK;
    RTC->WUTR |= wakeup_conf.auto_reload_value;

    if (RTC_IT_OPT_WAKEUP == wakeup_conf.it_opt)
    {
        RTC->CR |= RTC_CR_WUTIE;
        rtc_toggable_it(wakeup_conf.it_opt, ENABLE);
    }

    rtc_wakeup_clear_status();

    RTC->CR |= RTC_CR_WUTE;
}

//============================================================================//{
//  Function: rtc_wakeup_is_triggered
//  Overview: Read the status of the wakeup.
//  Parameters:
//      None
//  Return:
//      true
//      false
//  Note: None
//============================================================================//}
bool rtc_wakeup_is_triggered(void)
{
    return ((RTC->ISR >> RTC_ISR_WUTF_POS) & RTC_ISR_WUTF_MASK);
}

//============================================================================//{
//  Function: rtc_wakeup_clear_status
//  Overview: Clear the status of the wakeup section of RTC.
//  Parameters:
//      None
//  Return:
//      None
//  Note: None
//============================================================================//}
void rtc_wakeup_clear_status(void)
{
    RTC->ISR &= ~(RTC_ISR_WUTF_MASK << RTC_ISR_WUTF_POS);
}

//============================================================================//{
//  Function: rtc_it_handler
//  Overview: Handle interrupt for RTC peripheral.
//  Parameters:
//      None
//  Return:
//      None
//  Note: This function should be called by the applications IT handler.
//============================================================================//}
void rtc_it_handler(void)
{
    uint32_t temp_isr = RTC->ISR;

    if (temp_isr & RTC_ISR_ALRAF)
    {
        RTC->ISR &= ~RTC_ISR_ALRAF;
        exti_clear_pending((exti_lines_e) RTC_EXTI_NUM_ALARM);
    }

    if (temp_isr & RTC_ISR_ALRBF)
    {
        RTC->ISR &= ~RTC_ISR_ALRBF;
        exti_clear_pending((exti_lines_e) RTC_EXTI_NUM_ALARM);
    }

    if (temp_isr & RTC_ISR_WUTF)
    {
        RTC->ISR &= ~RTC_ISR_WUTF;
        exti_clear_pending((exti_lines_e) RTC_EXTI_NUM_WAKE_UP);
    }

    if (temp_isr & RTC_ISR_TSF)
    {
        RTC->ISR &= ~RTC_ISR_TSF;
        rtc_timestamp_clear_status();
        exti_clear_pending((exti_lines_e) RTC_EXTI_NUM_TIMESTAMP);
    }
}

//======================================================================================//}
//                  Private Function Implementation
//======================================================================================//{

static void set_rtc_clock(rtc_config const rtc_conf)
{
    assert_rtc_config_param(rtc_conf);

    rcc_set_rtc_clk_src(rtc_conf.rtc_sel);
    if (RCC_RTC_CLK_SRC_LSI == rtc_conf.rtc_sel)
    {
        rcc_lsi_config(ENABLE);
        while (!rcc_lsi_is_rdy());
    }
    if (RCC_RTC_CLK_SRC_HSE == rtc_conf.rtc_sel)
    {
        rcc_set_rtc_pre(rtc_conf.rtc_pre);
    }
}

static void assert_rtc_config_param(rtc_config const rtc_conf)
{
    // clang-format off
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
    // clang-format on
}

static void set_rtc_time(rtc_time_data const time_data)
{
    uint32_t temp_tr = RTC->TR;
    uint32_t temp_cr = RTC->CR;

    assert_time_data_param(time_data);

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

static void assert_time_data_param(rtc_time_data const time_data)
{
    assert_time_format_param(time_data.time_format);
    assert_am_pm_param(time_data.time_am_pm);
    assert_hours_param(time_data.hours);
    assert_minutes_param(time_data.minutes);
    assert_seconds_param(time_data.seconds);
}

static void set_rtc_date(rtc_date_data const date_data)
{
    uint32_t temp_dr = RTC->DR;

    assert_date_data_param(date_data);

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

static void assert_date_data_param(rtc_date_data const date_data)
{
    assert_weekday_param(date_data.weekday);
    assert_date_param(date_data.date);
    assert_month_param(date_data.month);
    assert_year_param(date_data.year);
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

    rtc_clk = rcc_get_rtc_clock_freq_hz();

    asynch_pre = 128; // Highest 7-bit number
    // If dividing RTC input clock results in remainder, then reduce prescaler by factor of 2.
    while (rtc_clk % asynch_pre)
    {
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
    switch (alarm_sel)
    {
        case RTC_ALARM_A_SEL:
            RTC->CR |= RTC_CR_ALRAE;
            if (ENABLE == it_toggle)
            {
                RTC->CR |= RTC_CR_ALRAIE;
            }
            break;
        case RTC_ALARM_B_SEL:
            RTC->CR |= RTC_CR_ALRBE;
            if (ENABLE == it_toggle)
            {
                RTC->CR |= RTC_CR_ALRBIE;
            }
            break;
    }
}

static void disable_alarm(rtc_alarm_sel_e const alarm_sel)
{
    // Turn off interrupts so we can modify the alarm.
    switch (alarm_sel)
    {
        case RTC_ALARM_A_SEL:
            // Wait till alarm update is allowed.
            while (!(RTC->ISR & RTC_ISR_ALRAWF));
            RTC->CR &= ~RTC_CR_ALRAE;
            RTC->CR &= ~RTC_CR_ALRAIE;
            break;

        case RTC_ALARM_B_SEL:
            // Wait till alarm update is allowed.
            while (!(RTC->ISR & RTC_ISR_ALRBWF));
            RTC->CR &= ~RTC_CR_ALRBE;
            RTC->CR &= ~RTC_CR_ALRBIE;
            break;
    }
}

static inline void assert_alarm_sel_param(rtc_alarm_sel_e alarm_sel)
{
    ASSERT((RTC_ALARM_A_SEL == alarm_sel) || (RTC_ALARM_A_SEL == alarm_sel));
}

static inline void assert_it_opt_param(rtc_it_options_e it_option)
{
    ASSERT((RTC_IT_OPT_NA == it_option) || (RTC_IT_OPT_NA == it_option) || (RTC_IT_OPT_ALARM == it_option)
           || (RTC_IT_OPT_WAKEUP == it_option) || (RTC_IT_OPT_TIMESTAMP == it_option)
           || (RTC_IT_OPT_TAMPER == it_option));
}

static inline void assert_seconds_param(uint8_t const seconds)
{
    ASSERT(seconds <= 59);
}

static inline void assert_minutes_param(uint8_t const minutes)
{
    ASSERT(minutes <= 59);
}

static inline void assert_hours_param(uint8_t const hours)
{
    ASSERT(hours <= 23);
}

static inline void assert_am_pm_param(rtc_time_am_pm_e const am_pm)
{
    ASSERT((RTC_TIME_AM == am_pm) || (RTC_TIME_PM == am_pm));
}

static inline void assert_time_format_param(rtc_time_format_e const time_format)
{
    ASSERT((RTC_TIME_FORMAT_12HR == time_format) || (RTC_TIME_FORMAT_24HR == time_format));
}

static inline void assert_date_param(uint8_t const date)
{
    ASSERT(date <= 31);
}

static inline void assert_month_param(uint8_t const month)
{
    ASSERT(month <= 12);
}

static inline void assert_year_param(uint8_t const year)
{
    ASSERT(year <= 99);
}

static inline void assert_weekday_param(rtc_weekdays_e const weekday)
{
    // clang-format off
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
    // clang-format on
}
static inline void assert_wakeup_clk_sel_param(rtc_wakeup_clk_sel_e clk_sel)
{
    ASSERT((clk_sel = RTC_WAKEUP_RTC_CLK_DIV16) || (clk_sel = RTC_WAKEUP_RTC_CLK_DIV8)
           || (clk_sel = RTC_WAKEUP_RTC_CLK_DIV4) || (clk_sel = RTC_WAKEUP_RTC_CLK_DIV2)
           || (clk_sel = RTC_WAKEUP_SPRE_CLK) || (clk_sel = RTC_WAKEUP_SPRE_CLK_EXTRA_BIT));
}

// *End of file*
