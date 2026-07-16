#ifndef TIMER_H
#define TIMER_H

#include "stm32f4xx.h"

// General Purpose Timers: TIM2 - TIM5
// General Purpose Timers: TIM9 - TIM14
// Basic Timers: TIM6 & TIM7
// Advanced Control Timers: TIM1 & TIM8

//======================================================================================//
//                  Address Definitions
//======================================================================================//

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
} timer_base_addr_e;

//======================================================================================//
//                  Macros and Other Enums
//======================================================================================//

//========================================================//
//          Register Bits
//========================================================//

typedef enum {
    TIM_CR1_CKD_POS  = 8, // Clock division)
    TIM_CR1_ARPE_POS = 7, // Auto-reload preload enable)
    TIM_CR1_CMS_POS  = 5, // Center-aligned mode selection)
    TIM_CR1_DIR_POS  = 4, // Direction)
    TIM_CR1_OPM_POS  = 3, // One-pulse mode)
    TIM_CR1_URS_POS  = 2, // Update request source)
    TIM_CR1_UDIS_POS = 1, // Update disable)
    TIM_CR1_CEN_POS  = 0, // Counter enable)
} tim_cr1_pos_e;

typedef enum {
    TIM_CR2_TI1S_POS = 7, // TI1 selection)
    TIM_CR2_MMS_POS  = 4, // Master mode selection)
    TIM_CR2_CCDS_POS = 3, // Capture/compare DMA selection)
} tim_cr2_pos_e;

typedef enum {
    TIM_SMCR_ETP_POS  = 15, // External trigger polarity)
    TIM_SMCR_ECE_POS  = 14, // External clock enable)
    TIM_SMCR_ETPS_POS = 12, // External trigger prescaler)
    TIM_SMCR_ETF_POS  = 8,  // External trigger filter)
    TIM_SMCR_MSM_POS  = 7,  // Master/Slave mode)
    TIM_SMCR_TS_POS   = 4,  // Trigger selection)
    TIM_SMCR_SMS_POS  = 0,  // Slave mode selection)
} tim_smcr_pos_e;

typedef enum {
    TIM_DIER_TDE_POS   = 14, // Trigger DMA request enable)
    TIM_DIER_CC4DE_POS = 12, // Capture/Compare 4 DMA request enable)
    TIM_DIER_CC3DE_POS = 11, // Capture/Compare 3 DMA request enable)
    TIM_DIER_CC2DE_POS = 10, // Capture/Compare 2 DMA request enable)
    TIM_DIER_CC1DE_POS = 9,  // Capture/Compare 1 DMA request enable)
    TIM_DIER_UDE_POS   = 8,  // Update DMA request enable)
    TIM_DIER_TIE_POS   = 6,  // Trigger interrupt enable)
    TIM_DIER_CC4IE_POS = 4,  // Capture/Compare 4 interrupt enable)
    TIM_DIER_CC3IE_POS = 3,  // Capture/Compare 3 interrupt enable)
    TIM_DIER_CC2IE_POS = 2,  // Capture/Compare 2 interrupt enable)
    TIM_DIER_CC1IE_POS = 1,  // Capture/Compare 1 interrupt enable)
    TIM_DIER_UIE_POS   = 0,  // Update interrupt enable)
} tim_dier_pos_e;

typedef enum {
    TIM_SR_CC4OF_POS = 12, // Capture/Compare 4 overcapture flag)
    TIM_SR_CC3OF_POS = 11, // Capture/Compare 3 overcapture flag)
    TIM_SR_CC2OF_POS = 10, // Capture/compare 2 overcapture flag)
    TIM_SR_CC1OF_POS = 9,  // Capture/Compare 1 overcapture flag)
    TIM_SR_TIF_POS   = 6,  // Trigger interrupt flag)
    TIM_SR_CC4IF_POS = 4,  // Capture/Compare 4 interrupt flag)
    TIM_SR_CC3IF_POS = 3,  // Capture/Compare 3 interrupt flag)
    TIM_SR_CC2IF_POS = 2,  // Capture/Compare 2 interrupt flag)
    TIM_SR_CC1IF_POS = 1,  // Capture/compare 1 interrupt flag)
    TIM_SR_UIF_POS   = 0,  // Update interrupt flag)
} tim_sr_pos_e;

typedef enum {
    TIM_EGR_TG_POS   = 6, // Trigger generation)
    TIM_EGR_CC4G_POS = 4, // Capture/compare 4 generation)
    TIM_EGR_CC3G_POS = 3, // Capture/compare 3 generation)
    TIM_EGR_CC2G_POS = 2, // Capture/compare 2 generation)
    TIM_EGR_CC1G_POS = 1, // Capture/compare 1 generation)
    TIM_EGR_UG_POS   = 0, // Update generation)
} tim_egr_pos_e;

typedef enum {
    TIM_CCMR1_Output_OC2CE_POS = 15, // OC2CE)
    TIM_CCMR1_Output_OC2M_POS  = 12, // OC2M)
    TIM_CCMR1_Output_OC2PE_POS = 11, // OC2PE)
    TIM_CCMR1_Output_OC2FE_POS = 10, // OC2FE)
    TIM_CCMR1_Output_CC2S_POS  = 8,  // CC2S)
    TIM_CCMR1_Output_OC1CE_POS = 7,  // OC1CE)
    TIM_CCMR1_Output_OC1M_POS  = 4,  // OC1M)
    TIM_CCMR1_Output_OC1PE_POS = 3,  // OC1PE)
    TIM_CCMR1_Output_OC1FE_POS = 2,  // OC1FE)
    TIM_CCMR1_Output_CC1S_POS  = 0,  // CC1S)
} tim_ccmr1_output_pos_e;

typedef enum {
    TIM_CCMR1_Input_IC2F_POS   = 12, // Input capture 2 filter)
    TIM_CCMR1_Input_IC2PCS_POS = 10, // Input capture 2 prescaler)
    TIM_CCMR1_Input_CC2S_POS   = 8,  // Capture/Compare 2 selection)
    TIM_CCMR1_Input_IC1F_POS   = 4,  // Input capture 1 filter)
    TIM_CCMR1_Input_ICPCS_POS  = 2,  // Input capture 1 prescaler)
    TIM_CCMR1_Input_CC1S_POS   = 0,  // Capture/Compare 1 selection)
} tim_ccmr1_input_pos_e;

typedef enum {
    TIM_CCMR2_Output_OC4CE_POS = 15, // OC4CE)
    TIM_CCMR2_Output_OC4M_POS  = 12, // OC4M)
    TIM_CCMR2_Output_OC4PE_POS = 11, // OC4PE)
    TIM_CCMR2_Output_OC4FE_POS = 10, // OC4FE)
    TIM_CCMR2_Output_CC4S_POS  = 8,  // CC4S)
    TIM_CCMR2_Output_OC3CE_POS = 7,  // OC3CE)
    TIM_CCMR2_Output_OC3M_POS  = 4,  // OC3M)
    TIM_CCMR2_Output_OC3PE_POS = 3,  // OC3PE)
    TIM_CCMR2_Output_OC3FE_POS = 2,  // OC3FE)
    TIM_CCMR2_Output_CC3S_POS  = 0,  // CC3S)
} tim_ccmr2_output_pos_e;

typedef enum {
    TIM_CCMR2_Input_IC4F_POS   = 12, // Input capture 4 filter)
    TIM_CCMR2_Input_IC4PSC_POS = 10, // Input capture 4 prescaler)
    TIM_CCMR2_Input_CC4S_POS   = 8,  // Capture/Compare 4 selection)
    TIM_CCMR2_Input_IC3F_POS   = 4,  // Input capture 3 filter)
    TIM_CCMR2_Input_IC3PSC_POS = 2,  // Input capture 3 prescaler)
    TIM_CCMR2_Input_CC3S_POS   = 0,  // Capture/compare 3 selection)
} tim_ccmr2_input_pos_e;

typedef enum {
    TIM_CCER_CC4NP_POS = 15, // Capture/Compare 4 output Polarity)
    TIM_CCER_CC4P_POS  = 13, // Capture/Compare 3 output Polarity)
    TIM_CCER_CC4E_POS  = 12, // Capture/Compare 4 output enable)
    TIM_CCER_CC3NP_POS = 11, // Capture/Compare 3 output Polarity)
    TIM_CCER_CC3P_POS  = 9,  // Capture/Compare 3 output Polarity)
    TIM_CCER_CC3E_POS  = 8,  // Capture/Compare 3 output enable)
    TIM_CCER_CC2NP_POS = 7,  // Capture/Compare 2 output Polarity)
    TIM_CCER_CC2P_POS  = 5,  // Capture/Compare 2 output Polarity)
    TIM_CCER_CC2E_POS  = 4,  // Capture/Compare 2 output enable)
    TIM_CCER_CC1NP_POS = 3,  // Capture/Compare 1 output Polarity)
    TIM_CCER_CC1P_POS  = 1,  // Capture/Compare 1 output Polarity)
    TIM_CCER_CC1E_POS  = 0,  // Capture/Compare 1 output enable)
} tim_ccer_pos_e;

typedef enum {
    TIM_CNT_CNT_H_POS = 16, // High counter value)
    TIM_CNT_CNT_L_POS = 0,  // Low counter value)
} tim_cnt_pos_e;

typedef enum {
    TIM_PSC_PSC_POS = 0, // Prescaler value)
} tim_psc_pos_e;

typedef enum {
    TIM_ARR_ARR_H_POS = 16, // High Auto-reload value)
    TIM_ARR_ARR_L_POS = 0,  // Low Auto-reload value)
} tim_arr_pos_e;

typedef enum {
    TIM_CCR1_CCR1_H_POS = 16, // High Capture/Compare 1 value)
    TIM_CCR1_CCR1_L_POS = 0,  // Low Capture/Compare 1 value)
} tim_ccr1_pos_e;

typedef enum {
    TIM_CCR2_CCR2_H_POS = 16, // High Capture/Compare 2 value)
    TIM_CCR2_CCR2_L_POS = 0,  // Low Capture/Compare 2 value)
} tim_ccr2_pos_e;

typedef enum {
    TIM_CCR3_CCR3_H_POS = 16, // High Capture/Compare value)
    TIM_CCR3_CCR3_L_POS = 0,  // Low Capture/Compare value)
} tim_ccr3_pos_e;

typedef enum {
    TIM_CCR4_CCR4_H_POS = 16, // High Capture/Compare value)
    TIM_CCR4_CCR4_L_POS = 0,  // Low Capture/Compare value)
} tim_ccr4_pos_e;

typedef enum {
    TIM_DCR_DBL_POS = 8, // DMA burst length)
    TIM_DCR_DBA_POS = 0, // DMA base address)
} tim_dcr_pos_e;

typedef enum {
    TIM_DMAR_DMAB_POS = 0, // DMA register for burst accesses)
} tim_dmar_pos_e;

typedef enum {
    TIM_OR_ITR1_RMP_POS = 10, // Timer Input 4 remap)
} tim_or_pos_e;

typedef enum {
    TIM_CR1_CKD  = (1 << TIM_CR1_CKD_POS),  // Clock division)
    TIM_CR1_ARPE = (1 << TIM_CR1_ARPE_POS), // Auto-reload preload enable)
    TIM_CR1_CMS  = (1 << TIM_CR1_CMS_POS),  // Center-aligned mode selection)
    TIM_CR1_DIR  = (1 << TIM_CR1_DIR_POS),  // Direction)
    TIM_CR1_OPM  = (1 << TIM_CR1_OPM_POS),  // One-pulse mode)
    TIM_CR1_URS  = (1 << TIM_CR1_URS_POS),  // Update request source)
    TIM_CR1_UDIS = (1 << TIM_CR1_UDIS_POS), // Update disable)
    TIM_CR1_CEN  = (1 << TIM_CR1_CEN_POS),  // Counter enable)
} tim_cr1_mask_e;

typedef enum {
    TIM_CR2_TI1S = (1 << TIM_CR2_TI1S_POS), // TI1 selection)
    TIM_CR2_MMS  = (1 << TIM_CR2_MMS_POS),  // Master mode selection)
    TIM_CR2_CCDS = (1 << TIM_CR2_CCDS_POS), // Capture/compare DMA selection)
} tim_cr2_mask_e;

typedef enum {
    TIM_SMCR_ETP  = (1 << TIM_SMCR_ETP_POS),  // External trigger polarity)
    TIM_SMCR_ECE  = (1 << TIM_SMCR_ECE_POS),  // External clock enable)
    TIM_SMCR_ETPS = (1 << TIM_SMCR_ETPS_POS), // External trigger prescaler)
    TIM_SMCR_ETF  = (1 << TIM_SMCR_ETF_POS),  // External trigger filter)
    TIM_SMCR_MSM  = (1 << TIM_SMCR_MSM_POS),  // Master/Slave mode)
    TIM_SMCR_TS   = (1 << TIM_SMCR_TS_POS),   // Trigger selection)
    TIM_SMCR_SMS  = (1 << TIM_SMCR_SMS_POS),  // Slave mode selection)
} tim_smcr_mask_e;

typedef enum {
    TIM_DIER_TDE   = (1 << TIM_DIER_TDE_POS),   // Trigger DMA request enable)
    TIM_DIER_CC4DE = (1 << TIM_DIER_CC4DE_POS), // Capture/Compare 4 DMA request enable)
    TIM_DIER_CC3DE = (1 << TIM_DIER_CC3DE_POS), // Capture/Compare 3 DMA request enable)
    TIM_DIER_CC2DE = (1 << TIM_DIER_CC2DE_POS), // Capture/Compare 2 DMA request enable)
    TIM_DIER_CC1DE = (1 << TIM_DIER_CC1DE_POS), // Capture/Compare 1 DMA request enable)
    TIM_DIER_UDE   = (1 << TIM_DIER_UDE_POS),   // Update DMA request enable)
    TIM_DIER_TIE   = (1 << TIM_DIER_TIE_POS),   // Trigger interrupt enable)
    TIM_DIER_CC4IE = (1 << TIM_DIER_CC4IE_POS), // Capture/Compare 4 interrupt enable)
    TIM_DIER_CC3IE = (1 << TIM_DIER_CC3IE_POS), // Capture/Compare 3 interrupt enable)
    TIM_DIER_CC2IE = (1 << TIM_DIER_CC2IE_POS), // Capture/Compare 2 interrupt enable)
    TIM_DIER_CC1IE = (1 << TIM_DIER_CC1IE_POS), // Capture/Compare 1 interrupt enable)
    TIM_DIER_UIE   = (1 << TIM_DIER_UIE_POS),   // Update interrupt enable)
} tim_dier_mask_e;

typedef enum {
    TIM_SR_CC4OF = (1 << TIM_SR_CC4OF_POS), // Capture/Compare 4 overcapture flag)
    TIM_SR_CC3OF = (1 << TIM_SR_CC3OF_POS), // Capture/Compare 3 overcapture flag)
    TIM_SR_CC2OF = (1 << TIM_SR_CC2OF_POS), // Capture/compare 2 overcapture flag)
    TIM_SR_CC1OF = (1 << TIM_SR_CC1OF_POS), // Capture/Compare 1 overcapture flag)
    TIM_SR_TIF   = (1 << TIM_SR_TIF_POS),   // Trigger interrupt flag)
    TIM_SR_CC4IF = (1 << TIM_SR_CC4IF_POS), // Capture/Compare 4 interrupt flag)
    TIM_SR_CC3IF = (1 << TIM_SR_CC3IF_POS), // Capture/Compare 3 interrupt flag)
    TIM_SR_CC2IF = (1 << TIM_SR_CC2IF_POS), // Capture/Compare 2 interrupt flag)
    TIM_SR_CC1IF = (1 << TIM_SR_CC1IF_POS), // Capture/compare 1 interrupt flag)
    TIM_SR_UIF   = (1 << TIM_SR_UIF_POS),   // Update interrupt flag)
} tim_sr_mask_e;

typedef enum {
    TIM_EGR_TG   = (1 << TIM_EGR_TG_POS),   // Trigger generation)
    TIM_EGR_CC4G = (1 << TIM_EGR_CC4G_POS), // Capture/compare 4 generation)
    TIM_EGR_CC3G = (1 << TIM_EGR_CC3G_POS), // Capture/compare 3 generation)
    TIM_EGR_CC2G = (1 << TIM_EGR_CC2G_POS), // Capture/compare 2 generation)
    TIM_EGR_CC1G = (1 << TIM_EGR_CC1G_POS), // Capture/compare 1 generation)
    TIM_EGR_UG   = (1 << TIM_EGR_UG_POS),   // Update generation)
} tim_egr_mask_e;

typedef enum {
    TIM_CCMR1_Output_OC2CE = (1 << TIM_CCMR1_Output_OC2CE_POS), // OC2CE)
    TIM_CCMR1_Output_OC2M  = (1 << TIM_CCMR1_Output_OC2M_POS),  // OC2M)
    TIM_CCMR1_Output_OC2PE = (1 << TIM_CCMR1_Output_OC2PE_POS), // OC2PE)
    TIM_CCMR1_Output_OC2FE = (1 << TIM_CCMR1_Output_OC2FE_POS), // OC2FE)
    TIM_CCMR1_Output_CC2S  = (1 << TIM_CCMR1_Output_CC2S_POS),  // CC2S)
    TIM_CCMR1_Output_OC1CE = (1 << TIM_CCMR1_Output_OC1CE_POS), // OC1CE)
    TIM_CCMR1_Output_OC1M  = (1 << TIM_CCMR1_Output_OC1M_POS),  // OC1M)
    TIM_CCMR1_Output_OC1PE = (1 << TIM_CCMR1_Output_OC1PE_POS), // OC1PE)
    TIM_CCMR1_Output_OC1FE = (1 << TIM_CCMR1_Output_OC1FE_POS), // OC1FE)
    TIM_CCMR1_Output_CC1S  = (1 << TIM_CCMR1_Output_CC1S_POS),  // CC1S)
} tim_ccmr1_output_mask_e;

typedef enum {
    TIM_CCMR1_Input_IC2F   = (1 << TIM_CCMR1_Input_IC2F_POS),   // Input capture 2 filter)
    TIM_CCMR1_Input_IC2PCS = (1 << TIM_CCMR1_Input_IC2PCS_POS), // Input capture 2 prescaler)
    TIM_CCMR1_Input_CC2S   = (1 << TIM_CCMR1_Input_CC2S_POS),   // Capture/Compare 2 selection)
    TIM_CCMR1_Input_IC1F   = (1 << TIM_CCMR1_Input_IC1F_POS),   // Input capture 1 filter)
    TIM_CCMR1_Input_ICPCS  = (1 << TIM_CCMR1_Input_ICPCS_POS),  // Input capture 1 prescaler)
    TIM_CCMR1_Input_CC1S   = (1 << TIM_CCMR1_Input_CC1S_POS),   // Capture/Compare 1 selection)
} tim_ccmr1_input_mask_e;

typedef enum {
    TIM_CCMR2_Output_OC4CE = (1 << TIM_CCMR2_Output_OC4CE_POS), // OC4CE)
    TIM_CCMR2_Output_OC4M  = (1 << TIM_CCMR2_Output_OC4M_POS),  // OC4M)
    TIM_CCMR2_Output_OC4PE = (1 << TIM_CCMR2_Output_OC4PE_POS), // OC4PE)
    TIM_CCMR2_Output_OC4FE = (1 << TIM_CCMR2_Output_OC4FE_POS), // OC4FE)
    TIM_CCMR2_Output_CC4S  = (1 << TIM_CCMR2_Output_CC4S_POS),  // CC4S)
    TIM_CCMR2_Output_OC3CE = (1 << TIM_CCMR2_Output_OC3CE_POS), // OC3CE)
    TIM_CCMR2_Output_OC3M  = (1 << TIM_CCMR2_Output_OC3M_POS),  // OC3M)
    TIM_CCMR2_Output_OC3PE = (1 << TIM_CCMR2_Output_OC3PE_POS), // OC3PE)
    TIM_CCMR2_Output_OC3FE = (1 << TIM_CCMR2_Output_OC3FE_POS), // OC3FE)
    TIM_CCMR2_Output_CC3S  = (1 << TIM_CCMR2_Output_CC3S_POS),  // CC3S)
} tim_ccmr2_output_mask_e;

typedef enum {
    TIM_CCMR2_Input_IC4F   = (1 << TIM_CCMR2_Input_IC4F_POS),   // Input capture 4 filter)
    TIM_CCMR2_Input_IC4PSC = (1 << TIM_CCMR2_Input_IC4PSC_POS), // Input capture 4 prescaler)
    TIM_CCMR2_Input_CC4S   = (1 << TIM_CCMR2_Input_CC4S_POS),   // Capture/Compare 4 selection)
    TIM_CCMR2_Input_IC3F   = (1 << TIM_CCMR2_Input_IC3F_POS),   // Input capture 3 filter)
    TIM_CCMR2_Input_IC3PSC = (1 << TIM_CCMR2_Input_IC3PSC_POS), // Input capture 3 prescaler)
    TIM_CCMR2_Input_CC3S   = (1 << TIM_CCMR2_Input_CC3S_POS),   // Capture/compare 3 selection)
} tim_ccmr2_input_mask_e;

typedef enum {
    TIM_CCER_CC4NP = (1 << TIM_CCER_CC4NP_POS), // Capture/Compare 4 output Polarity)
    TIM_CCER_CC4P  = (1 << TIM_CCER_CC4P_POS),  // Capture/Compare 3 output Polarity)
    TIM_CCER_CC4E  = (1 << TIM_CCER_CC4E_POS),  // Capture/Compare 4 output enable)
    TIM_CCER_CC3NP = (1 << TIM_CCER_CC3NP_POS), // Capture/Compare 3 output Polarity)
    TIM_CCER_CC3P  = (1 << TIM_CCER_CC3P_POS),  // Capture/Compare 3 output Polarity)
    TIM_CCER_CC3E  = (1 << TIM_CCER_CC3E_POS),  // Capture/Compare 3 output enable)
    TIM_CCER_CC2NP = (1 << TIM_CCER_CC2NP_POS), // Capture/Compare 2 output Polarity)
    TIM_CCER_CC2P  = (1 << TIM_CCER_CC2P_POS),  // Capture/Compare 2 output Polarity)
    TIM_CCER_CC2E  = (1 << TIM_CCER_CC2E_POS),  // Capture/Compare 2 output enable)
    TIM_CCER_CC1NP = (1 << TIM_CCER_CC1NP_POS), // Capture/Compare 1 output Polarity)
    TIM_CCER_CC1P  = (1 << TIM_CCER_CC1P_POS),  // Capture/Compare 1 output Polarity)
    TIM_CCER_CC1E  = (1 << TIM_CCER_CC1E_POS),  // Capture/Compare 1 output enable)
} tim_ccer_mask_e;

typedef enum {
    TIM_CNT_CNT_H = (1 << TIM_CNT_CNT_H_POS), // High counter value)
    TIM_CNT_CNT_L = (1 << TIM_CNT_CNT_L_POS), // Low counter value)
} tim_cnt_mask_e;

typedef enum {
    TIM_PSC_PSC = (1 << TIM_PSC_PSC_POS), // Prescaler value)
} tim_psc_mask_e;

typedef enum {
    TIM_ARR_ARR_H = (1 << TIM_ARR_ARR_H_POS), // High Auto-reload value)
    TIM_ARR_ARR_L = (1 << TIM_ARR_ARR_L_POS), // Low Auto-reload value)
} tim_arr_mask_e;

typedef enum {
    TIM_CCR1_CCR1_H = (1 << TIM_CCR1_CCR1_H_POS), // High Capture/Compare 1 value)
    TIM_CCR1_CCR1_L = (1 << TIM_CCR1_CCR1_L_POS), // Low Capture/Compare 1 value)
} tim_ccr1_mask_e;

typedef enum {
    TIM_CCR2_CCR2_H = (1 << TIM_CCR2_CCR2_H_POS), // High Capture/Compare 2 value)
    TIM_CCR2_CCR2_L = (1 << TIM_CCR2_CCR2_L_POS), // Low Capture/Compare 2 value)
} tim_ccr2_mask_e;

typedef enum {
    TIM_CCR3_CCR3_H = (1 << TIM_CCR3_CCR3_H_POS), // High Capture/Compare value)
    TIM_CCR3_CCR3_L = (1 << TIM_CCR3_CCR3_L_POS), // Low Capture/Compare value)
} tim_ccr3_mask_e;

typedef enum {
    TIM_CCR4_CCR4_H = (1 << TIM_CCR4_CCR4_H_POS), // High Capture/Compare value)
    TIM_CCR4_CCR4_L = (1 << TIM_CCR4_CCR4_L_POS), // Low Capture/Compare value)
} tim_ccr4_mask_e;

typedef enum {
    TIM_DCR_DBL = (1 << TIM_DCR_DBL_POS), // DMA burst length)
    TIM_DCR_DBA = (1 << TIM_DCR_DBA_POS), // DMA base address)
} tim_dcr_mask_e;

typedef enum {
    TIM_DMAR_DMAB = (1 << TIM_DMAR_DMAB_POS), // DMA register for burst accesses)
} tim_dmar_mask_e;

typedef enum {
    TIM_OR_ITR1_RMP = (1 << TIM_OR_ITR1_RMP_POS), // Timer Input 4 remap)
} tim_or_mask_e;

//======================================================================================//
//                  Register Structure Definitions
//======================================================================================//

typedef __vo struct {
    uint32_t CR1;          // control register 1                                 Offset: 0x0
    uint32_t CR2;          // control register 2                                 Offset: 0x4
    uint32_t SMCR;         // slave mode control register                        Offset: 0x8
    uint32_t DIER;         // DMA/Interrupt enable register                      Offset: 0xC
    uint32_t SR;           // status register                                    Offset: 0x10
    uint32_t EGR;          // event generation register                          Offset: 0x14
    uint32_t CCMR1_Output; // capture/compare mode register 1 (output mode)      Offset: 0x18
    uint32_t CCMR1_Input;  // capture/compare mode register 1 (input mode)       Offset: 0x18
    uint32_t CCMR2_Output; // capture/compare mode register 2 (output mode)      Offset: 0x1C
    uint32_t CCMR2_Input;  // capture/compare mode register 2 (input mode)       Offset: 0x1C
    uint32_t CCER;         // capture/compare enable register                    Offset: 0x20
    uint32_t CNT;          // counter                                            Offset: 0x24
    uint32_t PSC;          // prescaler                                          Offset: 0x28
    uint32_t ARR;          // auto-reload register                               Offset: 0x2C
    uint32_t CCR1;         // capture/compare register 1                         Offset: 0x34
    uint32_t CCR2;         // capture/compare register 2                         Offset: 0x38
    uint32_t CCR3;         // capture/compare register 3                         Offset: 0x3C
    uint32_t CCR4;         // capture/compare register 4                         Offset: 0x40
    uint32_t DCR;          // DMA control register                               Offset: 0x48
    uint32_t DMAR;         // DMA address for full transfer                      Offset: 0x4C
    uint32_t OR;           // TIM5 option register                               Offset: 0x50
} tim_reg_def;
// Peripheral structure definitions

// Function API prototypes
#endif
