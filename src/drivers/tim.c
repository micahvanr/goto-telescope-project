//======================================================================================//{
//  File: tim.c
//  Overview: Provides functions for initializing, configuring, and using the TIMER peripheral.
//  Features:
//      Base time keeping (w/ interrupt capability)
//      Input capture (w/ interrupt capability)
//      Output compare (w/ interrupt capability)
//  Notes:
//      Basic Timers: TIM6 & TIM7
//      General Purpose Timers: TIM2 - TIM5
//      General Purpose Timers: TIM9 - TIM14
//      Advanced Control Timers: TIM1 & TIM8
//======================================================================================//}

//} Include Statements
//=========================================//{

#include "tim.h"
#include "assert_handler.h"
#include "nvic.h"
#include "rcc.h"

//======================================================================================//}
//                  Private Constants
//======================================================================================//{

//} Initialization Handler Constants
//=========================================//{

typedef enum : uint8_t {
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

typedef enum : uint8_t {
    TIM_NOT_INITIALIZED = 0,
    TIM_INITIALIZED     = 1,
} tim_init_check_e;

//========================================================//}
//          Register Constants
//========================================================//{

typedef enum : uint32_t {
    TIM_CR1_CKD_POS  = 8U, // Clock division
    TIM_CR1_ARPE_POS = 7U, // Auto-reload preload enable
    TIM_CR1_CMS_POS  = 5U, // Center-aligned mode selection
    TIM_CR1_DIR_POS  = 4U, // Direction
    TIM_CR1_OPM_POS  = 3U, // One-pulse mode
    TIM_CR1_URS_POS  = 2U, // Update request source
    TIM_CR1_UDIS_POS = 1U, // Update disable
    TIM_CR1_CEN_POS  = 0U, // Counter enable
} tim_cr1_pos_e;

typedef enum : uint32_t {
    TIM_CR1_CKD  = (1U << TIM_CR1_CKD_POS),  // Clock division
    TIM_CR1_ARPE = (1U << TIM_CR1_ARPE_POS), // Auto-reload preload enable
    TIM_CR1_CMS  = (1U << TIM_CR1_CMS_POS),  // Center-aligned mode selection
    TIM_CR1_DIR  = (1U << TIM_CR1_DIR_POS),  // Direction
    TIM_CR1_OPM  = (1U << TIM_CR1_OPM_POS),  // One-pulse mode
    TIM_CR1_URS  = (1U << TIM_CR1_URS_POS),  // Update request source
    TIM_CR1_UDIS = (1U << TIM_CR1_UDIS_POS), // Update disable
    TIM_CR1_CEN  = (1U << TIM_CR1_CEN_POS),  // Counter enable
} tim_cr1_e;

typedef enum : uint32_t {
    TIM_CR1_CKD_MASK  = 0b11U, // 2 bit(s)
    TIM_CR1_ARPE_MASK = 0b1U,  // 1 bit(s)
    TIM_CR1_CMS_MASK  = 0b11U, // 2 bit(s)
    TIM_CR1_DIR_MASK  = 0b1U,  // 1 bit(s)
    TIM_CR1_OPM_MASK  = 0b1U,  // 1 bit(s)
    TIM_CR1_URS_MASK  = 0b1U,  // 1 bit(s)
    TIM_CR1_UDIS_MASK = 0b1U,  // 1 bit(s)
    TIM_CR1_CEN_MASK  = 0b1U,  // 1 bit(s)
} tim_cr1_mask_e;

typedef enum : uint32_t {
    TIM_CR2_OIS4_POS  = 14U, // Output Idle state 4
    TIM_CR2_OIS3N_POS = 13U, // Output Idle state 3
    TIM_CR2_OIS3_POS  = 12U, // Output Idle state 3
    TIM_CR2_OIS2N_POS = 11U, // Output Idle state 2
    TIM_CR2_OIS2_POS  = 10U, // Output Idle state 2
    TIM_CR2_OIS1N_POS = 9U,  // Output Idle state 1
    TIM_CR2_OIS1_POS  = 8U,  // Output Idle state 1
    TIM_CR2_TI1S_POS  = 7U,  // TI1 selection
    TIM_CR2_MMS_POS   = 4U,  // Master mode selection
    TIM_CR2_CCDS_POS  = 3U,  // Capture/compare DMA selection
    TIM_CR2_CCUS_POS  = 2U,  // Capture/compare control update selection
    TIM_CR2_CCPC_POS  = 0U,  // Capture/compare preloaded control
} tim_cr2_pos_e;

typedef enum : uint32_t {
    TIM_CR2_OIS4  = (1U << TIM_CR2_OIS4_POS),  // Output Idle state 4
    TIM_CR2_OIS3N = (1U << TIM_CR2_OIS3N_POS), // Output Idle state 3
    TIM_CR2_OIS3  = (1U << TIM_CR2_OIS3_POS),  // Output Idle state 3
    TIM_CR2_OIS2N = (1U << TIM_CR2_OIS2N_POS), // Output Idle state 2
    TIM_CR2_OIS2  = (1U << TIM_CR2_OIS2_POS),  // Output Idle state 2
    TIM_CR2_OIS1N = (1U << TIM_CR2_OIS1N_POS), // Output Idle state 1
    TIM_CR2_OIS1  = (1U << TIM_CR2_OIS1_POS),  // Output Idle state 1
    TIM_CR2_TI1S  = (1U << TIM_CR2_TI1S_POS),  // TI1 selection
    TIM_CR2_MMS   = (1U << TIM_CR2_MMS_POS),   // Master mode selection
    TIM_CR2_CCDS  = (1U << TIM_CR2_CCDS_POS),  // Capture/compare DMA selection
    TIM_CR2_CCUS  = (1U << TIM_CR2_CCUS_POS),  // Capture/compare control update selection
    TIM_CR2_CCPC  = (1U << TIM_CR2_CCPC_POS),  // Capture/compare preloaded control
} tim_cr2_e;

typedef enum : uint32_t {
    TIM_CR2_OIS4_MASK  = 0b1U,   // 1 bit(s)
    TIM_CR2_OIS3N_MASK = 0b1U,   // 1 bit(s)
    TIM_CR2_OIS3_MASK  = 0b1U,   // 1 bit(s)
    TIM_CR2_OIS2N_MASK = 0b1U,   // 1 bit(s)
    TIM_CR2_OIS2_MASK  = 0b1U,   // 1 bit(s)
    TIM_CR2_OIS1N_MASK = 0b1U,   // 1 bit(s)
    TIM_CR2_OIS1_MASK  = 0b1U,   // 1 bit(s)
    TIM_CR2_TI1S_MASK  = 0b1U,   // 1 bit(s)
    TIM_CR2_MMS_MASK   = 0b111U, // 3 bit(s)
    TIM_CR2_CCDS_MASK  = 0b1U,   // 1 bit(s)
    TIM_CR2_CCUS_MASK  = 0b1U,   // 1 bit(s)
    TIM_CR2_CCPC_MASK  = 0b1U,   // 1 bit(s)
} tim_cr2_mask_e;

typedef enum : uint32_t {
    TIM_SMCR_ETP_POS  = 15U, // External trigger polarity
    TIM_SMCR_ECE_POS  = 14U, // External clock enable
    TIM_SMCR_ETPS_POS = 12U, // External trigger prescaler
    TIM_SMCR_ETF_POS  = 8U,  // External trigger filter
    TIM_SMCR_MSM_POS  = 7U,  // Master/Slave mode
    TIM_SMCR_TS_POS   = 4U,  // Trigger selection
    TIM_SMCR_SMS_POS  = 0U,  // Slave mode selection
} tim_smcr_pos_e;

typedef enum : uint32_t {
    TIM_SMCR_ETP  = (1U << TIM_SMCR_ETP_POS),  // External trigger polarity
    TIM_SMCR_ECE  = (1U << TIM_SMCR_ECE_POS),  // External clock enable
    TIM_SMCR_ETPS = (1U << TIM_SMCR_ETPS_POS), // External trigger prescaler
    TIM_SMCR_ETF  = (1U << TIM_SMCR_ETF_POS),  // External trigger filter
    TIM_SMCR_MSM  = (1U << TIM_SMCR_MSM_POS),  // Master/Slave mode
    TIM_SMCR_TS   = (1U << TIM_SMCR_TS_POS),   // Trigger selection
    TIM_SMCR_SMS  = (1U << TIM_SMCR_SMS_POS),  // Slave mode selection
} tim_smcr_e;

typedef enum : uint32_t {
    TIM_SMCR_ETP_MASK  = 0b1U,    // 1 bit(s)
    TIM_SMCR_ECE_MASK  = 0b1U,    // 1 bit(s)
    TIM_SMCR_ETPS_MASK = 0b11U,   // 2 bit(s)
    TIM_SMCR_ETF_MASK  = 0b1111U, // 4 bit(s)
    TIM_SMCR_MSM_MASK  = 0b1U,    // 1 bit(s)
    TIM_SMCR_TS_MASK   = 0b111U,  // 3 bit(s)
    TIM_SMCR_SMS_MASK  = 0b111U,  // 3 bit(s)
} tim_smcr_mask_e;

typedef enum : uint32_t {
    TIM_DIER_TDE_POS   = 14U, // Trigger DMA request enable
    TIM_DIER_COMDE_POS = 13U, // COM DMA request enable
    TIM_DIER_CC4DE_POS = 12U, // Capture/Compare 4 DMA request enable
    TIM_DIER_CC3DE_POS = 11U, // Capture/Compare 3 DMA request enable
    TIM_DIER_CC2DE_POS = 10U, // Capture/Compare 2 DMA request enable
    TIM_DIER_CC1DE_POS = 9U,  // Capture/Compare 1 DMA request enable
    TIM_DIER_UDE_POS   = 8U,  // Update DMA request enable
    TIM_DIER_TIE_POS   = 6U,  // Trigger interrupt enable
    TIM_DIER_CC4IE_POS = 4U,  // Capture/Compare 4 interrupt enable
    TIM_DIER_CC3IE_POS = 3U,  // Capture/Compare 3 interrupt enable
    TIM_DIER_CC2IE_POS = 2U,  // Capture/Compare 2 interrupt enable
    TIM_DIER_CC1IE_POS = 1U,  // Capture/Compare 1 interrupt enable
    TIM_DIER_UIE_POS   = 0U,  // Update interrupt enable
    TIM_DIER_BIE_POS   = 7U,  // Break interrupt enable
    TIM_DIER_COMIE_POS = 5U,  // COM interrupt enable
} tim_dier_pos_e;

typedef enum : uint32_t {
    TIM_DIER_TDE   = (1U << TIM_DIER_TDE_POS),   // Trigger DMA request enable
    TIM_DIER_COMDE = (1U << TIM_DIER_COMDE_POS), // COM DMA request enable
    TIM_DIER_CC4DE = (1U << TIM_DIER_CC4DE_POS), // Capture/Compare 4 DMA request enable
    TIM_DIER_CC3DE = (1U << TIM_DIER_CC3DE_POS), // Capture/Compare 3 DMA request enable
    TIM_DIER_CC2DE = (1U << TIM_DIER_CC2DE_POS), // Capture/Compare 2 DMA request enable
    TIM_DIER_CC1DE = (1U << TIM_DIER_CC1DE_POS), // Capture/Compare 1 DMA request enable
    TIM_DIER_UDE   = (1U << TIM_DIER_UDE_POS),   // Update DMA request enable
    TIM_DIER_TIE   = (1U << TIM_DIER_TIE_POS),   // Trigger interrupt enable
    TIM_DIER_CC4IE = (1U << TIM_DIER_CC4IE_POS), // Capture/Compare 4 interrupt enable
    TIM_DIER_CC3IE = (1U << TIM_DIER_CC3IE_POS), // Capture/Compare 3 interrupt enable
    TIM_DIER_CC2IE = (1U << TIM_DIER_CC2IE_POS), // Capture/Compare 2 interrupt enable
    TIM_DIER_CC1IE = (1U << TIM_DIER_CC1IE_POS), // Capture/Compare 1 interrupt enable
    TIM_DIER_UIE   = (1U << TIM_DIER_UIE_POS),   // Update interrupt enable
    TIM_DIER_BIE   = (1U << TIM_DIER_BIE_POS),   // Break interrupt enable
    TIM_DIER_COMIE = (1U << TIM_DIER_COMIE_POS), // COM interrupt enable
} tim_dier_e;

typedef enum : uint32_t {
    TIM_DIER_TDE_MASK   = 0b1U, // 1 bit(s)
    TIM_DIER_COMDE_MASK = 0b1U, // 1 bit(s)
    TIM_DIER_CC4DE_MASK = 0b1U, // 1 bit(s)
    TIM_DIER_CC3DE_MASK = 0b1U, // 1 bit(s)
    TIM_DIER_CC2DE_MASK = 0b1U, // 1 bit(s)
    TIM_DIER_CC1DE_MASK = 0b1U, // 1 bit(s)
    TIM_DIER_UDE_MASK   = 0b1U, // 1 bit(s)
    TIM_DIER_TIE_MASK   = 0b1U, // 1 bit(s)
    TIM_DIER_CC4IE_MASK = 0b1U, // 1 bit(s)
    TIM_DIER_CC3IE_MASK = 0b1U, // 1 bit(s)
    TIM_DIER_CC2IE_MASK = 0b1U, // 1 bit(s)
    TIM_DIER_CC1IE_MASK = 0b1U, // 1 bit(s)
    TIM_DIER_UIE_MASK   = 0b1U, // 1 bit(s)
    TIM_DIER_BIE_MASK   = 0b1U, // 1 bit(s)
    TIM_DIER_COMIE_MASK = 0b1U, // 1 bit(s)
} tim_dier_mask_e;

typedef enum : uint32_t {
    TIM_SR_CC4OF_POS = 12U, // Capture/Compare 4 overcapture flag
    TIM_SR_CC3OF_POS = 11U, // Capture/Compare 3 overcapture flag
    TIM_SR_CC2OF_POS = 10U, // Capture/compare 2 overcapture flag
    TIM_SR_CC1OF_POS = 9U,  // Capture/Compare 1 overcapture flag
    TIM_SR_BIF_POS   = 7U,  // Break interrupt flag
    TIM_SR_TIF_POS   = 6U,  // Trigger interrupt flag
    TIM_SR_COMIF_POS = 5U,  // COM interrupt flag
    TIM_SR_CC4IF_POS = 4U,  // Capture/Compare 4 interrupt flag
    TIM_SR_CC3IF_POS = 3U,  // Capture/Compare 3 interrupt flag
    TIM_SR_CC2IF_POS = 2U,  // Capture/Compare 2 interrupt flag
    TIM_SR_CC1IF_POS = 1U,  // Capture/compare 1 interrupt flag
    TIM_SR_UIF_POS   = 0U,  // Update interrupt flag
} tim_sr_pos_e;

typedef enum : uint32_t {
    TIM_SR_CC4OF = (1U << TIM_SR_CC4OF_POS), // Capture/Compare 4 overcapture flag
    TIM_SR_CC3OF = (1U << TIM_SR_CC3OF_POS), // Capture/Compare 3 overcapture flag
    TIM_SR_CC2OF = (1U << TIM_SR_CC2OF_POS), // Capture/compare 2 overcapture flag
    TIM_SR_CC1OF = (1U << TIM_SR_CC1OF_POS), // Capture/Compare 1 overcapture flag
    TIM_SR_BIF   = (1U << TIM_SR_BIF_POS),   // Break interrupt flag
    TIM_SR_TIF   = (1U << TIM_SR_TIF_POS),   // Trigger interrupt flag
    TIM_SR_COMIF = (1U << TIM_SR_COMIF_POS), // COM interrupt flag
    TIM_SR_CC4IF = (1U << TIM_SR_CC4IF_POS), // Capture/Compare 4 interrupt flag
    TIM_SR_CC3IF = (1U << TIM_SR_CC3IF_POS), // Capture/Compare 3 interrupt flag
    TIM_SR_CC2IF = (1U << TIM_SR_CC2IF_POS), // Capture/Compare 2 interrupt flag
    TIM_SR_CC1IF = (1U << TIM_SR_CC1IF_POS), // Capture/compare 1 interrupt flag
    TIM_SR_UIF   = (1U << TIM_SR_UIF_POS),   // Update interrupt flag
} tim_sr_e;

typedef enum : uint32_t {
    TIM_SR_CC4OF_MASK = 0b1U, // 1 bit(s)
    TIM_SR_CC3OF_MASK = 0b1U, // 1 bit(s)
    TIM_SR_CC2OF_MASK = 0b1U, // 1 bit(s)
    TIM_SR_CC1OF_MASK = 0b1U, // 1 bit(s)
    TIM_SR_BIF_MASK   = 0b1U, // 1 bit(s)
    TIM_SR_TIF_MASK   = 0b1U, // 1 bit(s)
    TIM_SR_COMIF_MASK = 0b1U, // 1 bit(s)
    TIM_SR_CC4IF_MASK = 0b1U, // 1 bit(s)
    TIM_SR_CC3IF_MASK = 0b1U, // 1 bit(s)
    TIM_SR_CC2IF_MASK = 0b1U, // 1 bit(s)
    TIM_SR_CC1IF_MASK = 0b1U, // 1 bit(s)
    TIM_SR_UIF_MASK   = 0b1U, // 1 bit(s)
} tim_sr_mask_e;

typedef enum : uint32_t {
    TIM_EGR_BG_POS   = 7U, // Break generation
    TIM_EGR_TG_POS   = 6U, // Trigger generation
    TIM_EGR_COMG_POS = 5U, // Capture/Compare control update generation
    TIM_EGR_CC4G_POS = 4U, // Capture/compare 4 generation
    TIM_EGR_CC3G_POS = 3U, // Capture/compare 3 generation
    TIM_EGR_CC2G_POS = 2U, // Capture/compare 2 generation
    TIM_EGR_CC1G_POS = 1U, // Capture/compare 1 generation
    TIM_EGR_UG_POS   = 0U, // Update generation
} tim_egr_pos_e;

typedef enum : uint32_t {
    TIM_EGR_BG   = (1U << TIM_EGR_BG_POS),   // Break generation
    TIM_EGR_TG   = (1U << TIM_EGR_TG_POS),   // Trigger generation
    TIM_EGR_COMG = (1U << TIM_EGR_COMG_POS), // Capture/Compare control update generation
    TIM_EGR_CC4G = (1U << TIM_EGR_CC4G_POS), // Capture/compare 4 generation
    TIM_EGR_CC3G = (1U << TIM_EGR_CC3G_POS), // Capture/compare 3 generation
    TIM_EGR_CC2G = (1U << TIM_EGR_CC2G_POS), // Capture/compare 2 generation
    TIM_EGR_CC1G = (1U << TIM_EGR_CC1G_POS), // Capture/compare 1 generation
    TIM_EGR_UG   = (1U << TIM_EGR_UG_POS),   // Update generation
} tim_egr_e;

typedef enum : uint32_t {
    TIM_EGR_BG_MASK   = 0b1U, // 1 bit(s)
    TIM_EGR_TG_MASK   = 0b1U, // 1 bit(s)
    TIM_EGR_COMG_MASK = 0b1U, // 1 bit(s)
    TIM_EGR_CC4G_MASK = 0b1U, // 1 bit(s)
    TIM_EGR_CC3G_MASK = 0b1U, // 1 bit(s)
    TIM_EGR_CC2G_MASK = 0b1U, // 1 bit(s)
    TIM_EGR_CC1G_MASK = 0b1U, // 1 bit(s)
    TIM_EGR_UG_MASK   = 0b1U, // 1 bit(s)
} tim_egr_mask_e;

typedef enum : uint32_t {
    TIM_CCMR1_OUTPUT_OC2CE_POS = 15U, // Output Compare 2 clear enable
    TIM_CCMR1_OUTPUT_OC2M_POS  = 12U, // Output Compare 2 mode
    TIM_CCMR1_OUTPUT_OC2PE_POS = 11U, // Output Compare 2 preload enable
    TIM_CCMR1_OUTPUT_OC2FE_POS = 10U, // Output Compare 2 fast enable
    TIM_CCMR1_OUTPUT_CC2S_POS  = 8U,  // Capture/Compare 2 selection
    TIM_CCMR1_OUTPUT_OC1CE_POS = 7U,  // Output Compare 1 clear enable
    TIM_CCMR1_OUTPUT_OC1M_POS  = 4U,  // Output Compare 1 mode
    TIM_CCMR1_OUTPUT_OC1PE_POS = 3U,  // Output Compare 1 preload enable
    TIM_CCMR1_OUTPUT_OC1FE_POS = 2U,  // Output Compare 1 fast enable
    TIM_CCMR1_OUTPUT_CC1S_POS  = 0U,  // Capture/Compare 1 selection
} tim_ccmr1_output_pos_e;

typedef enum : uint32_t {
    TIM_CCMR1_OUTPUT_OC2CE = (1U << TIM_CCMR1_OUTPUT_OC2CE_POS), // Output Compare 2 clear enable
    TIM_CCMR1_OUTPUT_OC2M  = (1U << TIM_CCMR1_OUTPUT_OC2M_POS),  // Output Compare 2 mode
    TIM_CCMR1_OUTPUT_OC2PE = (1U << TIM_CCMR1_OUTPUT_OC2PE_POS), // Output Compare 2 preload enable
    TIM_CCMR1_OUTPUT_OC2FE = (1U << TIM_CCMR1_OUTPUT_OC2FE_POS), // Output Compare 2 fast enable
    TIM_CCMR1_OUTPUT_CC2S  = (1U << TIM_CCMR1_OUTPUT_CC2S_POS),  // Capture/Compare 2 selection
    TIM_CCMR1_OUTPUT_OC1CE = (1U << TIM_CCMR1_OUTPUT_OC1CE_POS), // Output Compare 1 clear enable
    TIM_CCMR1_OUTPUT_OC1M  = (1U << TIM_CCMR1_OUTPUT_OC1M_POS),  // Output Compare 1 mode
    TIM_CCMR1_OUTPUT_OC1PE = (1U << TIM_CCMR1_OUTPUT_OC1PE_POS), // Output Compare 1 preload enable
    TIM_CCMR1_OUTPUT_OC1FE = (1U << TIM_CCMR1_OUTPUT_OC1FE_POS), // Output Compare 1 fast enable
    TIM_CCMR1_OUTPUT_CC1S  = (1U << TIM_CCMR1_OUTPUT_CC1S_POS),  // Capture/Compare 1 selection
} tim_ccmr1_output_e;

typedef enum : uint32_t {
    TIM_CCMR1_OUTPUT_OC2CE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_OC2M_MASK  = 0b111U, // 3 bit(s)
    TIM_CCMR1_OUTPUT_OC2PE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_OC2FE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_CC2S_MASK  = 0b11U,  // 2 bit(s)
    TIM_CCMR1_OUTPUT_OC1CE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_OC1M_MASK  = 0b111U, // 3 bit(s)
    TIM_CCMR1_OUTPUT_OC1PE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_OC1FE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR1_OUTPUT_CC1S_MASK  = 0b11U,  // 2 bit(s)
} tim_ccmr1_output_mask_e;

typedef enum : uint32_t {
    TIM_CCMR1_INPUT_IC2F_POS   = 12U, // Input capture 2 filter
    TIM_CCMR1_INPUT_IC2PCS_POS = 10U, // Input capture 2 prescaler
    TIM_CCMR1_INPUT_CC2S_POS   = 8U,  // Capture/Compare 2 selection
    TIM_CCMR1_INPUT_IC1F_POS   = 4U,  // Input capture 1 filter
    TIM_CCMR1_INPUT_ICPCS_POS  = 2U,  // Input capture 1 prescaler
    TIM_CCMR1_INPUT_CC1S_POS   = 0U,  // Capture/Compare 1 selection
} tim_ccmr1_input_pos_e;

typedef enum : uint32_t {
    TIM_CCMR1_INPUT_IC2F   = (1U << TIM_CCMR1_INPUT_IC2F_POS),   // Input capture 2 filter
    TIM_CCMR1_INPUT_IC2PCS = (1U << TIM_CCMR1_INPUT_IC2PCS_POS), // Input capture 2 prescaler
    TIM_CCMR1_INPUT_CC2S   = (1U << TIM_CCMR1_INPUT_CC2S_POS),   // Capture/Compare 2 selection
    TIM_CCMR1_INPUT_IC1F   = (1U << TIM_CCMR1_INPUT_IC1F_POS),   // Input capture 1 filter
    TIM_CCMR1_INPUT_ICPCS  = (1U << TIM_CCMR1_INPUT_ICPCS_POS),  // Input capture 1 prescaler
    TIM_CCMR1_INPUT_CC1S   = (1U << TIM_CCMR1_INPUT_CC1S_POS),   // Capture/Compare 1 selection
} tim_ccmr1_input_e;

typedef enum : uint32_t {
    TIM_CCMR1_INPUT_IC2F_MASK   = 0b1111U, // 4 bit(s)
    TIM_CCMR1_INPUT_IC2PCS_MASK = 0b11U,   // 2 bit(s)
    TIM_CCMR1_INPUT_CC2S_MASK   = 0b11U,   // 2 bit(s)
    TIM_CCMR1_INPUT_IC1F_MASK   = 0b1111U, // 4 bit(s)
    TIM_CCMR1_INPUT_ICPCS_MASK  = 0b11U,   // 2 bit(s)
    TIM_CCMR1_INPUT_CC1S_MASK   = 0b11U,   // 2 bit(s)
} tim_ccmr1_input_mask_e;

typedef enum : uint32_t {
    TIM_CCMR2_OUTPUT_OC4CE_POS = 15U, // Output compare 4 clear enable
    TIM_CCMR2_OUTPUT_OC4M_POS  = 12U, // Output compare 4 mode
    TIM_CCMR2_OUTPUT_OC4PE_POS = 11U, // Output compare 4 preload enable
    TIM_CCMR2_OUTPUT_OC4FE_POS = 10U, // Output compare 4 fast enable
    TIM_CCMR2_OUTPUT_CC4S_POS  = 8U,  // Capture/Compare 4 selection
    TIM_CCMR2_OUTPUT_OC3CE_POS = 7U,  // Output compare 3 clear enable
    TIM_CCMR2_OUTPUT_OC3M_POS  = 4U,  // Output compare 3 mode
    TIM_CCMR2_OUTPUT_OC3PE_POS = 3U,  // Output compare 3 preload enable
    TIM_CCMR2_OUTPUT_OC3FE_POS = 2U,  // Output compare 3 fast enable
    TIM_CCMR2_OUTPUT_CC3S_POS  = 0U,  // Capture/Compare 3 selection
} tim_ccmr2_output_pos_e;

typedef enum : uint32_t {
    TIM_CCMR2_OUTPUT_OC4CE = (1U << TIM_CCMR2_OUTPUT_OC4CE_POS), // Output compare 4 clear enable
    TIM_CCMR2_OUTPUT_OC4M  = (1U << TIM_CCMR2_OUTPUT_OC4M_POS),  // Output compare 4 mode
    TIM_CCMR2_OUTPUT_OC4PE = (1U << TIM_CCMR2_OUTPUT_OC4PE_POS), // Output compare 4 preload enable
    TIM_CCMR2_OUTPUT_OC4FE = (1U << TIM_CCMR2_OUTPUT_OC4FE_POS), // Output compare 4 fast enable
    TIM_CCMR2_OUTPUT_CC4S  = (1U << TIM_CCMR2_OUTPUT_CC4S_POS),  // Capture/Compare 4 selection
    TIM_CCMR2_OUTPUT_OC3CE = (1U << TIM_CCMR2_OUTPUT_OC3CE_POS), // Output compare 3 clear enable
    TIM_CCMR2_OUTPUT_OC3M  = (1U << TIM_CCMR2_OUTPUT_OC3M_POS),  // Output compare 3 mode
    TIM_CCMR2_OUTPUT_OC3PE = (1U << TIM_CCMR2_OUTPUT_OC3PE_POS), // Output compare 3 preload enable
    TIM_CCMR2_OUTPUT_OC3FE = (1U << TIM_CCMR2_OUTPUT_OC3FE_POS), // Output compare 3 fast enable
    TIM_CCMR2_OUTPUT_CC3S  = (1U << TIM_CCMR2_OUTPUT_CC3S_POS),  // Capture/Compare 3 selection
} tim_ccmr2_output_e;

typedef enum : uint32_t {
    TIM_CCMR2_OUTPUT_OC4CE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_OC4M_MASK  = 0b111U, // 3 bit(s)
    TIM_CCMR2_OUTPUT_OC4PE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_OC4FE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_CC4S_MASK  = 0b11U,  // 2 bit(s)
    TIM_CCMR2_OUTPUT_OC3CE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_OC3M_MASK  = 0b111U, // 3 bit(s)
    TIM_CCMR2_OUTPUT_OC3PE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_OC3FE_MASK = 0b1U,   // 1 bit(s)
    TIM_CCMR2_OUTPUT_CC3S_MASK  = 0b11U,  // 2 bit(s)
} tim_ccmr2_output_mask_e;

typedef enum : uint32_t {
    TIM_CCMR2_INPUT_IC4F_POS   = 12U, // Input capture 4 filter
    TIM_CCMR2_INPUT_IC4PSC_POS = 10U, // Input capture 4 prescaler
    TIM_CCMR2_INPUT_CC4S_POS   = 8U,  // Capture/Compare 4 selection
    TIM_CCMR2_INPUT_IC3F_POS   = 4U,  // Input capture 3 filter
    TIM_CCMR2_INPUT_IC3PSC_POS = 2U,  // Input capture 3 prescaler
    TIM_CCMR2_INPUT_CC3S_POS   = 0U,  // Capture/compare 3 selection
} tim_ccmr2_input_pos_e;

typedef enum : uint32_t {
    TIM_CCMR2_INPUT_IC4F   = (1U << TIM_CCMR2_INPUT_IC4F_POS),   // Input capture 4 filter
    TIM_CCMR2_INPUT_IC4PSC = (1U << TIM_CCMR2_INPUT_IC4PSC_POS), // Input capture 4 prescaler
    TIM_CCMR2_INPUT_CC4S   = (1U << TIM_CCMR2_INPUT_CC4S_POS),   // Capture/Compare 4 selection
    TIM_CCMR2_INPUT_IC3F   = (1U << TIM_CCMR2_INPUT_IC3F_POS),   // Input capture 3 filter
    TIM_CCMR2_INPUT_IC3PSC = (1U << TIM_CCMR2_INPUT_IC3PSC_POS), // Input capture 3 prescaler
    TIM_CCMR2_INPUT_CC3S   = (1U << TIM_CCMR2_INPUT_CC3S_POS),   // Capture/compare 3 selection
} tim_ccmr2_input_e;

typedef enum : uint32_t {
    TIM_CCMR2_INPUT_IC4F_MASK   = 0b1111U, // 4 bit(s)
    TIM_CCMR2_INPUT_IC4PSC_MASK = 0b11U,   // 2 bit(s)
    TIM_CCMR2_INPUT_CC4S_MASK   = 0b11U,   // 2 bit(s)
    TIM_CCMR2_INPUT_IC3F_MASK   = 0b1111U, // 4 bit(s)
    TIM_CCMR2_INPUT_IC3PSC_MASK = 0b11U,   // 2 bit(s)
    TIM_CCMR2_INPUT_CC3S_MASK   = 0b11U,   // 2 bit(s)
} tim_ccmr2_input_mask_e;

typedef enum : uint32_t {
    TIM_CCER_CC4NP_POS = 15U, // Capture/Compare 4 output Polarity
    TIM_CCER_CC4P_POS  = 13U, // Capture/Compare 3 output Polarity
    TIM_CCER_CC4E_POS  = 12U, // Capture/Compare 4 output enable
    TIM_CCER_CC3NP_POS = 11U, // Capture/Compare 3 output Polarity
    TIM_CCER_CC3NE_POS = 10U, // Capture/Compare 3 complementary output enable
    TIM_CCER_CC3P_POS  = 9U,  // Capture/Compare 3 output Polarity
    TIM_CCER_CC3E_POS  = 8U,  // Capture/Compare 3 output enable
    TIM_CCER_CC2NP_POS = 7U,  // Capture/Compare 2 output Polarity
    TIM_CCER_CC2NE_POS = 6U,  // Capture/Compare 2 complementary output enable
    TIM_CCER_CC2P_POS  = 5U,  // Capture/Compare 2 output Polarity
    TIM_CCER_CC2E_POS  = 4U,  // Capture/Compare 2 output enable
    TIM_CCER_CC1NP_POS = 3U,  // Capture/Compare 1 output Polarity
    TIM_CCER_CC1NE_POS = 2U,  // Capture/Compare 1 complementary output enable
    TIM_CCER_CC1P_POS  = 1U,  // Capture/Compare 1 output Polarity
    TIM_CCER_CC1E_POS  = 0U,  // Capture/Compare 1 output enable
} tim_ccer_pos_e;

typedef enum : uint32_t {
    TIM_CCER_CC4NP = (1U << TIM_CCER_CC4NP_POS), // Capture/Compare 4 output Polarity
    TIM_CCER_CC4P  = (1U << TIM_CCER_CC4P_POS),  // Capture/Compare 3 output Polarity
    TIM_CCER_CC4E  = (1U << TIM_CCER_CC4E_POS),  // Capture/Compare 4 output enable
    TIM_CCER_CC3NP = (1U << TIM_CCER_CC3NP_POS), // Capture/Compare 3 output Polarity
    TIM_CCER_CC3NE = (1U << TIM_CCER_CC3NE_POS), // Capture/Compare 3 complementary output enable
    TIM_CCER_CC3P  = (1U << TIM_CCER_CC3P_POS),  // Capture/Compare 3 output Polarity
    TIM_CCER_CC3E  = (1U << TIM_CCER_CC3E_POS),  // Capture/Compare 3 output enable
    TIM_CCER_CC2NP = (1U << TIM_CCER_CC2NP_POS), // Capture/Compare 2 output Polarity
    TIM_CCER_CC2NE = (1U << TIM_CCER_CC2NE_POS), // Capture/Compare 2 complementary output enable
    TIM_CCER_CC2P  = (1U << TIM_CCER_CC2P_POS),  // Capture/Compare 2 output Polarity
    TIM_CCER_CC2E  = (1U << TIM_CCER_CC2E_POS),  // Capture/Compare 2 output enable
    TIM_CCER_CC1NP = (1U << TIM_CCER_CC1NP_POS), // Capture/Compare 1 output Polarity
    TIM_CCER_CC1NE = (1U << TIM_CCER_CC1NE_POS), // Capture/Compare 1 complementary output enable
    TIM_CCER_CC1P  = (1U << TIM_CCER_CC1P_POS),  // Capture/Compare 1 output Polarity
    TIM_CCER_CC1E  = (1U << TIM_CCER_CC1E_POS),  // Capture/Compare 1 output enable
} tim_ccer_e;

typedef enum : uint32_t {
    TIM_CCER_CC4NP_MASK = 0b1U, // 1 bit(s)
    TIM_CCER_CC4P_MASK  = 0b1U, // 1 bit(s)
    TIM_CCER_CC4E_MASK  = 0b1U, // 1 bit(s)
    TIM_CCER_CC3NP_MASK = 0b1U, // 1 bit(s)
    TIM_CCER_CC3NE_MASK = 0b1U, // 1 bit(s)
    TIM_CCER_CC3P_MASK  = 0b1U, // 1 bit(s)
    TIM_CCER_CC3E_MASK  = 0b1U, // 1 bit(s)
    TIM_CCER_CC2NP_MASK = 0b1U, // 1 bit(s)
    TIM_CCER_CC2NE_MASK = 0b1U, // 1 bit(s)
    TIM_CCER_CC2P_MASK  = 0b1U, // 1 bit(s)
    TIM_CCER_CC2E_MASK  = 0b1U, // 1 bit(s)
    TIM_CCER_CC1NP_MASK = 0b1U, // 1 bit(s)
    TIM_CCER_CC1NE_MASK = 0b1U, // 1 bit(s)
    TIM_CCER_CC1P_MASK  = 0b1U, // 1 bit(s)
    TIM_CCER_CC1E_MASK  = 0b1U, // 1 bit(s)
} tim_ccer_mask_e;

typedef enum : uint32_t {
    TIM_CNT_CNT_H_POS = 16U, // High counter value
    TIM_CNT_CNT_L_POS = 0U,  // Low counter value
} tim_cnt_pos_e;

typedef enum : uint32_t {
    TIM_CNT_CNT_H = (1U << TIM_CNT_CNT_H_POS), // High counter value
    TIM_CNT_CNT_L = (1U << TIM_CNT_CNT_L_POS), // Low counter value
} tim_cnt_e;

typedef enum : uint32_t {
    TIM_CNT_CNT_H_MASK = 0b1111111111111111U, // 16 bit(s)
    TIM_CNT_CNT_L_MASK = 0b1111111111111111U, // 16 bit(s)
} tim_cnt_mask_e;

typedef enum : uint32_t {
    TIM_PSC_PSC_POS = 0U, // Prescaler value
} tim_psc_pos_e;

typedef enum : uint32_t {
    TIM_PSC_PSC = (1U << TIM_PSC_PSC_POS), // Prescaler value
} tim_psc_e;

typedef enum : uint32_t {
    TIM_PSC_PSC_MASK = 0b1111111111111111U, // 16 bit(s)
} tim_psc_mask_e;

typedef enum : uint32_t {
    TIM_ARR_ARR_H_POS = 16U, // High Auto-reload value
    TIM_ARR_ARR_L_POS = 0U,  // Low Auto-reload value
} tim_arr_pos_e;

typedef enum : uint32_t {
    TIM_ARR_ARR_H = (1U << TIM_ARR_ARR_H_POS), // High Auto-reload value
    TIM_ARR_ARR_L = (1U << TIM_ARR_ARR_L_POS), // Low Auto-reload value
} tim_arr_e;

typedef enum : uint32_t {
    TIM_ARR_ARR_H_MASK = 0b1111111111111111U, // 16 bit(s)
    TIM_ARR_ARR_L_MASK = 0b1111111111111111U, // 16 bit(s)
} tim_arr_mask_e;

typedef enum : uint32_t {
    TIM_CCR1_CCR1_H_POS = 16U, // High Capture/Compare 1 value
    TIM_CCR1_CCR1_L_POS = 0U,  // Low Capture/Compare 1 value
} tim_ccr1_pos_e;

typedef enum : uint32_t {
    TIM_CCR1_CCR1_H = (1U << TIM_CCR1_CCR1_H_POS), // High Capture/Compare 1 value
    TIM_CCR1_CCR1_L = (1U << TIM_CCR1_CCR1_L_POS), // Low Capture/Compare 1 value
} tim_ccr1_e;

typedef enum : uint32_t {
    TIM_CCR1_CCR1_H_MASK = 0b1111111111111111U, // 16 bit(s)
    TIM_CCR1_CCR1_L_MASK = 0b1111111111111111U, // 16 bit(s)
} tim_ccr1_mask_e;

typedef enum : uint32_t {
    TIM_CCR2_CCR2_H_POS = 16U, // High Capture/Compare 2 value
    TIM_CCR2_CCR2_L_POS = 0U,  // Low Capture/Compare 2 value
} tim_ccr2_pos_e;

typedef enum : uint32_t {
    TIM_CCR2_CCR2_H = (1U << TIM_CCR2_CCR2_H_POS), // High Capture/Compare 2 value
    TIM_CCR2_CCR2_L = (1U << TIM_CCR2_CCR2_L_POS), // Low Capture/Compare 2 value
} tim_ccr2_e;

typedef enum : uint32_t {
    TIM_CCR2_CCR2_H_MASK = 0b1111111111111111U, // 16 bit(s)
    TIM_CCR2_CCR2_L_MASK = 0b1111111111111111U, // 16 bit(s)
} tim_ccr2_mask_e;

typedef enum : uint32_t {
    TIM_CCR3_CCR3_H_POS = 16U, // High Capture/Compare value
    TIM_CCR3_CCR3_L_POS = 0U,  // Low Capture/Compare value
} tim_ccr3_pos_e;

typedef enum : uint32_t {
    TIM_CCR3_CCR3_H = (1U << TIM_CCR3_CCR3_H_POS), // High Capture/Compare value
    TIM_CCR3_CCR3_L = (1U << TIM_CCR3_CCR3_L_POS), // Low Capture/Compare value
} tim_ccr3_e;

typedef enum : uint32_t {
    TIM_CCR3_CCR3_H_MASK = 0b1111111111111111U, // 16 bit(s)
    TIM_CCR3_CCR3_L_MASK = 0b1111111111111111U, // 16 bit(s)
} tim_ccr3_mask_e;

typedef enum : uint32_t {
    TIM_CCR4_CCR4_H_POS = 16U, // High Capture/Compare value
    TIM_CCR4_CCR4_L_POS = 0U,  // Low Capture/Compare value
} tim_ccr4_pos_e;

typedef enum : uint32_t {
    TIM_CCR4_CCR4_H = (1U << TIM_CCR4_CCR4_H_POS), // High Capture/Compare value
    TIM_CCR4_CCR4_L = (1U << TIM_CCR4_CCR4_L_POS), // Low Capture/Compare value
} tim_ccr4_e;

typedef enum : uint32_t {
    TIM_CCR4_CCR4_H_MASK = 0b1111111111111111U, // 16 bit(s)
    TIM_CCR4_CCR4_L_MASK = 0b1111111111111111U, // 16 bit(s)
} tim_ccr4_mask_e;

typedef enum : uint32_t {
    TIM_DCR_DBL_POS = 8U, // DMA burst length
    TIM_DCR_DBA_POS = 0U, // DMA base address
} tim_dcr_pos_e;

typedef enum : uint32_t {
    TIM_DCR_DBL = (1U << TIM_DCR_DBL_POS), // DMA burst length
    TIM_DCR_DBA = (1U << TIM_DCR_DBA_POS), // DMA base address
} tim_dcr_e;

typedef enum : uint32_t {
    TIM_DCR_DBL_MASK = 0b11111U, // 5 bit(s)
    TIM_DCR_DBA_MASK = 0b11111U, // 5 bit(s)
} tim_dcr_mask_e;

typedef enum : uint32_t {
    TIM_DMAR_DMAB_POS = 0U, // DMA register for burst accesses
} tim_dmar_pos_e;

typedef enum : uint32_t {
    TIM_DMAR_DMAB = (1U << TIM_DMAR_DMAB_POS), // DMA register for burst accesses
} tim_dmar_e;

typedef enum : uint32_t {
    TIM_DMAR_DMAB_MASK = 0b1111111111111111U, // 16 bit(s)
} tim_dmar_mask_e;

typedef enum : uint32_t {
    TIM_OR_ITR1_RMP_POS = 10U, // Timer Input 4 remap
} tim_or_pos_e;

typedef enum : uint32_t {
    TIM_OR_ITR1_RMP = (1U << TIM_OR_ITR1_RMP_POS), // Timer Input 4 remap
} tim_or_e;

typedef enum : uint32_t {
    TIM_OR_ITR1_RMP_MASK = 0b11U, // 2 bit(s)
} tim_or_mask_e;

typedef enum : uint32_t {
    TIM_BDTR_MOE_POS  = 15U, // Main output enable
    TIM_BDTR_AOE_POS  = 14U, // Automatic output enable
    TIM_BDTR_BKP_POS  = 13U, // Break polarity
    TIM_BDTR_BKE_POS  = 12U, // Break enable
    TIM_BDTR_OSSR_POS = 11U, // Off-state selection for Run mode
    TIM_BDTR_OSSI_POS = 10U, // Off-state selection for Idle mode
    TIM_BDTR_LOCK_POS = 8U,  // Lock configuration
    TIM_BDTR_DTG_POS  = 0U,  // Dead-time generator setup
} tim_bdtr_pos_e;

typedef enum : uint32_t {
    TIM_BDTR_MOE  = (1U << TIM_BDTR_MOE_POS),  // Main output enable
    TIM_BDTR_AOE  = (1U << TIM_BDTR_AOE_POS),  // Automatic output enable
    TIM_BDTR_BKP  = (1U << TIM_BDTR_BKP_POS),  // Break polarity
    TIM_BDTR_BKE  = (1U << TIM_BDTR_BKE_POS),  // Break enable
    TIM_BDTR_OSSR = (1U << TIM_BDTR_OSSR_POS), // Off-state selection for Run mode
    TIM_BDTR_OSSI = (1U << TIM_BDTR_OSSI_POS), // Off-state selection for Idle mode
    TIM_BDTR_LOCK = (1U << TIM_BDTR_LOCK_POS), // Lock configuration
    TIM_BDTR_DTG  = (1U << TIM_BDTR_DTG_POS),  // Dead-time generator setup
} tim_bdtr_e;

typedef enum : uint32_t {
    TIM_BDTR_MOE_MASK  = 0b1U,        // 1 bit(s)
    TIM_BDTR_AOE_MASK  = 0b1U,        // 1 bit(s)
    TIM_BDTR_BKP_MASK  = 0b1U,        // 1 bit(s)
    TIM_BDTR_BKE_MASK  = 0b1U,        // 1 bit(s)
    TIM_BDTR_OSSR_MASK = 0b1U,        // 1 bit(s)
    TIM_BDTR_OSSI_MASK = 0b1U,        // 1 bit(s)
    TIM_BDTR_LOCK_MASK = 0b11U,       // 2 bit(s)
    TIM_BDTR_DTG_MASK  = 0b11111111U, // 8 bit(s)
} tim_bdtr_mask_e;

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

uint16_t g_tim_peri_init                       = 0;
uint8_t g_tim_channel_init[TIM_MAX_NUM_TIMERS] = {0};

//======================================================================================//}
//                  Private Function Prototypes
//======================================================================================//{

// Assert helper functions
static void assert_tim_handle_param(tim_handle const *const p_tim_handle);
static void assert_tim_ic_config_param(tim_reg_def const *const p_timx, tim_ic_config const ic_config);
static void assert_tim_oc_config_param(tim_reg_def const *const p_timx, tim_oc_config const oc_config);
static void assert_tim_peri_param(tim_reg_def const *const p_timx);

// General helper functions
static void set_prescaler_count(tim_reg_def *const p_timx, uint32_t time, tim_unit_of_time_e unit);
static inline bool check_overflow(tim_reg_def const *const p_timx, uint32_t check_num, uint32_t factor);
static inline uint32_t get_ccr_pwm(tim_reg_def const *const p_timx, uint32_t duty_cycle);
static tim_type_e get_tim_type(tim_reg_def const *const p_timx);

static inline uint8_t map_tim_peri_to_num(tim_reg_def const *const p_timx);
static inline void set_tim_init_status(tim_reg_def const *const p_timx);
static inline tim_init_check_e get_tim_init_status(tim_reg_def const *const p_timx);
static inline void set_tim_channel_init_status(tim_reg_def const *const p_timx, tim_channel_sel_e channel);
static inline tim_init_check_e get_tim_channel_init_status(tim_reg_def const *const p_timx, tim_channel_sel_e channel);

//======================================================================================//}
//                  Public Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: tim_delay 
//  Overview: Create a delay for a given amount of time.
//  Parameters: 
//      time: Time for delay
//      unit: Unit of time
//          TIM_UNIT_S  
//          TIM_UNIT_MS 
//          TIM_UNIT_US 
//  Return: 
//      None
//  Note: Care should be taken when using us unit as the delay can be too short and 
//  the software may not be able to keep up.
//============================================================================//}
void tim_delay(uint32_t const period, tim_unit_of_time_e const unit)
{
    static uint8_t initialized = false;
    tim_handle delay_handler   = {0};

    delay_handler.p_timx = DELAY_TIMER;
    if (initialized == false)
    {
        delay_handler.tim_conf.preload        = TIM_ARR_PRELOAD_EN;
        delay_handler.tim_conf.one_pulse_mode = TIM_ONE_PULSE_MODE_DI;
        delay_handler.tim_conf.clock_sel      = TIM_CLK_SEL_INTERNAL;

        delay_handler.timing_conf.time = period;
        delay_handler.timing_conf.unit = unit;

        tim_init(&delay_handler);
        initialized = true;
    }

    tim_start(delay_handler.p_timx);

    while (tim_read_status(delay_handler.p_timx) != TIM_UPDATE_FOUND);
    tim_reset_status(delay_handler.p_timx);
}

//============================================================================//{
//  Function: tim_init
//  Overview: Initialize the specified TIMER periperal according to settings in handle.
//  Parameters:
//      p_tim_handle: Structure with the corresponding settings to configure the TIMER peripheral.
//  Return:
//      None
//  Note:
//============================================================================//}
void tim_init(tim_handle *const p_tim_handle)
{
    tim_type_e timer_type;
    uint32_t temp_cr1  = p_tim_handle->p_timx->CR1;
    uint32_t temp_smcr = p_tim_handle->p_timx->SMCR;

    assert_tim_handle_param(p_tim_handle);

    set_tim_init_status(p_tim_handle->p_timx);

    timer_type = get_tim_type(p_tim_handle->p_timx);

    rcc_tim_togglable_clock(p_tim_handle->p_timx, ENABLE);

    // One pulse mode
    temp_cr1 &= ~(TIM_CR1_OPM);
    temp_cr1 |= p_tim_handle->tim_conf.one_pulse_mode << TIM_CR1_OPM_POS;

    if ((timer_type == TIM_TYPE_ADVANCED) || (timer_type == TIM_TYPE_GENERAL_2_5))
    {
        // Set direction
        temp_cr1 &= ~(TIM_CR1_DIR);
        temp_cr1 |= p_tim_handle->tim_conf.direction << TIM_CR1_DIR_POS;
        // Set align mode
        temp_cr1 &= ~(TIM_CR1_CMS_MASK << TIM_CR1_DIR_POS);
        temp_cr1 |= p_tim_handle->tim_conf.align_mode << TIM_CR1_CMS_POS;
    }

    if ((timer_type == TIM_TYPE_ADVANCED) || (timer_type == TIM_TYPE_GENERAL_2_5)
        || (timer_type == TIM_TYPE_GENERAL_9_14))
    {
        temp_smcr &= ~(TIM_SMCR_SMS_MASK << TIM_SMCR_SMS_POS);
        temp_smcr |= p_tim_handle->tim_conf.clock_sel << TIM_SMCR_SMS_POS;
        if (p_tim_handle->tim_conf.clock_sel == TIM_CLK_SEL_EXTERNAL)
        {
            temp_smcr &= ~(TIM_SMCR_TS_MASK << TIM_SMCR_TS_POS);
            temp_smcr |= p_tim_handle->tim_conf.trigger_sel << TIM_SMCR_TS_POS;
        }
    }

    // Set prescaler and auto reload value.
    // Use manual ARR and PSC.
    if ((p_tim_handle->timing_conf.auto_reload != 0) && (p_tim_handle->timing_conf.prescaler != 0))
    {
        p_tim_handle->p_timx->PSC = p_tim_handle->timing_conf.prescaler - 1;
        p_tim_handle->p_timx->ARR = p_tim_handle->timing_conf.auto_reload - 1;

        // Calculate ARR and PSC from given time and unit.
    }
    else if ((p_tim_handle->timing_conf.time != 0) && (p_tim_handle->timing_conf.unit != 0))
    {
        set_prescaler_count(p_tim_handle->p_timx, p_tim_handle->timing_conf.time, p_tim_handle->timing_conf.unit);
    }
    // Set ARR preload
    // (if preload is off, the count value will count to this new value immediately)
    // (if preload is on, the count value will count to the new value after finishing what its counting to now)
    // This value is set after the prescaler and counter are set and only if a value has been set.
    // otherwise a garbage value will be set in the PSC register.
    if ((p_tim_handle->p_timx->PSC != 0) && (p_tim_handle->p_timx->ARR != 0))
    {
        temp_cr1 &= ~(TIM_CR1_ARPE);
        temp_cr1 |= p_tim_handle->tim_conf.preload << TIM_CR1_ARPE_POS;
    }

    p_tim_handle->p_timx->CR1  = temp_cr1;
    p_tim_handle->p_timx->SMCR = temp_smcr;
}

// Check each setting of the handle and ensure it is one of the available enum values.
static void assert_tim_handle_param(tim_handle const *const p_tim_handle)
{
    // clang-format off
    uint8_t found_setting = false;

    assert_tim_peri_param(p_tim_handle->p_timx);

    found_setting = false;
    switch (p_tim_handle->tim_conf.preload) {
    case TIM_ARR_PRELOAD_DI: found_setting = true; break;
    case TIM_ARR_PRELOAD_EN: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handle->tim_conf.one_pulse_mode) {
    case TIM_ONE_PULSE_MODE_DI: found_setting = true; break;
    case TIM_ONE_PULSE_MODE_EN: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handle->tim_conf.clock_sel) {
    case TIM_CLK_SEL_INTERNAL:         found_setting = true; break;
    case TIM_CLK_SEL_INTERNAL_TRIGGER: found_setting = true; break;
    case TIM_CLK_SEL_EXTERNAL:         found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handle->tim_conf.trigger_sel) {
    case TIM_TRIGGER_SEL_ITR0:    found_setting = true; break;
    case TIM_TRIGGER_SEL_ITR1:    found_setting = true; break;
    case TIM_TRIGGER_SEL_ITR2:    found_setting = true; break;
    case TIM_TRIGGER_SEL_ITR3:    found_setting = true; break;
    case TIM_TRIGGER_SEL_TI1F_ED: found_setting = true; break;
    case TIM_TRIGGER_SEL_TI1FP1:  found_setting = true; break;
    case TIM_TRIGGER_SEL_TI1FP2:  found_setting = true; break;
    case TIM_TRIGGER_SEL_ETRF:    found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handle->tim_conf.direction) {
    case TIM_CNT_DIR_UP:   found_setting = true; break;
    case TIM_CNT_DIR_DOWN: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handle->tim_conf.align_mode) {
    case TIM_ALIGN_EDGE:              found_setting = true; break;
    case TIM_ALIGN_CENTER_IT_DOWN:    found_setting = true; break;
    case TIM_ALIGN_CENTER_IT_UP:      found_setting = true; break;
    case TIM_ALIGN_CENTER_IT_DOWN_UP: found_setting = true; break;
    }
    ASSERT(found_setting);
    // clang-format on
}

//============================================================================//{
//  Function: tim_reset
//  Overview: Reset the specified timer.
//  Parameters:
//      p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//  Return:
//      None
//  Note: None
//============================================================================//}
void tim_reset(tim_reg_def const *const p_timx)
{
    rcc_tim_togglable_clock(p_timx, DISABLE);
}

//============================================================================//{
// Function: tim_start
// Overview: Start the counter on the given timer.
// Parameters:
//      p_TIMx: TIMER register structure macro.
//         TIMx (1-14)
// Return:
//     None
// Note: None
//============================================================================//}
void tim_start(tim_reg_def *const p_timx)
{
    if (get_tim_init_status(p_timx) == TIM_NOT_INITIALIZED)
    {
        ASSERT(false);
    }
    p_timx->CR1 |= TIM_CR1_CEN;
}

//============================================================================//{
// Function: tim_start_it
// Overview: Starts the counter on the specified timer with interrupts enabled.
// Parameters:
//      p_TIMx: TIMER register structure macro.
//         TIMx (1-14)
// Return:
//     None
// Note: None
//============================================================================//}
void tim_start_it(tim_reg_def *const p_timx)
{
    if (get_tim_init_status(p_timx) == TIM_NOT_INITIALIZED)
    {
        ASSERT(false);
    }
    p_timx->DIER |= TIM_DIER_UIE;
    tim_config_it(p_timx, TIM_CHANNEL_NA, ENABLE);
    p_timx->CR1 |= TIM_CR1_CEN;
}

//============================================================================//{
//  Function: tim_channel_start
//  Overview: Enable the counter on the specified timer.
//  Parameters:
//      p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//  Return:
//      None
//  Note: None
//============================================================================//}
void tim_channel_start(tim_reg_def *const p_timx, tim_channel_sel_e const channel)
{
    uint8_t const ENABLE_REG_SHIFT = (channel * 4);

    if (get_tim_channel_init_status(p_timx, channel) == TIM_NOT_INITIALIZED)
    {
        ASSERT(false);
    }

    p_timx->CCER |= 1 << ENABLE_REG_SHIFT;
}

//============================================================================//{
//  Function: tim_channel_start
//  Overview: Enable the counter on the specified timer.
//  Parameters:
//      p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//  Return:
//      None
//  Note: None
//============================================================================//}
void tim_channel_start_it(tim_reg_def *const p_timx, tim_channel_sel_e const channel)
{
    uint8_t const ENABLE_REG_SHIFT = (channel * 4);

    if (get_tim_channel_init_status(p_timx, channel) == TIM_NOT_INITIALIZED)
    {
        ASSERT(false);
    }

    tim_config_it(p_timx, channel, ENABLE);
    p_timx->DIER |= TIM_DIER_CC1IE << channel;
    p_timx->CCER |= 1 << ENABLE_REG_SHIFT;
}

//============================================================================//{
//  Function: tim_config_it
//  Overview: Enable or disable the specified TIMER peripheral and/or channel.
//  Parameters:
//      p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//      channel: Channel to be configured.
//          TIM_CHANNEL_SEL_x (1-4)
//          TIM_CHANNEL_NA
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void tim_config_it(tim_reg_def const *const p_timx, tim_channel_sel_e const channel, togglable_e const toggle)
{
    if ((channel != TIM_CHANNEL_NA) && (p_timx == TIM1))
    {
        nvic_toggable_irq(TIM1_CC_IRQ_NUM_27, toggle);
        return;
    }
    else if ((channel != TIM_CHANNEL_NA) && (p_timx == TIM8))
    {
        nvic_toggable_irq(TIM8_CC_IRQ_NUM_46, toggle);
        return;
    }

    if (p_timx == TIM9)
    {
        nvic_toggable_irq(TIM1_BRK_TIM9_IRQ_NUM_24, toggle);
    }
    else if ((p_timx == TIM1) || (p_timx == TIM10))
    {
        nvic_toggable_irq(TIM1_UP_TIM10_IRQ_NUM_25, toggle);
    }
    else if (p_timx == TIM11)
    {
        nvic_toggable_irq(TIM1_TRG_COM_TIM11_IRQ_NUM_26, toggle);
    }
    else if (p_timx == TIM2)
    {
        nvic_toggable_irq(TIM2_IRQ_NUM_28, toggle);
    }
    else if (p_timx == TIM3)
    {
        nvic_toggable_irq(TIM3_IRQ_NUM_29, toggle);
    }
    else if (p_timx == TIM4)
    {
        nvic_toggable_irq(TIM4_IRQ_NUM_30, toggle);
    }
    else if (p_timx == TIM12)
    {
        nvic_toggable_irq(TIM8_BRK_TIM12_IRQ_NUM_43, toggle);
    }
    else if ((p_timx == TIM8) || (p_timx == TIM13))
    {
        nvic_toggable_irq(TIM8_UP_TIM13_IRQ_NUM_44, toggle);
    }
    else if (p_timx == TIM14)
    {
        nvic_toggable_irq(TIM8_TRG_COM_TIM14_IRQ_NUM_45, toggle);
    }
    else if (p_timx == TIM7)
    {
        nvic_toggable_irq(TIM7_IRQ_NUM_55, toggle);
    }
}

//============================================================================//{
// Function: tim_ic_init
// Overview: Initializes the TIMER input capture with the settings in the configuration structure.
// Parameters:
//      p_TIMx: TIMER register structure macro.
//         TIMx (1-14)
//     ic_config: Input capture configuration settings.
// Return:
//     None
// Note: None
//============================================================================//}
void tim_ic_init(tim_reg_def *const p_timx, tim_ic_config const ic_config)
{
    // Either % the channel num and start from 1 or dont and start channel num from 0.
    uint8_t const MODE_REG         = ic_config.channel_num / 2;
    uint8_t const MODE_REG_SHIFT   = (ic_config.channel_num % 2) * 8;
    uint8_t const ENABLE_REG_SHIFT = (ic_config.channel_num * 4);

    assert_tim_ic_config_param(p_timx, ic_config);

    set_tim_channel_init_status(p_timx, ic_config.channel_num);

    // Select active input
    p_timx->CCMR[MODE_REG] |= ic_config.input_map << MODE_REG_SHIFT;

    // Program filter
    p_timx->CCMR[MODE_REG] |= ic_config.filter << MODE_REG_SHIFT;

    // Select edge

    p_timx->CCER |= (ic_config.edge_detection << ENABLE_REG_SHIFT) << TIM_CCER_CC1P_POS;

    // Program input prescaler
    p_timx->CCMR[MODE_REG] |= (ic_config.prescaler << MODE_REG_SHIFT) << TIM_CCMR1_INPUT_ICPCS_POS;

    // Enable capture
    p_timx->CCER |= (1 << ENABLE_REG_SHIFT) << TIM_CCER_CC1E_POS;
}

// Check each setting of the config and ensure it is one of the available enum values.
static void assert_tim_ic_config_param(tim_reg_def const *const p_timx, tim_ic_config const ic_config)
{
    // clang-format off
    uint8_t found_setting;

    assert_tim_peri_param(p_timx);

    found_setting = false;
    switch (ic_config.channel_num) {
    case TIM_CHANNEL_SEL_1: found_setting = true; break;
    case TIM_CHANNEL_SEL_2: found_setting = true; break;
    case TIM_CHANNEL_SEL_3: found_setting = true; break;
    case TIM_CHANNEL_SEL_4: found_setting = true; break;
    case TIM_CHANNEL_NA:    found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (ic_config.dts_mult) {
    case TIM_DTS_FREQ_CLK_INT:        found_setting = true; break;
    case TIM_DTS_FREQ_CLK_INT_MULT_2: found_setting = true; break;
    case TIM_DTS_FREQ_CLK_INT_MULT_4: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (ic_config.input_map) {
    case TIM_INPUT_MAP_MAIN:         found_setting = true; break;
    case TIM_INPUT_MAP_TI_ALTERNATE: found_setting = true; break;
    case TIM_INPUT_MAP_TRC:          found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (ic_config.prescaler) {
    case TIM_IC_PRESCALER_NA:   found_setting = true; break;
    case TIM_IC_PRESCALER_2_EV: found_setting = true; break;
    case TIM_IC_PRESCALER_4_EV: found_setting = true; break;
    case TIM_IC_PRESCALER_8_EV: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (ic_config.filter) {
    case TIM_IC_FILTER_NA:            found_setting = true; break;
    case TIM_IC_FILTER_FCLKINT_N2:    found_setting = true; break;
    case TIM_IC_FILTER_FCLKINT_N4:    found_setting = true; break;
    case TIM_IC_FILTER_FCLKINT_N8:    found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV2_N6:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV2_N8:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV4_N6:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV4_N8:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV8_N6:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV8_N8:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV16_N5: found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV16_N6: found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV16_N8: found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV32_N5: found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV32_N6: found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV32_N8: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (ic_config.edge_detection) {
    case TIM_IC_EDGE_DETECTION_RISING:         found_setting = true; break;
    case TIM_IC_EDGE_DETECTION_FALLING:        found_setting = true; break;
    case TIM_IC_EDGE_DETECTION_RISING_FALLING: found_setting = true; break;
    }
    ASSERT(found_setting);
    // clang-format on
}

//============================================================================//{
//  Function: tim_oc_init
//  Overview: Initialize the TIMER output compare according to the settings in the config structure.
//  Parameters:
//      p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//      oc_config: Output compare configuration settings.
//  Return:
//      None
//  Note: None
//============================================================================//}
void tim_oc_init(tim_reg_def *const p_timx, tim_oc_config const oc_config)
{
    // Either % the channel num and start from 1 or dont and start channel num from 0.
    uint8_t const MODE_REG         = oc_config.channel_num / 2;
    uint8_t const MODE_REG_SHIFT   = (oc_config.channel_num % 2) * 8;
    uint8_t const ENABLE_REG_SHIFT = (oc_config.channel_num * 4);

    assert_tim_oc_config_param(p_timx, oc_config);

    set_tim_channel_init_status(p_timx, oc_config.channel_num);

    // Ensure channel in output mode.
    p_timx->CCMR[MODE_REG] &= ~(TIM_CCMR1_OUTPUT_CC1S_MASK << MODE_REG_SHIFT);

    // Select oc mode
    p_timx->CCMR[MODE_REG] |= (oc_config.output_mode << MODE_REG_SHIFT) << TIM_CCMR1_OUTPUT_OC1M_POS;

    // Select output polarity
    p_timx->CCER |= (oc_config.polarity << ENABLE_REG_SHIFT) << TIM_CCER_CC1P_POS;

    // Select preload
    p_timx->CCMR[MODE_REG] |= (oc_config.arr_preload << MODE_REG_SHIFT) << TIM_CCMR1_OUTPUT_OC1PE_POS;

    // Set the CCR value manually.
    if (oc_config.ccr_value != 0)
    {
        p_timx->CCR[oc_config.channel_num] = oc_config.ccr_value;
    }
    // Set CCR value based on PWM duty cycle.
    else if (oc_config.duty_cycle != 0)
    {
        p_timx->CCR[oc_config.channel_num] = get_ccr_pwm(p_timx, oc_config.duty_cycle);
    }

    // Enable output
    p_timx->BDTR |= TIM_BDTR_MOE;
}

// Check each setting of the config and ensure it is one of the available enum values.
static void assert_tim_oc_config_param(tim_reg_def const *const p_timx, tim_oc_config const oc_config)
{
    // clang-format off
    uint8_t found_setting;
    assert_tim_peri_param(p_timx);

    found_setting = false;
    switch (oc_config.channel_num) {
    case TIM_CHANNEL_SEL_1: found_setting = true; break;
    case TIM_CHANNEL_SEL_2: found_setting = true; break;
    case TIM_CHANNEL_SEL_3: found_setting = true; break;
    case TIM_CHANNEL_SEL_4: found_setting = true; break;
    case TIM_CHANNEL_NA:    found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (oc_config.output_mode) {
    case TIM_OC_OPM_FROZEN:         found_setting = true; break;
    case TIM_OC_OPM_EQ_ACTIVE:      found_setting = true; break;
    case TIM_OC_OPM_EQ_INACTIVE:    found_setting = true; break;
    case TIM_OC_OPM_TOGGLE:         found_setting = true; break;
    case TIM_OC_OPM_FORCE_INACTIVE: found_setting = true; break;
    case TIM_OC_OPM_FORCE_ACTIVE:   found_setting = true; break;
    case TIM_OC_OPM_PWM_1:          found_setting = true; break;
    case TIM_OC_OPM_PWM_2:          found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (oc_config.polarity) {
    case TIM_OC_POL_ACTIVE_HIGH: found_setting = true; break;
    case TIM_OC_POL_ACTIVE_LOW:  found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (oc_config.arr_preload) {
    case TIM_ARR_PRELOAD_DI: found_setting = true; break;
    case TIM_ARR_PRELOAD_EN: found_setting = true; break;
    }
    ASSERT(found_setting);
    // clang-format on
}

//============================================================================//{
//  Function: tim_read_status
//  Overview: Get the specified timer's status update flag.
//  Parameters:
//      p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//  Return:
//      tim_status_e: Status of timer:
//      TIM_UPDATE_NA
//      TIM_UPDATE_FOUND
//  Note: None
//============================================================================//}
tim_status_e tim_read_status(tim_reg_def const *const p_timx)
{
    return (tim_status_e) (p_timx->SR & TIM_SR_UIF);
}

//============================================================================//{
//  Function: tim_reset_status
//  Overview: Reset the specified timer's status update flag.
//  Parameters:
//      p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//  Return:
//      None
//  Note: None
//============================================================================//}
void tim_reset_status(tim_reg_def *const p_timx)
{
    p_timx->SR &= ~TIM_SR_UIF;
}

//============================================================================//{
//  Function: tim_read_channel_status
//  Overview: Get the specified timer's status update flag.
//  Parameters:
//      p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//  Return:
//      tim_status_e: Status of timer:
//      TIM_UPDATE_NA
//      TIM_UPDATE_FOUND
//  Note: None
//============================================================================//}
tim_status_e tim_read_channel_status(tim_reg_def const *const p_timx, tim_channel_sel_e const channel)
{
    return (tim_status_e) ((p_timx->SR & (TIM_SR_CC1IF << channel)) >> TIM_SR_CC1IF_POS);
}

//============================================================================//{
//  Function: tim_reset_status
//  Overview: Reset the specified channel's status update flag.
//  Parameters:
//      p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//      channel: Channel to be configured.
//          TIM_CHANNEL_SEL_x (1-4)
//          TIM_CHANNEL_NA
//  Return:
//      None
//  Note: None
//============================================================================//}
void tim_reset_channel_status(tim_reg_def *const p_timx, tim_channel_sel_e const channel)
{
    p_timx->SR &= ~(TIM_SR_CC1IF << channel);
}

//============================================================================//{
//  Function: tim_get_channel_ccr
//  Overview: Get the specified TIMER's channel capture/compare value.
//  Parameters:
//       p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//      channel: Channel to be configured.
//          TIM_CHANNEL_SEL_x (1-4)
//          TIM_CHANNEL_NA
//  Return:
//      uint32_t: CCR value of channel.
//  Note: None
//============================================================================//}
uint32_t tim_get_channel_ccr(tim_reg_def *const p_timx, tim_channel_sel_e const channel)
{
    return p_timx->CCR[channel];
}

//============================================================================//{
//  Function: tim_get_bus
//  Overview: Get the specified timer's bus.
//  Parameters:
//      p_TIMx: TIMER register structure macro.
//          TIMx (1-14)
//  Return:
//      bus_types:
//          AHB1_BUS
//          AHB2_BUS
//          APB1_BUS
//          APB2_BUS
//  Note: None
//============================================================================//}
bus_types tim_get_bus(tim_reg_def const *const p_timx)
{
    // If TIMER peripheral on APB2
    if ((p_timx == TIM1) || (p_timx == TIM8) || (p_timx == TIM9) || (p_timx == TIM10) || (p_timx == TIM11))
    {
        return APB2_BUS;

        // If TIMER peripheral on APB1
    }
    else
    {
        return APB1_BUS;
    }
}

//======================================================================================//}
//                  Private Function Implementation
//======================================================================================//{

static void set_prescaler_count(tim_reg_def *const p_timx, uint32_t time, tim_unit_of_time_e unit)
{

    uint32_t found_setting = false;
    switch (unit)
    {
        case TIM_UNIT_S:
            found_setting = true;
            break;
        case TIM_UNIT_MS:
            found_setting = true;
            break;
        case TIM_UNIT_US:
            found_setting = true;
            break;
        case TIM_UNIT_HZ:
            found_setting = true;
            break;
        case TIM_UNIT_KHZ:
            found_setting = true;
            break;
        case TIM_UNIT_MHZ:
            found_setting = true;
            break;
    }
    ASSERT(found_setting);

    uint32_t clk_freq; // Represents clock frequency of what the clock would be according to new prescaler.
    uint32_t prescaler       = 1;
    uint32_t count           = 1;
    uint32_t unit_in_seconds = 0;
    uint32_t period;

    uint8_t const FACTOR_5 = 5;
    uint8_t const FACTOR_3 = 3;
    uint8_t const FACTOR_2 = 2;

    switch (unit)
    {
        case TIM_UNIT_MHZ:
            time *= 1000;
            [[fallthrough]];
        case TIM_UNIT_KHZ:
            time *= 1000;
            [[fallthrough]];
        case TIM_UNIT_HZ:
            period = 1000000 / time;
            unit   = TIM_UNIT_US;
            break;
        default:
            period = time;
            break;
    }

    switch (unit)
    {
        case TIM_UNIT_S:
            unit_in_seconds = 1;
            break;
        case TIM_UNIT_MS:
            unit_in_seconds = 1000;
            break;
        case TIM_UNIT_US:
            unit_in_seconds = 1000000;
            break;
        default:
            ASSERT(false);
            break;
    }

    clk_freq = rcc_get_timer_clock_freq_hz(tim_get_bus(p_timx));

    // Finding the highest prescaler value:
    // Increase the prescaler value until one of the following is true.
    // - Dividing clock by new prescaler value results in decimal number.
    // - The prescaler value is too large for 16 bits (32 bits for TIM2/TIM5).
    // - The new count value is 0.
    // This series of operations will result in a high prescaler value while maintaining accuracy.

    while (((clk_freq % FACTOR_5) == 0) && (((clk_freq / FACTOR_5) % unit_in_seconds) == 0)
           && !check_overflow(p_timx, prescaler, FACTOR_5))
    {
        clk_freq /= FACTOR_5;
        prescaler *= FACTOR_5;

        count = clk_freq / unit_in_seconds;
        count *= period;
    }

    while (((clk_freq % FACTOR_3) == 0) && (((clk_freq / FACTOR_3) % unit_in_seconds) == 0)
           && !check_overflow(p_timx, prescaler, FACTOR_3))
    {
        clk_freq /= FACTOR_3;
        prescaler *= FACTOR_3;

        count = clk_freq / unit_in_seconds;
        count *= period;
    }

    while (((clk_freq % FACTOR_2) == 0) && (((clk_freq / FACTOR_2) % unit_in_seconds) == 0)
           && !check_overflow(p_timx, prescaler, FACTOR_2)
           && (((clk_freq / (FACTOR_2 * unit_in_seconds)) * period) > 1))
    {
        clk_freq /= FACTOR_2;
        prescaler *= FACTOR_2;

        count = clk_freq / unit_in_seconds;
        count *= period;
    }

    count -= 1;
    prescaler -= 1;

    p_timx->PSC = prescaler;
    p_timx->ARR = count;
}

// Checks multiplication against an unsigned 16 bit variable.
// Returns true if there is an overflow.
static inline bool check_overflow(tim_reg_def const *const p_timx, uint32_t const check_num, uint32_t const factor)
{
    uint32_t max;
    if ((p_timx == TIM2) || (p_timx == TIM5))
    {
        max = UINT32_MAX;
    }
    else
    {
        max = UINT16_MAX;
    }
    if (check_num < (max / factor))
    {
        return false;
    }
    else
    {
        return true;
    }
}

static inline uint32_t get_ccr_pwm(tim_reg_def const *const p_timx, uint32_t const duty_cycle)
{
    uint32_t arr_value = p_timx->ARR;
    uint32_t ccr;
    if ((arr_value + 1) < 100)
    {
        ASSERT(false);
    }
    ccr = ((arr_value + 1) / 100) * duty_cycle;
    return ccr;
}

static tim_type_e get_tim_type(tim_reg_def const *const p_timx)
{
    if ((p_timx == TIM6) || (p_timx == TIM7))
    {
        return TIM_TYPE_BASIC;
    }
    else if ((p_timx == TIM2) || (p_timx == TIM3) || (p_timx == TIM4) || (p_timx == TIM5))
    {
        return TIM_TYPE_GENERAL_2_5;
    }
    else if ((p_timx == TIM9) || (p_timx == TIM10) || (p_timx == TIM11) || (p_timx == TIM12) || (p_timx == TIM13)
             || (p_timx == TIM14))
    {
        return TIM_TYPE_GENERAL_9_14;
    }
    else if ((p_timx == TIM1) || (p_timx == TIM8))
    {
        return TIM_TYPE_ADVANCED;
    }
    return 0;
}

static void assert_tim_peri_param(tim_reg_def const *const p_timx)
{
    // clang-format off
    uint8_t found_setting = false;

    // Peripheral check
    found_setting = (p_timx == TIM1)  ? true
                  : (p_timx == TIM2)  ? true
                  : (p_timx == TIM3)  ? true
                  : (p_timx == TIM4)  ? true
                  : (p_timx == TIM5)  ? true
                  : (p_timx == TIM6)  ? true
                  : (p_timx == TIM7)  ? true
                  : (p_timx == TIM8)  ? true
                  : (p_timx == TIM9)  ? true
                  : (p_timx == TIM10) ? true
                  : (p_timx == TIM11) ? true
                  : (p_timx == TIM12) ? true
                  : (p_timx == TIM13) ? true
                  : (p_timx == TIM14) ? true
                                      : false;
    ASSERT(found_setting);
    // clang-format on
}

static inline uint8_t map_tim_peri_to_num(tim_reg_def const *const p_timx)
{
    if (p_timx == TIM1)
    {
        return TIM1_INIT_NUM;
    }
    else if (p_timx == TIM2)
    {
        return TIM2_INIT_NUM;
    }
    else if (p_timx == TIM3)
    {
        return TIM3_INIT_NUM;
    }
    else if (p_timx == TIM4)
    {
        return TIM4_INIT_NUM;
    }
    else if (p_timx == TIM5)
    {
        return TIM5_INIT_NUM;
    }
    else if (p_timx == TIM6)
    {
        return TIM6_INIT_NUM;
    }
    else if (p_timx == TIM7)
    {
        return TIM7_INIT_NUM;
    }
    else if (p_timx == TIM8)
    {
        return TIM8_INIT_NUM;
    }
    else if (p_timx == TIM9)
    {
        return TIM9_INIT_NUM;
    }
    else if (p_timx == TIM10)
    {
        return TIM10_INIT_NUM;
    }
    else if (p_timx == TIM11)
    {
        return TIM11_INIT_NUM;
    }
    else if (p_timx == TIM12)
    {
        return TIM12_INIT_NUM;
    }
    else if (p_timx == TIM13)
    {
        return TIM13_INIT_NUM;
    }
    else if (p_timx == TIM14)
    {
        return TIM14_INIT_NUM;
    }
    else
    {
        ASSERT(0);
    }
    return 0;
}

static inline void set_tim_init_status(tim_reg_def const *const p_timx)
{
    g_tim_peri_init |= (1 << map_tim_peri_to_num(p_timx));
}

static inline tim_init_check_e get_tim_init_status(tim_reg_def const *const p_timx)
{
    return 0b1 & (g_tim_peri_init >> map_tim_peri_to_num(p_timx));
}

static inline void set_tim_channel_init_status(tim_reg_def const *const p_timx, tim_channel_sel_e const channel)
{
    g_tim_channel_init[map_tim_peri_to_num(p_timx)] |= (1 << channel);
}

static inline tim_init_check_e get_tim_channel_init_status(tim_reg_def const *const p_timx,
                                                           tim_channel_sel_e const channel)
{
    return 0b1 & (g_tim_channel_init[map_tim_peri_to_num(p_timx)] >> channel);
}

// *End of file*
