#ifndef TIM_H
#define TIM_H

#include "rcc.h"
#include "stm32f4xx.h"

// Basic Timers: TIM6 & TIM7
// General Purpose Timers: TIM2 - TIM5
// General Purpose Timers: TIM9 - TIM14
// Advanced Control Timers: TIM1 & TIM8

//======================================================================================//}
//                  Address Definitions
//======================================================================================//{

typedef enum {
    TIM1_BASE_ADDR = ((APB2_BASE_ADDR) + (0x0000u)),

    TIM2_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0000u)),
    TIM3_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0400u)),
    TIM4_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0800u)),
    TIM5_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0C00u)),
    TIM6_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x1000u)),
    TIM7_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x1400u)),
    TIM12_BASE_ADDR = ((APB1_BASE_ADDR) + (0x1800u)),
    TIM13_BASE_ADDR = ((APB1_BASE_ADDR) + (0x1C00u)),
    TIM14_BASE_ADDR = ((APB1_BASE_ADDR) + (0x2000u)),

    TIM8_BASE_ADDR  = ((APB2_BASE_ADDR) + (0x0400u)),
    TIM9_BASE_ADDR  = ((APB2_BASE_ADDR) + (0x4000u)),
    TIM10_BASE_ADDR = ((APB2_BASE_ADDR) + (0x4400u)),
    TIM11_BASE_ADDR = ((APB2_BASE_ADDR) + (0x4800u)),
} tim_base_addr_e;

//======================================================================================//}
//                  Peripheral Constants
//======================================================================================//{

typedef enum {
    TIM_MAX_NUM_TIMERS = 14,
} tim_num_of_timers_e;

// Init check enum
typedef enum {
    TIM1_INIT_NUM  = 0,
    TIM2_INIT_NUM  = 1,
    TIM3_INIT_NUM  = 2,
    TIM4_INIT_NUM  = 3,
    TIM5_INIT_NUM  = 4,
    TIM6_INIT_NUM  = 5,
    TIM7_INIT_NUM  = 6,
    TIM8_INIT_NUM  = 7,
    TIM9_INIT_NUM  = 8,
    TIM10_INIT_NUM = 9,
    TIM11_INIT_NUM = 10,
    TIM12_INIT_NUM = 11,
    TIM13_INIT_NUM = 12,
    TIM14_INIT_NUM = 13,
} tim_init_peri_num_e;

typedef enum {
    TIM_NOT_INITIALIZED = 0,
    TIM_INITIALIZED     = 1,
} tim_init_check_e;

typedef enum {
    TIM_TYPE_BASIC,
    TIM_TYPE_GENERAL_2_5,
    TIM_TYPE_GENERAL_9_14,
    TIM_TYPE_ADVANCED,
} tim_type_e;

typedef enum {
    TIM_COUNTER_EN,
    TIM_COUNTER_DI,
} tim_counter_toggle_e;

typedef enum {
    TIM_UPDATE_NA    = 0b0,
    TIM_UPDATE_FOUND = 0b1,
} tim_status_e;

typedef enum {
    TIM_UNIT_S,  // Seconds
    TIM_UNIT_MS, // Miliseconds
    TIM_UNIT_US, // Microseconds
    TIM_UNIT_HZ,
    TIM_UNIT_KHZ,
    TIM_UNIT_MHZ,
} tim_unit_of_time_e;

typedef enum {
    TIM_ARR_PRELOAD_DI = 0b0, // Count value will count to this new value immediately
    TIM_ARR_PRELOAD_EN = 0b1, // Count value will count to the new value after finishing what its counting to now
} tim_arr_preload_e;

typedef enum {
    TIM_AUTO_ARR_COUNT_DI = 0b0,
    TIM_AUTO_ARR_COUNT_EN = 0b1,
} tim_auto_arr_count_e;

typedef enum {
    TIM_ONE_PULSE_MODE_DI = 0b0,
    TIM_ONE_PULSE_MODE_EN = 0b1,
} tim_one_pulse_mode_e;

// Numbers used to select register
typedef enum {
    TIM_CHANNEL_SEL_1 = 0,
    TIM_CHANNEL_SEL_2 = 1,
    TIM_CHANNEL_SEL_3 = 2,
    TIM_CHANNEL_SEL_4 = 3,
    TIM_CHANNEL_NA,
} tim_channel_sel_e;

typedef enum {
    TIM_CLK_SEL_INTERNAL         = 0b000,
    TIM_CLK_SEL_INTERNAL_TRIGGER = 0b110,
    TIM_CLK_SEL_EXTERNAL         = 0b111,
} tim_clock_sel_e;

typedef enum {
    TIM_TRIGGER_SEL_ITR0    = 0b000,
    TIM_TRIGGER_SEL_ITR1    = 0b001,
    TIM_TRIGGER_SEL_ITR2    = 0b010,
    TIM_TRIGGER_SEL_ITR3    = 0b011,
    TIM_TRIGGER_SEL_TI1F_ED = 0b100,
    TIM_TRIGGER_SEL_TI1FP1  = 0b101,
    TIM_TRIGGER_SEL_TI1FP2  = 0b110,
    TIM_TRIGGER_SEL_ETRF    = 0b111,
} tim_trigger_sel_e;

typedef enum {
    TIM_CNT_DIR_UP   = 0b0,
    TIM_CNT_DIR_DOWN = 0b1,
} tim_cnt_dir_e;

typedef enum {
    TIM_ALIGN_EDGE              = 0b00,
    TIM_ALIGN_CENTER_IT_DOWN    = 0b01,
    TIM_ALIGN_CENTER_IT_UP      = 0b10,
    TIM_ALIGN_CENTER_IT_DOWN_UP = 0b11,
} tim_align_mode_e;

// DTS = dead-time and sampling clock (derived from CR1->CKD)
typedef enum {
    TIM_DTS_FREQ_CLK_INT        = 0b00,
    TIM_DTS_FREQ_CLK_INT_MULT_2 = 0b01,
    TIM_DTS_FREQ_CLK_INT_MULT_4 = 0b10,
} tim_dts_mult_e;

typedef enum {
    TIM_INPUT_MAP_MAIN         = 0b01, // Input will match the channel number
    TIM_INPUT_MAP_TI_ALTERNATE = 0b10, // Input will be the alternate channel number (either +- 1)
    TIM_INPUT_MAP_TRC          = 0b11,
} tim_input_map_type_e;

typedef enum {
    TIM_IC_PRESCALER_NA   = 0b00,
    TIM_IC_PRESCALER_2_EV = 0b01,
    TIM_IC_PRESCALER_4_EV = 0b10,
    TIM_IC_PRESCALER_8_EV = 0b11,
} tim_ic_prescaler_e;

typedef enum {
    TIM_IC_FILTER_NA            = 0b0000,
    TIM_IC_FILTER_FCLKINT_N2    = 0b0001,
    TIM_IC_FILTER_FCLKINT_N4    = 0b0010,
    TIM_IC_FILTER_FCLKINT_N8    = 0b0011,
    TIM_IC_FILTER_FDTS_DIV2_N6  = 0b0100,
    TIM_IC_FILTER_FDTS_DIV2_N8  = 0b0101,
    TIM_IC_FILTER_FDTS_DIV4_N6  = 0b0110,
    TIM_IC_FILTER_FDTS_DIV4_N8  = 0b0111,
    TIM_IC_FILTER_FDTS_DIV8_N6  = 0b1000,
    TIM_IC_FILTER_FDTS_DIV8_N8  = 0b1001,
    TIM_IC_FILTER_FDTS_DIV16_N5 = 0b1010,
    TIM_IC_FILTER_FDTS_DIV16_N6 = 0b1011,
    TIM_IC_FILTER_FDTS_DIV16_N8 = 0b1100,
    TIM_IC_FILTER_FDTS_DIV32_N5 = 0b1101,
    TIM_IC_FILTER_FDTS_DIV32_N6 = 0b1110,
    TIM_IC_FILTER_FDTS_DIV32_N8 = 0b1111,
} tim_ic_filter_e;

typedef enum {
    TIM_IC_EDGE_DETECTION_RISING         = 0b00,
    TIM_IC_EDGE_DETECTION_FALLING        = 0b01,
    TIM_IC_EDGE_DETECTION_RISING_FALLING = 0b101, // Special case where the bits are not adjacent to eachother
} tim_ic_edge_detection_e;

// clang-format off
typedef enum {
    TIM_OC_OPM_FROZEN         = 0b000, // Comparison between output compare register and counter has no effect on OCxREF
    TIM_OC_OPM_EQ_ACTIVE      = 0b001, // Sets OCxREF active level on match
    TIM_OC_OPM_EQ_INACTIVE    = 0b010, // Sets OCxREF inactive level on match
    TIM_OC_OPM_TOGGLE         = 0b011, // Toggles OCxREF when there is a match
    TIM_OC_OPM_FORCE_INACTIVE = 0b100, // Forces OCxREF low
    TIM_OC_OPM_FORCE_ACTIVE   = 0b101, // Forces OCxREF high
    TIM_OC_OPM_PWM_1          = 0b110, // Upcounting: OCxREF is active when count < CCR, else it is inactive. Downcounting: Vice versa
    TIM_OC_OPM_PWM_2          = 0b111, // Upcounting: OCxREF is inactive when count < CCR, else it is active. Downcounting: Vice versa
} tim_oc_output_mode_e;
// clang-format on

typedef enum {
    TIM_OC_POL_ACTIVE_HIGH = 0,
    TIM_OC_POL_ACTIVE_LOW  = 1,
} tim_oc_polarity_e;

//======================================================================================//}
//                  Register Constants
//======================================================================================//{

typedef enum {
    TIM_CR1_CKD_POS  = 8, // Clock division
    TIM_CR1_ARPE_POS = 7, // Auto-reload preload enable
    TIM_CR1_CMS_POS  = 5, // Center-aligned mode selection
    TIM_CR1_DIR_POS  = 4, // Direction
    TIM_CR1_OPM_POS  = 3, // One-pulse mode
    TIM_CR1_URS_POS  = 2, // Update request source
    TIM_CR1_UDIS_POS = 1, // Update disable
    TIM_CR1_CEN_POS  = 0, // Counter enable
} tim_cr1_pos_e;

typedef enum {
    TIM_CR1_CKD  = (1 << TIM_CR1_CKD_POS),  // Clock division
    TIM_CR1_ARPE = (1 << TIM_CR1_ARPE_POS), // Auto-reload preload enable
    TIM_CR1_CMS  = (1 << TIM_CR1_CMS_POS),  // Center-aligned mode selection
    TIM_CR1_DIR  = (1 << TIM_CR1_DIR_POS),  // Direction
    TIM_CR1_OPM  = (1 << TIM_CR1_OPM_POS),  // One-pulse mode
    TIM_CR1_URS  = (1 << TIM_CR1_URS_POS),  // Update request source
    TIM_CR1_UDIS = (1 << TIM_CR1_UDIS_POS), // Update disable
    TIM_CR1_CEN  = (1 << TIM_CR1_CEN_POS),  // Counter enable
} tim_cr1_e;

typedef enum {
    TIM_CR1_CKD_MASK  = 0b11, // 2 bit(s)
    TIM_CR1_ARPE_MASK = 0b1,  // 1 bit(s)
    TIM_CR1_CMS_MASK  = 0b11, // 2 bit(s)
    TIM_CR1_DIR_MASK  = 0b1,  // 1 bit(s)
    TIM_CR1_OPM_MASK  = 0b1,  // 1 bit(s)
    TIM_CR1_URS_MASK  = 0b1,  // 1 bit(s)
    TIM_CR1_UDIS_MASK = 0b1,  // 1 bit(s)
    TIM_CR1_CEN_MASK  = 0b1,  // 1 bit(s)
} tim_cr1_mask_e;

typedef enum {
    TIM_CR2_OIS4_POS  = 14, // Output Idle state 4
    TIM_CR2_OIS3N_POS = 13, // Output Idle state 3
    TIM_CR2_OIS3_POS  = 12, // Output Idle state 3
    TIM_CR2_OIS2N_POS = 11, // Output Idle state 2
    TIM_CR2_OIS2_POS  = 10, // Output Idle state 2
    TIM_CR2_OIS1N_POS = 9,  // Output Idle state 1
    TIM_CR2_OIS1_POS  = 8,  // Output Idle state 1
    TIM_CR2_TI1S_POS  = 7,  // TI1 selection
    TIM_CR2_MMS_POS   = 4,  // Master mode selection
    TIM_CR2_CCDS_POS  = 3,  // Capture/compare DMA selection
    TIM_CR2_CCUS_POS  = 2,  // Capture/compare control update selection
    TIM_CR2_CCPC_POS  = 0,  // Capture/compare preloaded control
} tim_cr2_pos_e;

typedef enum {
    TIM_CR2_OIS4  = (1 << TIM_CR2_OIS4_POS),  // Output Idle state 4
    TIM_CR2_OIS3N = (1 << TIM_CR2_OIS3N_POS), // Output Idle state 3
    TIM_CR2_OIS3  = (1 << TIM_CR2_OIS3_POS),  // Output Idle state 3
    TIM_CR2_OIS2N = (1 << TIM_CR2_OIS2N_POS), // Output Idle state 2
    TIM_CR2_OIS2  = (1 << TIM_CR2_OIS2_POS),  // Output Idle state 2
    TIM_CR2_OIS1N = (1 << TIM_CR2_OIS1N_POS), // Output Idle state 1
    TIM_CR2_OIS1  = (1 << TIM_CR2_OIS1_POS),  // Output Idle state 1
    TIM_CR2_TI1S  = (1 << TIM_CR2_TI1S_POS),  // TI1 selection
    TIM_CR2_MMS   = (1 << TIM_CR2_MMS_POS),   // Master mode selection
    TIM_CR2_CCDS  = (1 << TIM_CR2_CCDS_POS),  // Capture/compare DMA selection
    TIM_CR2_CCUS  = (1 << TIM_CR2_CCUS_POS),  // Capture/compare control update selection
    TIM_CR2_CCPC  = (1 << TIM_CR2_CCPC_POS),  // Capture/compare preloaded control
} tim_cr2_e;

typedef enum {
    TIM_CR2_OIS4_MASK  = 0b1,   // 1 bit(s)
    TIM_CR2_OIS3N_MASK = 0b1,   // 1 bit(s)
    TIM_CR2_OIS3_MASK  = 0b1,   // 1 bit(s)
    TIM_CR2_OIS2N_MASK = 0b1,   // 1 bit(s)
    TIM_CR2_OIS2_MASK  = 0b1,   // 1 bit(s)
    TIM_CR2_OIS1N_MASK = 0b1,   // 1 bit(s)
    TIM_CR2_OIS1_MASK  = 0b1,   // 1 bit(s)
    TIM_CR2_TI1S_MASK  = 0b1,   // 1 bit(s)
    TIM_CR2_MMS_MASK   = 0b111, // 3 bit(s)
    TIM_CR2_CCDS_MASK  = 0b1,   // 1 bit(s)
    TIM_CR2_CCUS_MASK  = 0b1,   // 1 bit(s)
    TIM_CR2_CCPC_MASK  = 0b1,   // 1 bit(s)
} tim_cr2_mask_e;

typedef enum {
    TIM_SMCR_ETP_POS  = 15, // External trigger polarity
    TIM_SMCR_ECE_POS  = 14, // External clock enable
    TIM_SMCR_ETPS_POS = 12, // External trigger prescaler
    TIM_SMCR_ETF_POS  = 8,  // External trigger filter
    TIM_SMCR_MSM_POS  = 7,  // Master/Slave mode
    TIM_SMCR_TS_POS   = 4,  // Trigger selection
    TIM_SMCR_SMS_POS  = 0,  // Slave mode selection
} tim_smcr_pos_e;

typedef enum {
    TIM_SMCR_ETP  = (1 << TIM_SMCR_ETP_POS),  // External trigger polarity
    TIM_SMCR_ECE  = (1 << TIM_SMCR_ECE_POS),  // External clock enable
    TIM_SMCR_ETPS = (1 << TIM_SMCR_ETPS_POS), // External trigger prescaler
    TIM_SMCR_ETF  = (1 << TIM_SMCR_ETF_POS),  // External trigger filter
    TIM_SMCR_MSM  = (1 << TIM_SMCR_MSM_POS),  // Master/Slave mode
    TIM_SMCR_TS   = (1 << TIM_SMCR_TS_POS),   // Trigger selection
    TIM_SMCR_SMS  = (1 << TIM_SMCR_SMS_POS),  // Slave mode selection
} tim_smcr_e;

typedef enum {
    TIM_SMCR_ETP_MASK  = 0b1,    // 1 bit(s)
    TIM_SMCR_ECE_MASK  = 0b1,    // 1 bit(s)
    TIM_SMCR_ETPS_MASK = 0b11,   // 2 bit(s)
    TIM_SMCR_ETF_MASK  = 0b1111, // 4 bit(s)
    TIM_SMCR_MSM_MASK  = 0b1,    // 1 bit(s)
    TIM_SMCR_TS_MASK   = 0b111,  // 3 bit(s)
    TIM_SMCR_SMS_MASK  = 0b111,  // 3 bit(s)
} tim_smcr_mask_e;

typedef enum {
    TIM_DIER_TDE_POS   = 14, // Trigger DMA request enable
    TIM_DIER_COMDE_POS = 13, // COM DMA request enable
    TIM_DIER_CC4DE_POS = 12, // Capture/Compare 4 DMA request enable
    TIM_DIER_CC3DE_POS = 11, // Capture/Compare 3 DMA request enable
    TIM_DIER_CC2DE_POS = 10, // Capture/Compare 2 DMA request enable
    TIM_DIER_CC1DE_POS = 9,  // Capture/Compare 1 DMA request enable
    TIM_DIER_UDE_POS   = 8,  // Update DMA request enable
    TIM_DIER_TIE_POS   = 6,  // Trigger interrupt enable
    TIM_DIER_CC4IE_POS = 4,  // Capture/Compare 4 interrupt enable
    TIM_DIER_CC3IE_POS = 3,  // Capture/Compare 3 interrupt enable
    TIM_DIER_CC2IE_POS = 2,  // Capture/Compare 2 interrupt enable
    TIM_DIER_CC1IE_POS = 1,  // Capture/Compare 1 interrupt enable
    TIM_DIER_UIE_POS   = 0,  // Update interrupt enable
    TIM_DIER_BIE_POS   = 7,  // Break interrupt enable
    TIM_DIER_COMIE_POS = 5,  // COM interrupt enable
} tim_dier_pos_e;

typedef enum {
    TIM_DIER_TDE   = (1 << TIM_DIER_TDE_POS),   // Trigger DMA request enable
    TIM_DIER_COMDE = (1 << TIM_DIER_COMDE_POS), // COM DMA request enable
    TIM_DIER_CC4DE = (1 << TIM_DIER_CC4DE_POS), // Capture/Compare 4 DMA request enable
    TIM_DIER_CC3DE = (1 << TIM_DIER_CC3DE_POS), // Capture/Compare 3 DMA request enable
    TIM_DIER_CC2DE = (1 << TIM_DIER_CC2DE_POS), // Capture/Compare 2 DMA request enable
    TIM_DIER_CC1DE = (1 << TIM_DIER_CC1DE_POS), // Capture/Compare 1 DMA request enable
    TIM_DIER_UDE   = (1 << TIM_DIER_UDE_POS),   // Update DMA request enable
    TIM_DIER_TIE   = (1 << TIM_DIER_TIE_POS),   // Trigger interrupt enable
    TIM_DIER_CC4IE = (1 << TIM_DIER_CC4IE_POS), // Capture/Compare 4 interrupt enable
    TIM_DIER_CC3IE = (1 << TIM_DIER_CC3IE_POS), // Capture/Compare 3 interrupt enable
    TIM_DIER_CC2IE = (1 << TIM_DIER_CC2IE_POS), // Capture/Compare 2 interrupt enable
    TIM_DIER_CC1IE = (1 << TIM_DIER_CC1IE_POS), // Capture/Compare 1 interrupt enable
    TIM_DIER_UIE   = (1 << TIM_DIER_UIE_POS),   // Update interrupt enable
    TIM_DIER_BIE   = (1 << TIM_DIER_BIE_POS),   // Break interrupt enable
    TIM_DIER_COMIE = (1 << TIM_DIER_COMIE_POS), // COM interrupt enable
} tim_dier_e;

typedef enum {
    TIM_DIER_TDE_MASK   = 0b1, // 1 bit(s)
    TIM_DIER_COMDE_MASK = 0b1, // 1 bit(s)
    TIM_DIER_CC4DE_MASK = 0b1, // 1 bit(s)
    TIM_DIER_CC3DE_MASK = 0b1, // 1 bit(s)
    TIM_DIER_CC2DE_MASK = 0b1, // 1 bit(s)
    TIM_DIER_CC1DE_MASK = 0b1, // 1 bit(s)
    TIM_DIER_UDE_MASK   = 0b1, // 1 bit(s)
    TIM_DIER_TIE_MASK   = 0b1, // 1 bit(s)
    TIM_DIER_CC4IE_MASK = 0b1, // 1 bit(s)
    TIM_DIER_CC3IE_MASK = 0b1, // 1 bit(s)
    TIM_DIER_CC2IE_MASK = 0b1, // 1 bit(s)
    TIM_DIER_CC1IE_MASK = 0b1, // 1 bit(s)
    TIM_DIER_UIE_MASK   = 0b1, // 1 bit(s)
    TIM_DIER_BIE_MASK   = 0b1, // 1 bit(s)
    TIM_DIER_COMIE_MASK = 0b1, // 1 bit(s)
} tim_dier_mask_e;

typedef enum {
    TIM_SR_CC4OF_POS = 12, // Capture/Compare 4 overcapture flag
    TIM_SR_CC3OF_POS = 11, // Capture/Compare 3 overcapture flag
    TIM_SR_CC2OF_POS = 10, // Capture/compare 2 overcapture flag
    TIM_SR_CC1OF_POS = 9,  // Capture/Compare 1 overcapture flag
    TIM_SR_BIF_POS   = 7,  // Break interrupt flag
    TIM_SR_TIF_POS   = 6,  // Trigger interrupt flag
    TIM_SR_COMIF_POS = 5,  // COM interrupt flag
    TIM_SR_CC4IF_POS = 4,  // Capture/Compare 4 interrupt flag
    TIM_SR_CC3IF_POS = 3,  // Capture/Compare 3 interrupt flag
    TIM_SR_CC2IF_POS = 2,  // Capture/Compare 2 interrupt flag
    TIM_SR_CC1IF_POS = 1,  // Capture/compare 1 interrupt flag
    TIM_SR_UIF_POS   = 0,  // Update interrupt flag
} tim_sr_pos_e;

typedef enum {
    TIM_SR_CC4OF = (1 << TIM_SR_CC4OF_POS), // Capture/Compare 4 overcapture flag
    TIM_SR_CC3OF = (1 << TIM_SR_CC3OF_POS), // Capture/Compare 3 overcapture flag
    TIM_SR_CC2OF = (1 << TIM_SR_CC2OF_POS), // Capture/compare 2 overcapture flag
    TIM_SR_CC1OF = (1 << TIM_SR_CC1OF_POS), // Capture/Compare 1 overcapture flag
    TIM_SR_BIF   = (1 << TIM_SR_BIF_POS),   // Break interrupt flag
    TIM_SR_TIF   = (1 << TIM_SR_TIF_POS),   // Trigger interrupt flag
    TIM_SR_COMIF = (1 << TIM_SR_COMIF_POS), // COM interrupt flag
    TIM_SR_CC4IF = (1 << TIM_SR_CC4IF_POS), // Capture/Compare 4 interrupt flag
    TIM_SR_CC3IF = (1 << TIM_SR_CC3IF_POS), // Capture/Compare 3 interrupt flag
    TIM_SR_CC2IF = (1 << TIM_SR_CC2IF_POS), // Capture/Compare 2 interrupt flag
    TIM_SR_CC1IF = (1 << TIM_SR_CC1IF_POS), // Capture/compare 1 interrupt flag
    TIM_SR_UIF   = (1 << TIM_SR_UIF_POS),   // Update interrupt flag
} tim_sr_e;

typedef enum {
    TIM_SR_CC4OF_MASK = 0b1, // 1 bit(s)
    TIM_SR_CC3OF_MASK = 0b1, // 1 bit(s)
    TIM_SR_CC2OF_MASK = 0b1, // 1 bit(s)
    TIM_SR_CC1OF_MASK = 0b1, // 1 bit(s)
    TIM_SR_BIF_MASK   = 0b1, // 1 bit(s)
    TIM_SR_TIF_MASK   = 0b1, // 1 bit(s)
    TIM_SR_COMIF_MASK = 0b1, // 1 bit(s)
    TIM_SR_CC4IF_MASK = 0b1, // 1 bit(s)
    TIM_SR_CC3IF_MASK = 0b1, // 1 bit(s)
    TIM_SR_CC2IF_MASK = 0b1, // 1 bit(s)
    TIM_SR_CC1IF_MASK = 0b1, // 1 bit(s)
    TIM_SR_UIF_MASK   = 0b1, // 1 bit(s)
} tim_sr_mask_e;

typedef enum {
    TIM_EGR_BG_POS   = 7, // Break generation
    TIM_EGR_TG_POS   = 6, // Trigger generation
    TIM_EGR_COMG_POS = 5, // Capture/Compare control update generation
    TIM_EGR_CC4G_POS = 4, // Capture/compare 4 generation
    TIM_EGR_CC3G_POS = 3, // Capture/compare 3 generation
    TIM_EGR_CC2G_POS = 2, // Capture/compare 2 generation
    TIM_EGR_CC1G_POS = 1, // Capture/compare 1 generation
    TIM_EGR_UG_POS   = 0, // Update generation
} tim_egr_pos_e;

typedef enum {
    TIM_EGR_BG   = (1 << TIM_EGR_BG_POS),   // Break generation
    TIM_EGR_TG   = (1 << TIM_EGR_TG_POS),   // Trigger generation
    TIM_EGR_COMG = (1 << TIM_EGR_COMG_POS), // Capture/Compare control update generation
    TIM_EGR_CC4G = (1 << TIM_EGR_CC4G_POS), // Capture/compare 4 generation
    TIM_EGR_CC3G = (1 << TIM_EGR_CC3G_POS), // Capture/compare 3 generation
    TIM_EGR_CC2G = (1 << TIM_EGR_CC2G_POS), // Capture/compare 2 generation
    TIM_EGR_CC1G = (1 << TIM_EGR_CC1G_POS), // Capture/compare 1 generation
    TIM_EGR_UG   = (1 << TIM_EGR_UG_POS),   // Update generation
} tim_egr_e;

typedef enum {
    TIM_EGR_BG_MASK   = 0b1, // 1 bit(s)
    TIM_EGR_TG_MASK   = 0b1, // 1 bit(s)
    TIM_EGR_COMG_MASK = 0b1, // 1 bit(s)
    TIM_EGR_CC4G_MASK = 0b1, // 1 bit(s)
    TIM_EGR_CC3G_MASK = 0b1, // 1 bit(s)
    TIM_EGR_CC2G_MASK = 0b1, // 1 bit(s)
    TIM_EGR_CC1G_MASK = 0b1, // 1 bit(s)
    TIM_EGR_UG_MASK   = 0b1, // 1 bit(s)
} tim_egr_mask_e;

typedef enum {
    TIM_CCMR1_OUTPUT_OC2CE_POS = 15, // Output Compare 2 clear enable
    TIM_CCMR1_OUTPUT_OC2M_POS  = 12, // Output Compare 2 mode
    TIM_CCMR1_OUTPUT_OC2PE_POS = 11, // Output Compare 2 preload enable
    TIM_CCMR1_OUTPUT_OC2FE_POS = 10, // Output Compare 2 fast enable
    TIM_CCMR1_OUTPUT_CC2S_POS  = 8,  // Capture/Compare 2 selection
    TIM_CCMR1_OUTPUT_OC1CE_POS = 7,  // Output Compare 1 clear enable
    TIM_CCMR1_OUTPUT_OC1M_POS  = 4,  // Output Compare 1 mode
    TIM_CCMR1_OUTPUT_OC1PE_POS = 3,  // Output Compare 1 preload enable
    TIM_CCMR1_OUTPUT_OC1FE_POS = 2,  // Output Compare 1 fast enable
    TIM_CCMR1_OUTPUT_CC1S_POS  = 0,  // Capture/Compare 1 selection
} tim_ccmr1_output_pos_e;

typedef enum {
    TIM_CCMR1_OUTPUT_OC2CE = (1 << TIM_CCMR1_OUTPUT_OC2CE_POS), // Output Compare 2 clear enable
    TIM_CCMR1_OUTPUT_OC2M  = (1 << TIM_CCMR1_OUTPUT_OC2M_POS),  // Output Compare 2 mode
    TIM_CCMR1_OUTPUT_OC2PE = (1 << TIM_CCMR1_OUTPUT_OC2PE_POS), // Output Compare 2 preload enable
    TIM_CCMR1_OUTPUT_OC2FE = (1 << TIM_CCMR1_OUTPUT_OC2FE_POS), // Output Compare 2 fast enable
    TIM_CCMR1_OUTPUT_CC2S  = (1 << TIM_CCMR1_OUTPUT_CC2S_POS),  // Capture/Compare 2 selection
    TIM_CCMR1_OUTPUT_OC1CE = (1 << TIM_CCMR1_OUTPUT_OC1CE_POS), // Output Compare 1 clear enable
    TIM_CCMR1_OUTPUT_OC1M  = (1 << TIM_CCMR1_OUTPUT_OC1M_POS),  // Output Compare 1 mode
    TIM_CCMR1_OUTPUT_OC1PE = (1 << TIM_CCMR1_OUTPUT_OC1PE_POS), // Output Compare 1 preload enable
    TIM_CCMR1_OUTPUT_OC1FE = (1 << TIM_CCMR1_OUTPUT_OC1FE_POS), // Output Compare 1 fast enable
    TIM_CCMR1_OUTPUT_CC1S  = (1 << TIM_CCMR1_OUTPUT_CC1S_POS),  // Capture/Compare 1 selection
} tim_ccmr1_output_e;

typedef enum {
    TIM_CCMR1_OUTPUT_OC2CE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_OC2M_MASK  = 0b111, // 3 bit(s)
    TIM_CCMR1_OUTPUT_OC2PE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_OC2FE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_CC2S_MASK  = 0b11,  // 2 bit(s)
    TIM_CCMR1_OUTPUT_OC1CE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_OC1M_MASK  = 0b111, // 3 bit(s)
    TIM_CCMR1_OUTPUT_OC1PE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_OC1FE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_CC1S_MASK  = 0b11,  // 2 bit(s)
} tim_ccmr1_output_mask_e;

typedef enum {
    TIM_CCMR1_INPUT_IC2F_POS   = 12, // Input capture 2 filter
    TIM_CCMR1_INPUT_IC2PCS_POS = 10, // Input capture 2 prescaler
    TIM_CCMR1_INPUT_CC2S_POS   = 8,  // Capture/Compare 2 selection
    TIM_CCMR1_INPUT_IC1F_POS   = 4,  // Input capture 1 filter
    TIM_CCMR1_INPUT_ICPCS_POS  = 2,  // Input capture 1 prescaler
    TIM_CCMR1_INPUT_CC1S_POS   = 0,  // Capture/Compare 1 selection
} tim_ccmr1_input_pos_e;

typedef enum {
    TIM_CCMR1_INPUT_IC2F   = (1 << TIM_CCMR1_INPUT_IC2F_POS),   // Input capture 2 filter
    TIM_CCMR1_INPUT_IC2PCS = (1 << TIM_CCMR1_INPUT_IC2PCS_POS), // Input capture 2 prescaler
    TIM_CCMR1_INPUT_CC2S   = (1 << TIM_CCMR1_INPUT_CC2S_POS),   // Capture/Compare 2 selection
    TIM_CCMR1_INPUT_IC1F   = (1 << TIM_CCMR1_INPUT_IC1F_POS),   // Input capture 1 filter
    TIM_CCMR1_INPUT_ICPCS  = (1 << TIM_CCMR1_INPUT_ICPCS_POS),  // Input capture 1 prescaler
    TIM_CCMR1_INPUT_CC1S   = (1 << TIM_CCMR1_INPUT_CC1S_POS),   // Capture/Compare 1 selection
} tim_ccmr1_input_e;

typedef enum {
    TIM_CCMR1_INPUT_IC2F_MASK   = 0b1111, // 4 bit(s)
    TIM_CCMR1_INPUT_IC2PCS_MASK = 0b11,   // 2 bit(s)
    TIM_CCMR1_INPUT_CC2S_MASK   = 0b11,   // 2 bit(s)
    TIM_CCMR1_INPUT_IC1F_MASK   = 0b1111, // 4 bit(s)
    TIM_CCMR1_INPUT_ICPCS_MASK  = 0b11,   // 2 bit(s)
    TIM_CCMR1_INPUT_CC1S_MASK   = 0b11,   // 2 bit(s)
} tim_ccmr1_input_mask_e;

typedef enum {
    TIM_CCMR2_OUTPUT_OC4CE_POS = 15, // Output compare 4 clear enable
    TIM_CCMR2_OUTPUT_OC4M_POS  = 12, // Output compare 4 mode
    TIM_CCMR2_OUTPUT_OC4PE_POS = 11, // Output compare 4 preload enable
    TIM_CCMR2_OUTPUT_OC4FE_POS = 10, // Output compare 4 fast enable
    TIM_CCMR2_OUTPUT_CC4S_POS  = 8,  // Capture/Compare 4 selection
    TIM_CCMR2_OUTPUT_OC3CE_POS = 7,  // Output compare 3 clear enable
    TIM_CCMR2_OUTPUT_OC3M_POS  = 4,  // Output compare 3 mode
    TIM_CCMR2_OUTPUT_OC3PE_POS = 3,  // Output compare 3 preload enable
    TIM_CCMR2_OUTPUT_OC3FE_POS = 2,  // Output compare 3 fast enable
    TIM_CCMR2_OUTPUT_CC3S_POS  = 0,  // Capture/Compare 3 selection
} tim_ccmr2_output_pos_e;

typedef enum {
    TIM_CCMR2_OUTPUT_OC4CE = (1 << TIM_CCMR2_OUTPUT_OC4CE_POS), // Output compare 4 clear enable
    TIM_CCMR2_OUTPUT_OC4M  = (1 << TIM_CCMR2_OUTPUT_OC4M_POS),  // Output compare 4 mode
    TIM_CCMR2_OUTPUT_OC4PE = (1 << TIM_CCMR2_OUTPUT_OC4PE_POS), // Output compare 4 preload enable
    TIM_CCMR2_OUTPUT_OC4FE = (1 << TIM_CCMR2_OUTPUT_OC4FE_POS), // Output compare 4 fast enable
    TIM_CCMR2_OUTPUT_CC4S  = (1 << TIM_CCMR2_OUTPUT_CC4S_POS),  // Capture/Compare 4 selection
    TIM_CCMR2_OUTPUT_OC3CE = (1 << TIM_CCMR2_OUTPUT_OC3CE_POS), // Output compare 3 clear enable
    TIM_CCMR2_OUTPUT_OC3M  = (1 << TIM_CCMR2_OUTPUT_OC3M_POS),  // Output compare 3 mode
    TIM_CCMR2_OUTPUT_OC3PE = (1 << TIM_CCMR2_OUTPUT_OC3PE_POS), // Output compare 3 preload enable
    TIM_CCMR2_OUTPUT_OC3FE = (1 << TIM_CCMR2_OUTPUT_OC3FE_POS), // Output compare 3 fast enable
    TIM_CCMR2_OUTPUT_CC3S  = (1 << TIM_CCMR2_OUTPUT_CC3S_POS),  // Capture/Compare 3 selection
} tim_ccmr2_output_e;

typedef enum {
    TIM_CCMR2_OUTPUT_OC4CE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_OC4M_MASK  = 0b111, // 3 bit(s)
    TIM_CCMR2_OUTPUT_OC4PE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_OC4FE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_CC4S_MASK  = 0b11,  // 2 bit(s)
    TIM_CCMR2_OUTPUT_OC3CE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_OC3M_MASK  = 0b111, // 3 bit(s)
    TIM_CCMR2_OUTPUT_OC3PE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_OC3FE_MASK = 0b1,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_CC3S_MASK  = 0b11,  // 2 bit(s)
} tim_ccmr2_output_mask_e;

typedef enum {
    TIM_CCMR2_INPUT_IC4F_POS   = 12, // Input capture 4 filter
    TIM_CCMR2_INPUT_IC4PSC_POS = 10, // Input capture 4 prescaler
    TIM_CCMR2_INPUT_CC4S_POS   = 8,  // Capture/Compare 4 selection
    TIM_CCMR2_INPUT_IC3F_POS   = 4,  // Input capture 3 filter
    TIM_CCMR2_INPUT_IC3PSC_POS = 2,  // Input capture 3 prescaler
    TIM_CCMR2_INPUT_CC3S_POS   = 0,  // Capture/compare 3 selection
} tim_ccmr2_input_pos_e;

typedef enum {
    TIM_CCMR2_INPUT_IC4F   = (1 << TIM_CCMR2_INPUT_IC4F_POS),   // Input capture 4 filter
    TIM_CCMR2_INPUT_IC4PSC = (1 << TIM_CCMR2_INPUT_IC4PSC_POS), // Input capture 4 prescaler
    TIM_CCMR2_INPUT_CC4S   = (1 << TIM_CCMR2_INPUT_CC4S_POS),   // Capture/Compare 4 selection
    TIM_CCMR2_INPUT_IC3F   = (1 << TIM_CCMR2_INPUT_IC3F_POS),   // Input capture 3 filter
    TIM_CCMR2_INPUT_IC3PSC = (1 << TIM_CCMR2_INPUT_IC3PSC_POS), // Input capture 3 prescaler
    TIM_CCMR2_INPUT_CC3S   = (1 << TIM_CCMR2_INPUT_CC3S_POS),   // Capture/compare 3 selection
} tim_ccmr2_input_e;

typedef enum {
    TIM_CCMR2_INPUT_IC4F_MASK   = 0b1111, // 4 bit(s)
    TIM_CCMR2_INPUT_IC4PSC_MASK = 0b11,   // 2 bit(s)
    TIM_CCMR2_INPUT_CC4S_MASK   = 0b11,   // 2 bit(s)
    TIM_CCMR2_INPUT_IC3F_MASK   = 0b1111, // 4 bit(s)
    TIM_CCMR2_INPUT_IC3PSC_MASK = 0b11,   // 2 bit(s)
    TIM_CCMR2_INPUT_CC3S_MASK   = 0b11,   // 2 bit(s)
} tim_ccmr2_input_mask_e;

typedef enum {
    TIM_CCER_CC4NP_POS = 15, // Capture/Compare 4 output Polarity
    TIM_CCER_CC4P_POS  = 13, // Capture/Compare 3 output Polarity
    TIM_CCER_CC4E_POS  = 12, // Capture/Compare 4 output enable
    TIM_CCER_CC3NP_POS = 11, // Capture/Compare 3 output Polarity
    TIM_CCER_CC3NE_POS = 10, // Capture/Compare 3 complementary output enable
    TIM_CCER_CC3P_POS  = 9,  // Capture/Compare 3 output Polarity
    TIM_CCER_CC3E_POS  = 8,  // Capture/Compare 3 output enable
    TIM_CCER_CC2NP_POS = 7,  // Capture/Compare 2 output Polarity
    TIM_CCER_CC2NE_POS = 6,  // Capture/Compare 2 complementary output enable
    TIM_CCER_CC2P_POS  = 5,  // Capture/Compare 2 output Polarity
    TIM_CCER_CC2E_POS  = 4,  // Capture/Compare 2 output enable
    TIM_CCER_CC1NP_POS = 3,  // Capture/Compare 1 output Polarity
    TIM_CCER_CC1NE_POS = 2,  // Capture/Compare 1 complementary output enable
    TIM_CCER_CC1P_POS  = 1,  // Capture/Compare 1 output Polarity
    TIM_CCER_CC1E_POS  = 0,  // Capture/Compare 1 output enable
} tim_ccer_pos_e;

typedef enum {
    TIM_CCER_CC4NP = (1 << TIM_CCER_CC4NP_POS), // Capture/Compare 4 output Polarity
    TIM_CCER_CC4P  = (1 << TIM_CCER_CC4P_POS),  // Capture/Compare 3 output Polarity
    TIM_CCER_CC4E  = (1 << TIM_CCER_CC4E_POS),  // Capture/Compare 4 output enable
    TIM_CCER_CC3NP = (1 << TIM_CCER_CC3NP_POS), // Capture/Compare 3 output Polarity
    TIM_CCER_CC3NE = (1 << TIM_CCER_CC3NE_POS), // Capture/Compare 3 complementary output enable
    TIM_CCER_CC3P  = (1 << TIM_CCER_CC3P_POS),  // Capture/Compare 3 output Polarity
    TIM_CCER_CC3E  = (1 << TIM_CCER_CC3E_POS),  // Capture/Compare 3 output enable
    TIM_CCER_CC2NP = (1 << TIM_CCER_CC2NP_POS), // Capture/Compare 2 output Polarity
    TIM_CCER_CC2NE = (1 << TIM_CCER_CC2NE_POS), // Capture/Compare 2 complementary output enable
    TIM_CCER_CC2P  = (1 << TIM_CCER_CC2P_POS),  // Capture/Compare 2 output Polarity
    TIM_CCER_CC2E  = (1 << TIM_CCER_CC2E_POS),  // Capture/Compare 2 output enable
    TIM_CCER_CC1NP = (1 << TIM_CCER_CC1NP_POS), // Capture/Compare 1 output Polarity
    TIM_CCER_CC1NE = (1 << TIM_CCER_CC1NE_POS), // Capture/Compare 1 complementary output enable
    TIM_CCER_CC1P  = (1 << TIM_CCER_CC1P_POS),  // Capture/Compare 1 output Polarity
    TIM_CCER_CC1E  = (1 << TIM_CCER_CC1E_POS),  // Capture/Compare 1 output enable
} tim_ccer_e;

typedef enum {
    TIM_CCER_CC4NP_MASK = 0b1, // 1 bit(s)
    TIM_CCER_CC4P_MASK  = 0b1, // 1 bit(s)
    TIM_CCER_CC4E_MASK  = 0b1, // 1 bit(s)
    TIM_CCER_CC3NP_MASK = 0b1, // 1 bit(s)
    TIM_CCER_CC3NE_MASK = 0b1, // 1 bit(s)
    TIM_CCER_CC3P_MASK  = 0b1, // 1 bit(s)
    TIM_CCER_CC3E_MASK  = 0b1, // 1 bit(s)
    TIM_CCER_CC2NP_MASK = 0b1, // 1 bit(s)
    TIM_CCER_CC2NE_MASK = 0b1, // 1 bit(s)
    TIM_CCER_CC2P_MASK  = 0b1, // 1 bit(s)
    TIM_CCER_CC2E_MASK  = 0b1, // 1 bit(s)
    TIM_CCER_CC1NP_MASK = 0b1, // 1 bit(s)
    TIM_CCER_CC1NE_MASK = 0b1, // 1 bit(s)
    TIM_CCER_CC1P_MASK  = 0b1, // 1 bit(s)
    TIM_CCER_CC1E_MASK  = 0b1, // 1 bit(s)
} tim_ccer_mask_e;

typedef enum {
    TIM_CNT_CNT_H_POS = 16, // High counter value
    TIM_CNT_CNT_L_POS = 0,  // Low counter value
} tim_cnt_pos_e;

typedef enum {
    TIM_CNT_CNT_H = (1 << TIM_CNT_CNT_H_POS), // High counter value
    TIM_CNT_CNT_L = (1 << TIM_CNT_CNT_L_POS), // Low counter value
} tim_cnt_e;

typedef enum {
    TIM_CNT_CNT_H_MASK = 0b1111111111111111, // 16 bit(s)
    TIM_CNT_CNT_L_MASK = 0b1111111111111111, // 16 bit(s)
} tim_cnt_mask_e;

typedef enum {
    TIM_PSC_PSC_POS = 0, // Prescaler value
} tim_psc_pos_e;

typedef enum {
    TIM_PSC_PSC = (1 << TIM_PSC_PSC_POS), // Prescaler value
} tim_psc_e;

typedef enum {
    TIM_PSC_PSC_MASK = 0b1111111111111111, // 16 bit(s)
} tim_psc_mask_e;

typedef enum {
    TIM_ARR_ARR_H_POS = 16, // High Auto-reload value
    TIM_ARR_ARR_L_POS = 0,  // Low Auto-reload value
} tim_arr_pos_e;

typedef enum {
    TIM_ARR_ARR_H = (1 << TIM_ARR_ARR_H_POS), // High Auto-reload value
    TIM_ARR_ARR_L = (1 << TIM_ARR_ARR_L_POS), // Low Auto-reload value
} tim_arr_e;

typedef enum {
    TIM_ARR_ARR_H_MASK = 0b1111111111111111, // 16 bit(s)
    TIM_ARR_ARR_L_MASK = 0b1111111111111111, // 16 bit(s)
} tim_arr_mask_e;

typedef enum {
    TIM_CCR1_CCR1_H_POS = 16, // High Capture/Compare 1 value
    TIM_CCR1_CCR1_L_POS = 0,  // Low Capture/Compare 1 value
} tim_ccr1_pos_e;

typedef enum {
    TIM_CCR1_CCR1_H = (1 << TIM_CCR1_CCR1_H_POS), // High Capture/Compare 1 value
    TIM_CCR1_CCR1_L = (1 << TIM_CCR1_CCR1_L_POS), // Low Capture/Compare 1 value
} tim_ccr1_e;

typedef enum {
    TIM_CCR1_CCR1_H_MASK = 0b1111111111111111, // 16 bit(s)
    TIM_CCR1_CCR1_L_MASK = 0b1111111111111111, // 16 bit(s)
} tim_ccr1_mask_e;

typedef enum {
    TIM_CCR2_CCR2_H_POS = 16, // High Capture/Compare 2 value
    TIM_CCR2_CCR2_L_POS = 0,  // Low Capture/Compare 2 value
} tim_ccr2_pos_e;

typedef enum {
    TIM_CCR2_CCR2_H = (1 << TIM_CCR2_CCR2_H_POS), // High Capture/Compare 2 value
    TIM_CCR2_CCR2_L = (1 << TIM_CCR2_CCR2_L_POS), // Low Capture/Compare 2 value
} tim_ccr2_e;

typedef enum {
    TIM_CCR2_CCR2_H_MASK = 0b1111111111111111, // 16 bit(s)
    TIM_CCR2_CCR2_L_MASK = 0b1111111111111111, // 16 bit(s)
} tim_ccr2_mask_e;

typedef enum {
    TIM_CCR3_CCR3_H_POS = 16, // High Capture/Compare value
    TIM_CCR3_CCR3_L_POS = 0,  // Low Capture/Compare value
} tim_ccr3_pos_e;

typedef enum {
    TIM_CCR3_CCR3_H = (1 << TIM_CCR3_CCR3_H_POS), // High Capture/Compare value
    TIM_CCR3_CCR3_L = (1 << TIM_CCR3_CCR3_L_POS), // Low Capture/Compare value
} tim_ccr3_e;

typedef enum {
    TIM_CCR3_CCR3_H_MASK = 0b1111111111111111, // 16 bit(s)
    TIM_CCR3_CCR3_L_MASK = 0b1111111111111111, // 16 bit(s)
} tim_ccr3_mask_e;

typedef enum {
    TIM_CCR4_CCR4_H_POS = 16, // High Capture/Compare value
    TIM_CCR4_CCR4_L_POS = 0,  // Low Capture/Compare value
} tim_ccr4_pos_e;

typedef enum {
    TIM_CCR4_CCR4_H = (1 << TIM_CCR4_CCR4_H_POS), // High Capture/Compare value
    TIM_CCR4_CCR4_L = (1 << TIM_CCR4_CCR4_L_POS), // Low Capture/Compare value
} tim_ccr4_e;

typedef enum {
    TIM_CCR4_CCR4_H_MASK = 0b1111111111111111, // 16 bit(s)
    TIM_CCR4_CCR4_L_MASK = 0b1111111111111111, // 16 bit(s)
} tim_ccr4_mask_e;

typedef enum {
    TIM_DCR_DBL_POS = 8, // DMA burst length
    TIM_DCR_DBA_POS = 0, // DMA base address
} tim_dcr_pos_e;

typedef enum {
    TIM_DCR_DBL = (1 << TIM_DCR_DBL_POS), // DMA burst length
    TIM_DCR_DBA = (1 << TIM_DCR_DBA_POS), // DMA base address
} tim_dcr_e;

typedef enum {
    TIM_DCR_DBL_MASK = 0b11111, // 5 bit(s)
    TIM_DCR_DBA_MASK = 0b11111, // 5 bit(s)
} tim_dcr_mask_e;

typedef enum {
    TIM_DMAR_DMAB_POS = 0, // DMA register for burst accesses
} tim_dmar_pos_e;

typedef enum {
    TIM_DMAR_DMAB = (1 << TIM_DMAR_DMAB_POS), // DMA register for burst accesses
} tim_dmar_e;

typedef enum {
    TIM_DMAR_DMAB_MASK = 0b1111111111111111, // 16 bit(s)
} tim_dmar_mask_e;

typedef enum {
    TIM_OR_ITR1_RMP_POS = 10, // Timer Input 4 remap
} tim_or_pos_e;

typedef enum {
    TIM_OR_ITR1_RMP = (1 << TIM_OR_ITR1_RMP_POS), // Timer Input 4 remap
} tim_or_e;

typedef enum {
    TIM_OR_ITR1_RMP_MASK = 0b11, // 2 bit(s)
} tim_or_mask_e;

typedef enum {
    TIM_BDTR_MOE_POS  = 15, // Main output enable
    TIM_BDTR_AOE_POS  = 14, // Automatic output enable
    TIM_BDTR_BKP_POS  = 13, // Break polarity
    TIM_BDTR_BKE_POS  = 12, // Break enable
    TIM_BDTR_OSSR_POS = 11, // Off-state selection for Run mode
    TIM_BDTR_OSSI_POS = 10, // Off-state selection for Idle mode
    TIM_BDTR_LOCK_POS = 8,  // Lock configuration
    TIM_BDTR_DTG_POS  = 0,  // Dead-time generator setup
} tim_bdtr_pos_e;

typedef enum {
    TIM_BDTR_MOE  = (1 << TIM_BDTR_MOE_POS),  // Main output enable
    TIM_BDTR_AOE  = (1 << TIM_BDTR_AOE_POS),  // Automatic output enable
    TIM_BDTR_BKP  = (1 << TIM_BDTR_BKP_POS),  // Break polarity
    TIM_BDTR_BKE  = (1 << TIM_BDTR_BKE_POS),  // Break enable
    TIM_BDTR_OSSR = (1 << TIM_BDTR_OSSR_POS), // Off-state selection for Run mode
    TIM_BDTR_OSSI = (1 << TIM_BDTR_OSSI_POS), // Off-state selection for Idle mode
    TIM_BDTR_LOCK = (1 << TIM_BDTR_LOCK_POS), // Lock configuration
    TIM_BDTR_DTG  = (1 << TIM_BDTR_DTG_POS),  // Dead-time generator setup
} tim_bdtr_e;

typedef enum {
    TIM_BDTR_MOE_MASK  = 0b1,        // 1 bit(s)
    TIM_BDTR_AOE_MASK  = 0b1,        // 1 bit(s)
    TIM_BDTR_BKP_MASK  = 0b1,        // 1 bit(s)
    TIM_BDTR_BKE_MASK  = 0b1,        // 1 bit(s)
    TIM_BDTR_OSSR_MASK = 0b1,        // 1 bit(s)
    TIM_BDTR_OSSI_MASK = 0b1,        // 1 bit(s)
    TIM_BDTR_LOCK_MASK = 0b11,       // 2 bit(s)
    TIM_BDTR_DTG_MASK  = 0b11111111, // 8 bit(s)
} tim_bdtr_mask_e;

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

typedef __vo struct {
    uint32_t CR1;     // control register 1                                 Offset: 0x0
    uint32_t CR2;     // control register 2                                 Offset: 0x4
    uint32_t SMCR;    // slave mode control register                        Offset: 0x8
    uint32_t DIER;    // DMA/Interrupt enable register                      Offset: 0xC
    uint32_t SR;      // status register                                    Offset: 0x10
    uint32_t EGR;     // event generation register                          Offset: 0x14
    uint32_t CCMR[2]; // capture/compare mode register 1 & 2                Offset: 0x18
    uint32_t CCER;    // capture/compare enable register                    Offset: 0x20
    uint32_t CNT;     // counter                                            Offset: 0x24
    uint32_t PSC;     // prescaler                                          Offset: 0x28
    uint32_t ARR;     // auto-reload register                               Offset: 0x2C
    uint32_t RCR;     // repetition counter register                        Offset: 0x30
    uint32_t CCR[4];  // capture/compare register 1-4                       Offset: 0x34
    uint32_t BDTR;    // break and dead-time register                       Offset: 0x44
    uint32_t DCR;     // DMA control register                               Offset: 0x48
    uint32_t DMAR;    // DMA address for full transfer                      Offset: 0x4C
    uint32_t OR;      // TIM5 option register                               Offset: 0x50
} tim_reg_def;

// Base config settings
typedef struct {
    tim_arr_preload_e preload;           // Default: TIM_ARR_PRELOAD_DI
    tim_one_pulse_mode_e one_pulse_mode; // Default: TIM_ONE_PULSE_MODE_DI
    tim_clock_sel_e clock_sel;           // Default: TIM_CLK_SEL_INTERNAL
    tim_trigger_sel_e trigger_sel;       // Default: TIM_TRIGGER_SEL_ITR0
    tim_cnt_dir_e direction;             // Default: TIM_CNT_DIR_UP
    tim_align_mode_e align_mode;         // Default: TIM_ALIGN_EDGE
} tim_config;

// Base timer timing settings
typedef struct {
    uint32_t auto_reload;    // Default: 0
    uint32_t prescaler;      // Default: 0
    uint32_t time;           // Default: 0
    tim_unit_of_time_e unit; // Default: TIM_UNIT_S
} tim_timing_config;

// Base timer handler
typedef struct {
    tim_reg_def *p_timx;
    tim_config tim_conf;
    tim_timing_config timing_conf;
} tim_handler;

// Timer input capture channel settings
typedef struct {
    tim_channel_sel_e channel_num;          // Default: TIM_CHANNEL_SEL_1
    tim_dts_mult_e dts_mult;                // Default: TIM_DTS_FREQ_CLK_INT
    tim_input_map_type_e input_map;         // Default: TIM_INPUT_MAP_MAIN
    tim_ic_prescaler_e prescaler;           // Default: TIM_IC_PRESCALER_NA
    tim_ic_filter_e filter;                 // Default: TIM_IC_FILTER_NA
    tim_ic_edge_detection_e edge_detection; // Default: TIM_IC_EDGE_DETECTION_RISING
} tim_ic_config;

// Timer output compare channel settings
typedef struct {
    tim_channel_sel_e channel_num;    // Default: TIM_CHANNEL_SEL_1
    tim_oc_output_mode_e output_mode; // Default: TIM_OC_OPM_FROZEN
    tim_oc_polarity_e polarity;       // Default: TIM_OC_POL_ACTIVE_HIGH
    tim_arr_preload_e arr_preload;    // Default: TIM_ARR_PRELOAD_DI
    uint32_t duty_cycle;              // Default: 0
    uint32_t ccr_value;               // Default: 0
} tim_oc_config;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define TIM1  ((tim_reg_def *)TIM1_BASE_ADDR)
#define TIM2  ((tim_reg_def *)TIM2_BASE_ADDR)
#define TIM3  ((tim_reg_def *)TIM3_BASE_ADDR)
#define TIM4  ((tim_reg_def *)TIM4_BASE_ADDR)
#define TIM5  ((tim_reg_def *)TIM5_BASE_ADDR)
#define TIM6  ((tim_reg_def *)TIM6_BASE_ADDR)
#define TIM7  ((tim_reg_def *)TIM7_BASE_ADDR)
#define TIM8  ((tim_reg_def *)TIM8_BASE_ADDR)
#define TIM9  ((tim_reg_def *)TIM9_BASE_ADDR)
#define TIM10 ((tim_reg_def *)TIM10_BASE_ADDR)
#define TIM11 ((tim_reg_def *)TIM11_BASE_ADDR)
#define TIM12 ((tim_reg_def *)TIM12_BASE_ADDR)
#define TIM13 ((tim_reg_def *)TIM13_BASE_ADDR)
#define TIM14 ((tim_reg_def *)TIM14_BASE_ADDR)

//======================================================================================//}
//                  Function API Prototypes
//======================================================================================//{

// Delay functions
void tim_delay(uint32_t time, tim_unit_of_time_e unit);

void tim_init(tim_handler *const p_tim_handler);
void tim_reset(tim_reg_def const *const p_timx);

void tim_ic_init(tim_reg_def *const p_timx, tim_ic_config ic_config);
void tim_oc_init(tim_reg_def *const p_timx, tim_oc_config oc_config);

void tim_start(tim_reg_def *const p_timx);
void tim_start_it(tim_reg_def *const p_timx);
void tim_channel_start(tim_reg_def *const p_timx, tim_channel_sel_e channel);
void tim_channel_start_it(tim_reg_def *const p_timx, tim_channel_sel_e channel);

void tim_it_config(tim_reg_def const *const p_timx, tim_channel_sel_e channel, togglable_e toggle);

tim_status_e tim_read_base_status(tim_reg_def const *const p_timx);
void tim_reset_base_status(tim_reg_def *const p_timx);
tim_status_e tim_read_channel_status(tim_reg_def const *const p_timx, tim_channel_sel_e channel);
void tim_reset_channel_status(tim_reg_def *const p_timx, tim_channel_sel_e channel);

uint32_t tim_get_channel_ccr(tim_reg_def *const p_timx, tim_channel_sel_e channel);
bus_types get_tim_bus(tim_reg_def const *const p_timx);

//} Other Configuration
//=========================================//{
#define DELAY_TIMER TIM6

#endif
