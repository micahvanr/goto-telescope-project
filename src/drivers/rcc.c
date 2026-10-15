//======================================================================================//{
//  File: rcc.c
//  Overview: Provides functions for initializing, configuring, and using the RCC peripheral.
//  Features:
//      Enable or disable LSI
//      Check if LSI is ready
//      Enable or disable GPIO clock
//      Enable or disable I2C clock
//      Enable or disable PWR clock
//      Enable or disable RTC clock
//      Enable or disable SYSCFG clock
//      Enable or disable TIM clock
//      Enable or disable USART clock
//      Get bus clock frequency
//      Get timer bus clock frequency
//      Get system clock frequency
//      Get PLL clock frequency
//      Configure RTC clock source and prescaler
//      Configure MCO pin
//  Notes: Additional notes that user might/should want to know
//======================================================================================//}

//} Include Statements
//=========================================//{

#include "rcc.h"
#include "assert_handler.h"
#include "gpio.h"
#include "stm32f4xx.h"
#include <stdint.h>

//======================================================================================//}
//                  Private Constants
//======================================================================================//{

//========================================================//}
//          Register Constants
//========================================================//{

typedef enum : uint32_t {
    RCC_BDCR_RTCSEL_MASK = 0b11U
} rcc_reg_misc_e;

// clang-format off
typedef enum : uint32_t {
    RCC_CR_PLLI2SRDY_POS = 27U,   // PLLI2S clock ready flag
    RCC_CR_PLLI2SON_POS  = 26U,   // PLLI2S enable
    RCC_CR_PLLRDY_POS    = 25U,   // Main PLL (PLL) clock ready flag
    RCC_CR_PLLON_POS     = 24U,   // Main PLL (PLL) enable
    RCC_CR_CSSON_POS     = 19U,   // Clock security system enable
    RCC_CR_HSEBYP_POS    = 18U,   // HSE clock bypass
    RCC_CR_HSERDY_POS    = 17U,   // HSE clock ready flag
    RCC_CR_HSEON_POS     = 16U,   // HSE clock enable
    RCC_CR_HSICAL_POS    = 8U,    // Internal high-speed clock calibration
    RCC_CR_HSITRIM_POS   = 3U,    // Internal high-speed clock trimming
    RCC_CR_HSIRDY_POS    = 1U,    // Internal high-speed clock ready flag
    RCC_CR_HSION_POS     = 0U,    // Internal high-speed clock enable
} rcc_cr_pos_e;

typedef enum : uint32_t {
    RCC_CR_PLLI2SRDY = (1U << RCC_CR_PLLI2SRDY_POS),   // PLLI2S clock ready flag
    RCC_CR_PLLI2SON  = (1U << RCC_CR_PLLI2SON_POS),    // PLLI2S enable
    RCC_CR_PLLRDY    = (1U << RCC_CR_PLLRDY_POS),      // Main PLL (PLL) clock ready flag
    RCC_CR_PLLON     = (1U << RCC_CR_PLLON_POS),       // Main PLL (PLL) enable
    RCC_CR_CSSON     = (1U << RCC_CR_CSSON_POS),       // Clock security system enable
    RCC_CR_HSEBYP    = (1U << RCC_CR_HSEBYP_POS),      // HSE clock bypass
    RCC_CR_HSERDY    = (1U << RCC_CR_HSERDY_POS),      // HSE clock ready flag
    RCC_CR_HSEON     = (1U << RCC_CR_HSEON_POS),       // HSE clock enable
    RCC_CR_HSICAL    = (1U << RCC_CR_HSICAL_POS),      // Internal high-speed clock calibration
    RCC_CR_HSITRIM   = (1U << RCC_CR_HSITRIM_POS),     // Internal high-speed clock trimming
    RCC_CR_HSIRDY    = (1U << RCC_CR_HSIRDY_POS),      // Internal high-speed clock ready flag
    RCC_CR_HSION     = (1U << RCC_CR_HSION_POS),       // Internal high-speed clock enable
} rcc_cr_e;

typedef enum : uint32_t {
    RCC_CR_PLLI2SRDY_MASK = 0b1U,          // 1 bit(s)
    RCC_CR_PLLI2SON_MASK  = 0b1U,          // 1 bit(s)
    RCC_CR_PLLRDY_MASK    = 0b1U,          // 1 bit(s)
    RCC_CR_PLLON_MASK     = 0b1U,          // 1 bit(s)
    RCC_CR_CSSON_MASK     = 0b1U,          // 1 bit(s)
    RCC_CR_HSEBYP_MASK    = 0b1U,          // 1 bit(s)
    RCC_CR_HSERDY_MASK    = 0b1U,          // 1 bit(s)
    RCC_CR_HSEON_MASK     = 0b1U,          // 1 bit(s)
    RCC_CR_HSICAL_MASK    = 0b11111111U,   // 8 bit(s)
    RCC_CR_HSITRIM_MASK   = 0b11111U,      // 5 bit(s)
    RCC_CR_HSIRDY_MASK    = 0b1U,          // 1 bit(s)
    RCC_CR_HSION_MASK     = 0b1U,          // 1 bit(s)
} rcc_cr_mask_e;

typedef enum : uint32_t {
    RCC_PLLCFGR_PLLQ3_POS  = 27U,   // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ2_POS  = 26U,   // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ1_POS  = 25U,   // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ0_POS  = 24U,   // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLSRC_POS = 22U,   // Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
    RCC_PLLCFGR_PLLP1_POS  = 17U,   // Main PLL (PLL) division factor for main system clock
    RCC_PLLCFGR_PLLP0_POS  = 16U,   // Main PLL (PLL) division factor for main system clock
    RCC_PLLCFGR_PLLN8_POS  = 14U,   // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN7_POS  = 13U,   // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN6_POS  = 12U,   // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN5_POS  = 11U,   // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN4_POS  = 10U,   // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN3_POS  = 9U,    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN2_POS  = 8U,    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN1_POS  = 7U,    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN0_POS  = 6U,    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLM5_POS  = 5U,    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM4_POS  = 4U,    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM3_POS  = 3U,    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM2_POS  = 2U,    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM1_POS  = 1U,    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM0_POS  = 0U,    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
} rcc_pllcfgr_pos_e;

typedef enum : uint32_t {
    RCC_PLLCFGR_PLLQ3  = (1U << RCC_PLLCFGR_PLLQ3_POS),    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ2  = (1U << RCC_PLLCFGR_PLLQ2_POS),    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ1  = (1U << RCC_PLLCFGR_PLLQ1_POS),    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ0  = (1U << RCC_PLLCFGR_PLLQ0_POS),    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLSRC = (1U << RCC_PLLCFGR_PLLSRC_POS),   // Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
    RCC_PLLCFGR_PLLP1  = (1U << RCC_PLLCFGR_PLLP1_POS),    // Main PLL (PLL) division factor for main system clock
    RCC_PLLCFGR_PLLP0  = (1U << RCC_PLLCFGR_PLLP0_POS),    // Main PLL (PLL) division factor for main system clock
    RCC_PLLCFGR_PLLN8  = (1U << RCC_PLLCFGR_PLLN8_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN7  = (1U << RCC_PLLCFGR_PLLN7_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN6  = (1U << RCC_PLLCFGR_PLLN6_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN5  = (1U << RCC_PLLCFGR_PLLN5_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN4  = (1U << RCC_PLLCFGR_PLLN4_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN3  = (1U << RCC_PLLCFGR_PLLN3_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN2  = (1U << RCC_PLLCFGR_PLLN2_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN1  = (1U << RCC_PLLCFGR_PLLN1_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN0  = (1U << RCC_PLLCFGR_PLLN0_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLM5  = (1U << RCC_PLLCFGR_PLLM5_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM4  = (1U << RCC_PLLCFGR_PLLM4_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM3  = (1U << RCC_PLLCFGR_PLLM3_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM2  = (1U << RCC_PLLCFGR_PLLM2_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM1  = (1U << RCC_PLLCFGR_PLLM1_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM0  = (1U << RCC_PLLCFGR_PLLM0_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
} rcc_pllcfgr_e;

typedef enum : uint32_t {
    RCC_PLLCFGR_PLLQ3_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLQ2_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLQ1_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLQ0_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLSRC_MASK = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLP1_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLP0_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLN8_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLN7_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLN6_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLN5_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLN4_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLN3_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLN2_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLN1_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLN0_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLM5_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLM4_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLM3_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLM2_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLM1_MASK  = 0b1U,   // 1 bit(s)
    RCC_PLLCFGR_PLLM0_MASK  = 0b1U,   // 1 bit(s)
} rcc_pllcfgr_mask_e;

typedef enum : uint32_t {
    RCC_CFGR_MCO2_POS    = 30U,   // Microcontroller clock output 2
    RCC_CFGR_MCO2PRE_POS = 27U,   // MCO2 prescaler
    RCC_CFGR_MCO1PRE_POS = 24U,   // MCO1 prescaler
    RCC_CFGR_I2SSRC_POS  = 23U,   // I2S clock selection
    RCC_CFGR_MCO1_POS    = 21U,   // Microcontroller clock output 1
    RCC_CFGR_RTCPRE_POS  = 16U,   // HSE division factor for RTC clock
    RCC_CFGR_PPRE2_POS   = 13U,   // APB high-speed prescaler (APB2)
    RCC_CFGR_PPRE1_POS   = 10U,   // APB Low speed prescaler (APB1)
    RCC_CFGR_HPRE_POS    = 4U,    // AHB prescaler
    RCC_CFGR_SWS1_POS    = 3U,    // System clock switch status
    RCC_CFGR_SWS0_POS    = 2U,    // System clock switch status
    RCC_CFGR_SW1_POS     = 1U,    // System clock switch
    RCC_CFGR_SW0_POS     = 0U,    // System clock switch
} rcc_cfgr_pos_e;

typedef enum : uint32_t {
    RCC_CFGR_MCO2    = (1U << RCC_CFGR_MCO2_POS),      // Microcontroller clock output 2
    RCC_CFGR_MCO2PRE = (1U << RCC_CFGR_MCO2PRE_POS),   // MCO2 prescaler
    RCC_CFGR_MCO1PRE = (1U << RCC_CFGR_MCO1PRE_POS),   // MCO1 prescaler
    RCC_CFGR_I2SSRC  = (1U << RCC_CFGR_I2SSRC_POS),    // I2S clock selection
    RCC_CFGR_MCO1    = (1U << RCC_CFGR_MCO1_POS),      // Microcontroller clock output 1
    RCC_CFGR_RTCPRE  = (1U << RCC_CFGR_RTCPRE_POS),    // HSE division factor for RTC clock
    RCC_CFGR_PPRE2   = (1U << RCC_CFGR_PPRE2_POS),     // APB high-speed prescaler (APB2)
    RCC_CFGR_PPRE1   = (1U << RCC_CFGR_PPRE1_POS),     // APB Low speed prescaler (APB1)
    RCC_CFGR_HPRE    = (1U << RCC_CFGR_HPRE_POS),      // AHB prescaler
    RCC_CFGR_SWS1    = (1U << RCC_CFGR_SWS1_POS),      // System clock switch status
    RCC_CFGR_SWS0    = (1U << RCC_CFGR_SWS0_POS),      // System clock switch status
    RCC_CFGR_SW1     = (1U << RCC_CFGR_SW1_POS),       // System clock switch
    RCC_CFGR_SW0     = (1U << RCC_CFGR_SW0_POS),       // System clock switch
} rcc_cfgr_e;

typedef enum : uint32_t {
    RCC_CFGR_MCO2_MASK    = 0b11U,      // 2 bit(s)
    RCC_CFGR_MCO2PRE_MASK = 0b111U,     // 3 bit(s)
    RCC_CFGR_MCO1PRE_MASK = 0b111U,     // 3 bit(s)
    RCC_CFGR_I2SSRC_MASK  = 0b1U,       // 1 bit(s)
    RCC_CFGR_MCO1_MASK    = 0b11U,      // 2 bit(s)
    RCC_CFGR_RTCPRE_MASK  = 0b11111U,   // 5 bit(s)
    RCC_CFGR_PPRE2_MASK   = 0b111U,     // 3 bit(s)
    RCC_CFGR_PPRE1_MASK   = 0b111U,     // 3 bit(s)
    RCC_CFGR_HPRE_MASK    = 0b1111U,    // 4 bit(s)
    RCC_CFGR_SWS1_MASK    = 0b1U,       // 1 bit(s)
    RCC_CFGR_SWS0_MASK    = 0b1U,       // 1 bit(s)
    RCC_CFGR_SW1_MASK     = 0b1U,       // 1 bit(s)
    RCC_CFGR_SW0_MASK     = 0b1U,       // 1 bit(s)
} rcc_cfgr_mask_e;

typedef enum : uint32_t {
    RCC_CIR_CSSC_POS        = 23U,   // Clock security system interrupt clear
    RCC_CIR_PLLI2SRDYC_POS  = 21U,   // PLLI2S ready interrupt clear
    RCC_CIR_PLLRDYC_POS     = 20U,   // Main PLL(PLL) ready interrupt clear
    RCC_CIR_HSERDYC_POS     = 19U,   // HSE ready interrupt clear
    RCC_CIR_HSIRDYC_POS     = 18U,   // HSI ready interrupt clear
    RCC_CIR_LSERDYC_POS     = 17U,   // LSE ready interrupt clear
    RCC_CIR_LSIRDYC_POS     = 16U,   // LSI ready interrupt clear
    RCC_CIR_PLLI2SRDYIE_POS = 13U,   // PLLI2S ready interrupt enable
    RCC_CIR_PLLRDYIE_POS    = 12U,   // Main PLL (PLL) ready interrupt enable
    RCC_CIR_HSERDYIE_POS    = 11U,   // HSE ready interrupt enable
    RCC_CIR_HSIRDYIE_POS    = 10U,   // HSI ready interrupt enable
    RCC_CIR_LSERDYIE_POS    = 9U,    // LSE ready interrupt enable
    RCC_CIR_LSIRDYIE_POS    = 8U,    // LSI ready interrupt enable
    RCC_CIR_CSSF_POS        = 7U,    // Clock security system interrupt flag
    RCC_CIR_PLLI2SRDYF_POS  = 5U,    // PLLI2S ready interrupt flag
    RCC_CIR_PLLRDYF_POS     = 4U,    // Main PLL (PLL) ready interrupt flag
    RCC_CIR_HSERDYF_POS     = 3U,    // HSE ready interrupt flag
    RCC_CIR_HSIRDYF_POS     = 2U,    // HSI ready interrupt flag
    RCC_CIR_LSERDYF_POS     = 1U,    // LSE ready interrupt flag
    RCC_CIR_LSIRDYF_POS     = 0U,    // LSI ready interrupt flag
} rcc_cir_pos_e;

typedef enum : uint32_t {
    RCC_CIR_CSSC        = (1U << RCC_CIR_CSSC_POS),          // Clock security system interrupt clear
    RCC_CIR_PLLI2SRDYC  = (1U << RCC_CIR_PLLI2SRDYC_POS),    // PLLI2S ready interrupt clear
    RCC_CIR_PLLRDYC     = (1U << RCC_CIR_PLLRDYC_POS),       // Main PLL(PLL) ready interrupt clear
    RCC_CIR_HSERDYC     = (1U << RCC_CIR_HSERDYC_POS),       // HSE ready interrupt clear
    RCC_CIR_HSIRDYC     = (1U << RCC_CIR_HSIRDYC_POS),       // HSI ready interrupt clear
    RCC_CIR_LSERDYC     = (1U << RCC_CIR_LSERDYC_POS),       // LSE ready interrupt clear
    RCC_CIR_LSIRDYC     = (1U << RCC_CIR_LSIRDYC_POS),       // LSI ready interrupt clear
    RCC_CIR_PLLI2SRDYIE = (1U << RCC_CIR_PLLI2SRDYIE_POS),   // PLLI2S ready interrupt enable
    RCC_CIR_PLLRDYIE    = (1U << RCC_CIR_PLLRDYIE_POS),      // Main PLL (PLL) ready interrupt enable
    RCC_CIR_HSERDYIE    = (1U << RCC_CIR_HSERDYIE_POS),      // HSE ready interrupt enable
    RCC_CIR_HSIRDYIE    = (1U << RCC_CIR_HSIRDYIE_POS),      // HSI ready interrupt enable
    RCC_CIR_LSERDYIE    = (1U << RCC_CIR_LSERDYIE_POS),      // LSE ready interrupt enable
    RCC_CIR_LSIRDYIE    = (1U << RCC_CIR_LSIRDYIE_POS),      // LSI ready interrupt enable
    RCC_CIR_CSSF        = (1U << RCC_CIR_CSSF_POS),          // Clock security system interrupt flag
    RCC_CIR_PLLI2SRDYF  = (1U << RCC_CIR_PLLI2SRDYF_POS),    // PLLI2S ready interrupt flag
    RCC_CIR_PLLRDYF     = (1U << RCC_CIR_PLLRDYF_POS),       // Main PLL (PLL) ready interrupt flag
    RCC_CIR_HSERDYF     = (1U << RCC_CIR_HSERDYF_POS),       // HSE ready interrupt flag
    RCC_CIR_HSIRDYF     = (1U << RCC_CIR_HSIRDYF_POS),       // HSI ready interrupt flag
    RCC_CIR_LSERDYF     = (1U << RCC_CIR_LSERDYF_POS),       // LSE ready interrupt flag
    RCC_CIR_LSIRDYF     = (1U << RCC_CIR_LSIRDYF_POS),       // LSI ready interrupt flag
} rcc_cir_e;

typedef enum : uint32_t {
    RCC_CIR_CSSC_MASK        = 0b1U,   // 1 bit(s)
    RCC_CIR_PLLI2SRDYC_MASK  = 0b1U,   // 1 bit(s)
    RCC_CIR_PLLRDYC_MASK     = 0b1U,   // 1 bit(s)
    RCC_CIR_HSERDYC_MASK     = 0b1U,   // 1 bit(s)
    RCC_CIR_HSIRDYC_MASK     = 0b1U,   // 1 bit(s)
    RCC_CIR_LSERDYC_MASK     = 0b1U,   // 1 bit(s)
    RCC_CIR_LSIRDYC_MASK     = 0b1U,   // 1 bit(s)
    RCC_CIR_PLLI2SRDYIE_MASK = 0b1U,   // 1 bit(s)
    RCC_CIR_PLLRDYIE_MASK    = 0b1U,   // 1 bit(s)
    RCC_CIR_HSERDYIE_MASK    = 0b1U,   // 1 bit(s)
    RCC_CIR_HSIRDYIE_MASK    = 0b1U,   // 1 bit(s)
    RCC_CIR_LSERDYIE_MASK    = 0b1U,   // 1 bit(s)
    RCC_CIR_LSIRDYIE_MASK    = 0b1U,   // 1 bit(s)
    RCC_CIR_CSSF_MASK        = 0b1U,   // 1 bit(s)
    RCC_CIR_PLLI2SRDYF_MASK  = 0b1U,   // 1 bit(s)
    RCC_CIR_PLLRDYF_MASK     = 0b1U,   // 1 bit(s)
    RCC_CIR_HSERDYF_MASK     = 0b1U,   // 1 bit(s)
    RCC_CIR_HSIRDYF_MASK     = 0b1U,   // 1 bit(s)
    RCC_CIR_LSERDYF_MASK     = 0b1U,   // 1 bit(s)
    RCC_CIR_LSIRDYF_MASK     = 0b1U,   // 1 bit(s)
} rcc_cir_mask_e;

typedef enum : uint32_t {
    RCC_AHB1RSTR_OTGHSRST_POS  = 29U,   // USB OTG HS module reset
    RCC_AHB1RSTR_ETHMACRST_POS = 25U,   // Ethernet MAC reset
    RCC_AHB1RSTR_DMA2RST_POS   = 22U,   // DMA2 reset
    RCC_AHB1RSTR_DMA1RST_POS   = 21U,   // DMA2 reset
    RCC_AHB1RSTR_CRCRST_POS    = 12U,   // CRC reset
    RCC_AHB1RSTR_GPIOIRST_POS  = 8U,    // IO port I reset
    RCC_AHB1RSTR_GPIOHRST_POS  = 7U,    // IO port H reset
    RCC_AHB1RSTR_GPIOGRST_POS  = 6U,    // IO port G reset
    RCC_AHB1RSTR_GPIOFRST_POS  = 5U,    // IO port F reset
    RCC_AHB1RSTR_GPIOERST_POS  = 4U,    // IO port E reset
    RCC_AHB1RSTR_GPIODRST_POS  = 3U,    // IO port D reset
    RCC_AHB1RSTR_GPIOCRST_POS  = 2U,    // IO port C reset
    RCC_AHB1RSTR_GPIOBRST_POS  = 1U,    // IO port B reset
    RCC_AHB1RSTR_GPIOARST_POS  = 0U,    // IO port A reset
} rcc_ahb1rstr_pos_e;

typedef enum : uint32_t {
    RCC_AHB1RSTR_OTGHSRST  = (1U << RCC_AHB1RSTR_OTGHSRST_POS),    // USB OTG HS module reset
    RCC_AHB1RSTR_ETHMACRST = (1U << RCC_AHB1RSTR_ETHMACRST_POS),   // Ethernet MAC reset
    RCC_AHB1RSTR_DMA2RST   = (1U << RCC_AHB1RSTR_DMA2RST_POS),     // DMA2 reset
    RCC_AHB1RSTR_DMA1RST   = (1U << RCC_AHB1RSTR_DMA1RST_POS),     // DMA2 reset
    RCC_AHB1RSTR_CRCRST    = (1U << RCC_AHB1RSTR_CRCRST_POS),      // CRC reset
    RCC_AHB1RSTR_GPIOIRST  = (1U << RCC_AHB1RSTR_GPIOIRST_POS),    // IO port I reset
    RCC_AHB1RSTR_GPIOHRST  = (1U << RCC_AHB1RSTR_GPIOHRST_POS),    // IO port H reset
    RCC_AHB1RSTR_GPIOGRST  = (1U << RCC_AHB1RSTR_GPIOGRST_POS),    // IO port G reset
    RCC_AHB1RSTR_GPIOFRST  = (1U << RCC_AHB1RSTR_GPIOFRST_POS),    // IO port F reset
    RCC_AHB1RSTR_GPIOERST  = (1U << RCC_AHB1RSTR_GPIOERST_POS),    // IO port E reset
    RCC_AHB1RSTR_GPIODRST  = (1U << RCC_AHB1RSTR_GPIODRST_POS),    // IO port D reset
    RCC_AHB1RSTR_GPIOCRST  = (1U << RCC_AHB1RSTR_GPIOCRST_POS),    // IO port C reset
    RCC_AHB1RSTR_GPIOBRST  = (1U << RCC_AHB1RSTR_GPIOBRST_POS),    // IO port B reset
    RCC_AHB1RSTR_GPIOARST  = (1U << RCC_AHB1RSTR_GPIOARST_POS),    // IO port A reset
} rcc_ahb1rstr_e;

typedef enum : uint32_t {
    RCC_AHB1RSTR_OTGHSRST_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_ETHMACRST_MASK = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_DMA2RST_MASK   = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_DMA1RST_MASK   = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_CRCRST_MASK    = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_GPIOIRST_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_GPIOHRST_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_GPIOGRST_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_GPIOFRST_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_GPIOERST_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_GPIODRST_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_GPIOCRST_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_GPIOBRST_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1RSTR_GPIOARST_MASK  = 0b1U,   // 1 bit(s)
} rcc_ahb1rstr_mask_e;

typedef enum : uint32_t {
    RCC_AHB2RSTR_OTGFSRST_POS = 7U,   // USB OTG FS module reset
    RCC_AHB2RSTR_RNGRST_POS   = 6U,   // Random number generator module reset
    RCC_AHB2RSTR_DCMIRST_POS  = 0U,   // Camera interface reset
} rcc_ahb2rstr_pos_e;

typedef enum : uint32_t {
    RCC_AHB2RSTR_OTGFSRST = (1U << RCC_AHB2RSTR_OTGFSRST_POS),   // USB OTG FS module reset
    RCC_AHB2RSTR_RNGRST   = (1U << RCC_AHB2RSTR_RNGRST_POS),     // Random number generator module reset
    RCC_AHB2RSTR_DCMIRST  = (1U << RCC_AHB2RSTR_DCMIRST_POS),    // Camera interface reset
} rcc_ahb2rstr_e;

typedef enum : uint32_t {
    RCC_AHB2RSTR_OTGFSRST_MASK = 0b1U,   // 1 bit(s)
    RCC_AHB2RSTR_RNGRST_MASK   = 0b1U,   // 1 bit(s)
    RCC_AHB2RSTR_DCMIRST_MASK  = 0b1U,   // 1 bit(s)
} rcc_ahb2rstr_mask_e;

typedef enum : uint32_t {
    RCC_AHB3RSTR_FSMCRST_POS = 0U,   // Flexible static memory controller module reset
} rcc_ahb3rstr_pos_e;

typedef enum : uint32_t {
    RCC_AHB3RSTR_FSMCRST = (1U << RCC_AHB3RSTR_FSMCRST_POS),   // Flexible static memory controller module reset
} rcc_ahb3rstr_e;

typedef enum : uint32_t {
    RCC_AHB3RSTR_FSMCRST_MASK = 0b1U,   // 1 bit(s)
} rcc_ahb3rstr_mask_e;

typedef enum : uint32_t {
    RCC_APB1RSTR_DACRST_POS   = 29U,   // DAC reset
    RCC_APB1RSTR_PWRRST_POS   = 28U,   // Power interface reset
    RCC_APB1RSTR_CAN2RST_POS  = 26U,   // CAN2 reset
    RCC_APB1RSTR_CAN1RST_POS  = 25U,   // CAN1 reset
    RCC_APB1RSTR_I2C3RST_POS  = 23U,   // I2C3 reset
    RCC_APB1RSTR_I2C2RST_POS  = 22U,   // I2C 2 reset
    RCC_APB1RSTR_I2C1RST_POS  = 21U,   // I2C 1 reset
    RCC_APB1RSTR_UART5RST_POS = 20U,   // USART 5 reset
    RCC_APB1RSTR_UART4RST_POS = 19U,   // USART 4 reset
    RCC_APB1RSTR_UART3RST_POS = 18U,   // USART 3 reset
    RCC_APB1RSTR_UART2RST_POS = 17U,   // USART 2 reset
    RCC_APB1RSTR_SPI3RST_POS  = 15U,   // SPI 3 reset
    RCC_APB1RSTR_SPI2RST_POS  = 14U,   // SPI 2 reset
    RCC_APB1RSTR_WWDGRST_POS  = 11U,   // Window watchdog reset
    RCC_APB1RSTR_TIM14RST_POS = 8U,    // TIM14 reset
    RCC_APB1RSTR_TIM13RST_POS = 7U,    // TIM13 reset
    RCC_APB1RSTR_TIM12RST_POS = 6U,    // TIM12 reset
    RCC_APB1RSTR_TIM7RST_POS  = 5U,    // TIM7 reset
    RCC_APB1RSTR_TIM6RST_POS  = 4U,    // TIM6 reset
    RCC_APB1RSTR_TIM5RST_POS  = 3U,    // TIM5 reset
    RCC_APB1RSTR_TIM4RST_POS  = 2U,    // TIM4 reset
    RCC_APB1RSTR_TIM3RST_POS  = 1U,    // TIM3 reset
    RCC_APB1RSTR_TIM2RST_POS  = 0U,    // TIM2 reset
} rcc_apb1rstr_pos_e;

typedef enum : uint32_t {
    RCC_APB1RSTR_DACRST   = (1U << RCC_APB1RSTR_DACRST_POS),     // DAC reset
    RCC_APB1RSTR_PWRRST   = (1U << RCC_APB1RSTR_PWRRST_POS),     // Power interface reset
    RCC_APB1RSTR_CAN2RST  = (1U << RCC_APB1RSTR_CAN2RST_POS),    // CAN2 reset
    RCC_APB1RSTR_CAN1RST  = (1U << RCC_APB1RSTR_CAN1RST_POS),    // CAN1 reset
    RCC_APB1RSTR_I2C3RST  = (1U << RCC_APB1RSTR_I2C3RST_POS),    // I2C3 reset
    RCC_APB1RSTR_I2C2RST  = (1U << RCC_APB1RSTR_I2C2RST_POS),    // I2C 2 reset
    RCC_APB1RSTR_I2C1RST  = (1U << RCC_APB1RSTR_I2C1RST_POS),    // I2C 1 reset
    RCC_APB1RSTR_UART5RST = (1U << RCC_APB1RSTR_UART5RST_POS),   // USART 5 reset
    RCC_APB1RSTR_UART4RST = (1U << RCC_APB1RSTR_UART4RST_POS),   // USART 4 reset
    RCC_APB1RSTR_UART3RST = (1U << RCC_APB1RSTR_UART3RST_POS),   // USART 3 reset
    RCC_APB1RSTR_UART2RST = (1U << RCC_APB1RSTR_UART2RST_POS),   // USART 2 reset
    RCC_APB1RSTR_SPI3RST  = (1U << RCC_APB1RSTR_SPI3RST_POS),    // SPI 3 reset
    RCC_APB1RSTR_SPI2RST  = (1U << RCC_APB1RSTR_SPI2RST_POS),    // SPI 2 reset
    RCC_APB1RSTR_WWDGRST  = (1U << RCC_APB1RSTR_WWDGRST_POS),    // Window watchdog reset
    RCC_APB1RSTR_TIM14RST = (1U << RCC_APB1RSTR_TIM14RST_POS),   // TIM14 reset
    RCC_APB1RSTR_TIM13RST = (1U << RCC_APB1RSTR_TIM13RST_POS),   // TIM13 reset
    RCC_APB1RSTR_TIM12RST = (1U << RCC_APB1RSTR_TIM12RST_POS),   // TIM12 reset
    RCC_APB1RSTR_TIM7RST  = (1U << RCC_APB1RSTR_TIM7RST_POS),    // TIM7 reset
    RCC_APB1RSTR_TIM6RST  = (1U << RCC_APB1RSTR_TIM6RST_POS),    // TIM6 reset
    RCC_APB1RSTR_TIM5RST  = (1U << RCC_APB1RSTR_TIM5RST_POS),    // TIM5 reset
    RCC_APB1RSTR_TIM4RST  = (1U << RCC_APB1RSTR_TIM4RST_POS),    // TIM4 reset
    RCC_APB1RSTR_TIM3RST  = (1U << RCC_APB1RSTR_TIM3RST_POS),    // TIM3 reset
    RCC_APB1RSTR_TIM2RST  = (1U << RCC_APB1RSTR_TIM2RST_POS),    // TIM2 reset
} rcc_apb1rstr_e;

typedef enum : uint32_t {
    RCC_APB1RSTR_DACRST_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_PWRRST_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_CAN2RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_CAN1RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_I2C3RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_I2C2RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_I2C1RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_UART5RST_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_UART4RST_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_UART3RST_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_UART2RST_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_SPI3RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_SPI2RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_WWDGRST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_TIM14RST_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_TIM13RST_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_TIM12RST_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_TIM7RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_TIM6RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_TIM5RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_TIM4RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_TIM3RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1RSTR_TIM2RST_MASK  = 0b1U,   // 1 bit(s)
} rcc_apb1rstr_mask_e;

typedef enum : uint32_t {
    RCC_APB2RSTR_TIM11RST_POS  = 18U,   // TIM11 reset
    RCC_APB2RSTR_TIM10RST_POS  = 17U,   // TIM10 reset
    RCC_APB2RSTR_TIM9RST_POS   = 16U,   // TIM9 reset
    RCC_APB2RSTR_SYSCFGRST_POS = 14U,   // System configuration controller reset
    RCC_APB2RSTR_SPI1RST_POS   = 12U,   // SPI 1 reset
    RCC_APB2RSTR_SDIORST_POS   = 11U,   // SDIO reset
    RCC_APB2RSTR_ADCRST_POS    = 8U,    // ADC interface reset (common to all ADCs)
    RCC_APB2RSTR_USART6RST_POS = 5U,    // USART6 reset
    RCC_APB2RSTR_USART1RST_POS = 4U,    // USART1 reset
    RCC_APB2RSTR_TIM8RST_POS   = 1U,    // TIM8 reset
    RCC_APB2RSTR_TIM1RST_POS   = 0U,    // TIM1 reset
} rcc_apb2rstr_pos_e;

typedef enum : uint32_t {
    RCC_APB2RSTR_TIM11RST  = (1U << RCC_APB2RSTR_TIM11RST_POS),    // TIM11 reset
    RCC_APB2RSTR_TIM10RST  = (1U << RCC_APB2RSTR_TIM10RST_POS),    // TIM10 reset
    RCC_APB2RSTR_TIM9RST   = (1U << RCC_APB2RSTR_TIM9RST_POS),     // TIM9 reset
    RCC_APB2RSTR_SYSCFGRST = (1U << RCC_APB2RSTR_SYSCFGRST_POS),   // System configuration controller reset
    RCC_APB2RSTR_SPI1RST   = (1U << RCC_APB2RSTR_SPI1RST_POS),     // SPI 1 reset
    RCC_APB2RSTR_SDIORST   = (1U << RCC_APB2RSTR_SDIORST_POS),     // SDIO reset
    RCC_APB2RSTR_ADCRST    = (1U << RCC_APB2RSTR_ADCRST_POS),      // ADC interface reset (common to all ADCs)
    RCC_APB2RSTR_USART6RST = (1U << RCC_APB2RSTR_USART6RST_POS),   // USART6 reset
    RCC_APB2RSTR_USART1RST = (1U << RCC_APB2RSTR_USART1RST_POS),   // USART1 reset
    RCC_APB2RSTR_TIM8RST   = (1U << RCC_APB2RSTR_TIM8RST_POS),     // TIM8 reset
    RCC_APB2RSTR_TIM1RST   = (1U << RCC_APB2RSTR_TIM1RST_POS),     // TIM1 reset
} rcc_apb2rstr_e;

typedef enum : uint32_t {
    RCC_APB2RSTR_TIM11RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB2RSTR_TIM10RST_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB2RSTR_TIM9RST_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2RSTR_SYSCFGRST_MASK = 0b1U,   // 1 bit(s)
    RCC_APB2RSTR_SPI1RST_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2RSTR_SDIORST_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2RSTR_ADCRST_MASK    = 0b1U,   // 1 bit(s)
    RCC_APB2RSTR_USART6RST_MASK = 0b1U,   // 1 bit(s)
    RCC_APB2RSTR_USART1RST_MASK = 0b1U,   // 1 bit(s)
    RCC_APB2RSTR_TIM8RST_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2RSTR_TIM1RST_MASK   = 0b1U,   // 1 bit(s)
} rcc_apb2rstr_mask_e;

typedef enum : uint32_t {
    RCC_AHB1ENR_OTGHSULPIEN_POS  = 30U,   // USB OTG HSULPI clock enable
    RCC_AHB1ENR_OTGHSEN_POS      = 29U,   // USB OTG HS clock enable
    RCC_AHB1ENR_ETHMACPTPEN_POS  = 28U,   // Ethernet PTP clock enable
    RCC_AHB1ENR_ETHMACRXEN_POS   = 27U,   // Ethernet Reception clock enable
    RCC_AHB1ENR_ETHMACTXEN_POS   = 26U,   // Ethernet Transmission clock enable
    RCC_AHB1ENR_ETHMACEN_POS     = 25U,   // Ethernet MAC clock enable
    RCC_AHB1ENR_DMA2EN_POS       = 22U,   // DMA2 clock enable
    RCC_AHB1ENR_DMA1EN_POS       = 21U,   // DMA1 clock enable
    RCC_AHB1ENR_CCMDATARAMEN_POS = 20U,   // CCM data RAM clock enable
    RCC_AHB1ENR_BKPSRAMEN_POS    = 18U,   // Backup SRAM interface clock enable
    RCC_AHB1ENR_CRCEN_POS        = 12U,   // CRC clock enable
    RCC_AHB1ENR_GPIOIEN_POS      = 8U,    // IO port I clock enable
    RCC_AHB1ENR_GPIOHEN_POS      = 7U,    // IO port H clock enable
    RCC_AHB1ENR_GPIOGEN_POS      = 6U,    // IO port G clock enable
    RCC_AHB1ENR_GPIOFEN_POS      = 5U,    // IO port F clock enable
    RCC_AHB1ENR_GPIOEEN_POS      = 4U,    // IO port E clock enable
    RCC_AHB1ENR_GPIODEN_POS      = 3U,    // IO port D clock enable
    RCC_AHB1ENR_GPIOCEN_POS      = 2U,    // IO port C clock enable
    RCC_AHB1ENR_GPIOBEN_POS      = 1U,    // IO port B clock enable
    RCC_AHB1ENR_GPIOAEN_POS      = 0U,    // IO port A clock enable
} rcc_ahb1enr_pos_e;

typedef enum : uint32_t {
    RCC_AHB1ENR_OTGHSULPIEN  = (1U << RCC_AHB1ENR_OTGHSULPIEN_POS),    // USB OTG HSULPI clock enable
    RCC_AHB1ENR_OTGHSEN      = (1U << RCC_AHB1ENR_OTGHSEN_POS),        // USB OTG HS clock enable
    RCC_AHB1ENR_ETHMACPTPEN  = (1U << RCC_AHB1ENR_ETHMACPTPEN_POS),    // Ethernet PTP clock enable
    RCC_AHB1ENR_ETHMACRXEN   = (1U << RCC_AHB1ENR_ETHMACRXEN_POS),     // Ethernet Reception clock enable
    RCC_AHB1ENR_ETHMACTXEN   = (1U << RCC_AHB1ENR_ETHMACTXEN_POS),     // Ethernet Transmission clock enable
    RCC_AHB1ENR_ETHMACEN     = (1U << RCC_AHB1ENR_ETHMACEN_POS),       // Ethernet MAC clock enable
    RCC_AHB1ENR_DMA2EN       = (1U << RCC_AHB1ENR_DMA2EN_POS),         // DMA2 clock enable
    RCC_AHB1ENR_DMA1EN       = (1U << RCC_AHB1ENR_DMA1EN_POS),         // DMA1 clock enable
    RCC_AHB1ENR_CCMDATARAMEN = (1U << RCC_AHB1ENR_CCMDATARAMEN_POS),   // CCM data RAM clock enable
    RCC_AHB1ENR_BKPSRAMEN    = (1U << RCC_AHB1ENR_BKPSRAMEN_POS),      // Backup SRAM interface clock enable
    RCC_AHB1ENR_CRCEN        = (1U << RCC_AHB1ENR_CRCEN_POS),          // CRC clock enable
    RCC_AHB1ENR_GPIOIEN      = (1U << RCC_AHB1ENR_GPIOIEN_POS),        // IO port I clock enable
    RCC_AHB1ENR_GPIOHEN      = (1U << RCC_AHB1ENR_GPIOHEN_POS),        // IO port H clock enable
    RCC_AHB1ENR_GPIOGEN      = (1U << RCC_AHB1ENR_GPIOGEN_POS),        // IO port G clock enable
    RCC_AHB1ENR_GPIOFEN      = (1U << RCC_AHB1ENR_GPIOFEN_POS),        // IO port F clock enable
    RCC_AHB1ENR_GPIOEEN      = (1U << RCC_AHB1ENR_GPIOEEN_POS),        // IO port E clock enable
    RCC_AHB1ENR_GPIODEN      = (1U << RCC_AHB1ENR_GPIODEN_POS),        // IO port D clock enable
    RCC_AHB1ENR_GPIOCEN      = (1U << RCC_AHB1ENR_GPIOCEN_POS),        // IO port C clock enable
    RCC_AHB1ENR_GPIOBEN      = (1U << RCC_AHB1ENR_GPIOBEN_POS),        // IO port B clock enable
    RCC_AHB1ENR_GPIOAEN      = (1U << RCC_AHB1ENR_GPIOAEN_POS),        // IO port A clock enable
} rcc_ahb1enr_e;

typedef enum : uint32_t {
    RCC_AHB1ENR_OTGHSULPIEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_OTGHSEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_ETHMACPTPEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_ETHMACRXEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_ETHMACTXEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_ETHMACEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_DMA2EN_MASK       = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_DMA1EN_MASK       = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_CCMDATARAMEN_MASK = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_BKPSRAMEN_MASK    = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_CRCEN_MASK        = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_GPIOIEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_GPIOHEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_GPIOGEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_GPIOFEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_GPIOEEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_GPIODEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_GPIOCEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_GPIOBEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1ENR_GPIOAEN_MASK      = 0b1U,   // 1 bit(s)
} rcc_ahb1enr_mask_e;

typedef enum : uint32_t {
    RCC_AHB2ENR_OTGFSEN_POS = 7U,   // USB OTG FS clock enable
    RCC_AHB2ENR_RNGEN_POS   = 6U,   // Random number generator clock enable
    RCC_AHB2ENR_DCMIEN_POS  = 0U,   // Camera interface enable
} rcc_ahb2enr_pos_e;

typedef enum : uint32_t {
    RCC_AHB2ENR_OTGFSEN = (1U << RCC_AHB2ENR_OTGFSEN_POS),   // USB OTG FS clock enable
    RCC_AHB2ENR_RNGEN   = (1U << RCC_AHB2ENR_RNGEN_POS),     // Random number generator clock enable
    RCC_AHB2ENR_DCMIEN  = (1U << RCC_AHB2ENR_DCMIEN_POS),    // Camera interface enable
} rcc_ahb2enr_e;

typedef enum : uint32_t {
    RCC_AHB2ENR_OTGFSEN_MASK = 0b1U,   // 1 bit(s)
    RCC_AHB2ENR_RNGEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_AHB2ENR_DCMIEN_MASK  = 0b1U,   // 1 bit(s)
} rcc_ahb2enr_mask_e;

typedef enum : uint32_t {
    RCC_AHB3ENR_FSMCEN_POS = 0U,   // Flexible static memory controller module clock enable
} rcc_ahb3enr_pos_e;

typedef enum : uint32_t {
    RCC_AHB3ENR_FSMCEN = (1U << RCC_AHB3ENR_FSMCEN_POS),   // Flexible static memory controller module clock enable
} rcc_ahb3enr_e;

typedef enum : uint32_t {
    RCC_AHB3ENR_FSMCEN_MASK = 0b1U,   // 1 bit(s)
} rcc_ahb3enr_mask_e;

typedef enum : uint32_t {
    RCC_APB1ENR_DACEN_POS    = 29U,   // DAC interface clock enable
    RCC_APB1ENR_PWREN_POS    = 28U,   // Power interface clock enable
    RCC_APB1ENR_CAN2EN_POS   = 26U,   // CAN 2 clock enable
    RCC_APB1ENR_CAN1EN_POS   = 25U,   // CAN 1 clock enable
    RCC_APB1ENR_I2C3EN_POS   = 23U,   // I2C3 clock enable
    RCC_APB1ENR_I2C2EN_POS   = 22U,   // I2C2 clock enable
    RCC_APB1ENR_I2C1EN_POS   = 21U,   // I2C1 clock enable
    RCC_APB1ENR_UART5EN_POS  = 20U,   // UART5 clock enable
    RCC_APB1ENR_UART4EN_POS  = 19U,   // UART4 clock enable
    RCC_APB1ENR_USART3EN_POS = 18U,   // USART3 clock enable
    RCC_APB1ENR_USART2EN_POS = 17U,   // USART 2 clock enable
    RCC_APB1ENR_SPI3EN_POS   = 15U,   // SPI3 clock enable
    RCC_APB1ENR_SPI2EN_POS   = 14U,   // SPI2 clock enable
    RCC_APB1ENR_WWDGEN_POS   = 11U,   // Window watchdog clock enable
    RCC_APB1ENR_TIM14EN_POS  = 8U,    // TIM14 clock enable
    RCC_APB1ENR_TIM13EN_POS  = 7U,    // TIM13 clock enable
    RCC_APB1ENR_TIM12EN_POS  = 6U,    // TIM12 clock enable
    RCC_APB1ENR_TIM7EN_POS   = 5U,    // TIM7 clock enable
    RCC_APB1ENR_TIM6EN_POS   = 4U,    // TIM6 clock enable
    RCC_APB1ENR_TIM5EN_POS   = 3U,    // TIM5 clock enable
    RCC_APB1ENR_TIM4EN_POS   = 2U,    // TIM4 clock enable
    RCC_APB1ENR_TIM3EN_POS   = 1U,    // TIM3 clock enable
    RCC_APB1ENR_TIM2EN_POS   = 0U,    // TIM2 clock enable
} rcc_apb1enr_pos_e;

typedef enum : uint32_t {
    RCC_APB1ENR_DACEN    = (1U << RCC_APB1ENR_DACEN_POS),      // DAC interface clock enable
    RCC_APB1ENR_PWREN    = (1U << RCC_APB1ENR_PWREN_POS),      // Power interface clock enable
    RCC_APB1ENR_CAN2EN   = (1U << RCC_APB1ENR_CAN2EN_POS),     // CAN 2 clock enable
    RCC_APB1ENR_CAN1EN   = (1U << RCC_APB1ENR_CAN1EN_POS),     // CAN 1 clock enable
    RCC_APB1ENR_I2C3EN   = (1U << RCC_APB1ENR_I2C3EN_POS),     // I2C3 clock enable
    RCC_APB1ENR_I2C2EN   = (1U << RCC_APB1ENR_I2C2EN_POS),     // I2C2 clock enable
    RCC_APB1ENR_I2C1EN   = (1U << RCC_APB1ENR_I2C1EN_POS),     // I2C1 clock enable
    RCC_APB1ENR_UART5EN  = (1U << RCC_APB1ENR_UART5EN_POS),    // UART5 clock enable
    RCC_APB1ENR_UART4EN  = (1U << RCC_APB1ENR_UART4EN_POS),    // UART4 clock enable
    RCC_APB1ENR_USART3EN = (1U << RCC_APB1ENR_USART3EN_POS),   // USART3 clock enable
    RCC_APB1ENR_USART2EN = (1U << RCC_APB1ENR_USART2EN_POS),   // USART 2 clock enable
    RCC_APB1ENR_SPI3EN   = (1U << RCC_APB1ENR_SPI3EN_POS),     // SPI3 clock enable
    RCC_APB1ENR_SPI2EN   = (1U << RCC_APB1ENR_SPI2EN_POS),     // SPI2 clock enable
    RCC_APB1ENR_WWDGEN   = (1U << RCC_APB1ENR_WWDGEN_POS),     // Window watchdog clock enable
    RCC_APB1ENR_TIM14EN  = (1U << RCC_APB1ENR_TIM14EN_POS),    // TIM14 clock enable
    RCC_APB1ENR_TIM13EN  = (1U << RCC_APB1ENR_TIM13EN_POS),    // TIM13 clock enable
    RCC_APB1ENR_TIM12EN  = (1U << RCC_APB1ENR_TIM12EN_POS),    // TIM12 clock enable
    RCC_APB1ENR_TIM7EN   = (1U << RCC_APB1ENR_TIM7EN_POS),     // TIM7 clock enable
    RCC_APB1ENR_TIM6EN   = (1U << RCC_APB1ENR_TIM6EN_POS),     // TIM6 clock enable
    RCC_APB1ENR_TIM5EN   = (1U << RCC_APB1ENR_TIM5EN_POS),     // TIM5 clock enable
    RCC_APB1ENR_TIM4EN   = (1U << RCC_APB1ENR_TIM4EN_POS),     // TIM4 clock enable
    RCC_APB1ENR_TIM3EN   = (1U << RCC_APB1ENR_TIM3EN_POS),     // TIM3 clock enable
    RCC_APB1ENR_TIM2EN   = (1U << RCC_APB1ENR_TIM2EN_POS),     // TIM2 clock enable
} rcc_apb1enr_e;

typedef enum : uint32_t {
    RCC_APB1ENR_DACEN_MASK    = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_PWREN_MASK    = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_CAN2EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_CAN1EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_I2C3EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_I2C2EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_I2C1EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_UART5EN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_UART4EN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_USART3EN_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_USART2EN_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_SPI3EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_SPI2EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_WWDGEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_TIM14EN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_TIM13EN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_TIM12EN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_TIM7EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_TIM6EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_TIM5EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_TIM4EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_TIM3EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1ENR_TIM2EN_MASK   = 0b1U,   // 1 bit(s)
} rcc_apb1enr_mask_e;

typedef enum : uint32_t {
    RCC_APB2ENR_TIM11EN_POS  = 18U,   // TIM11 clock enable
    RCC_APB2ENR_TIM10EN_POS  = 17U,   // TIM10 clock enable
    RCC_APB2ENR_TIM9EN_POS   = 16U,   // TIM9 clock enable
    RCC_APB2ENR_SYSCFGEN_POS = 14U,   // System configuration controller clock enable
    RCC_APB2ENR_SPI1EN_POS   = 12U,   // SPI1 clock enable
    RCC_APB2ENR_SDIOEN_POS   = 11U,   // SDIO clock enable
    RCC_APB2ENR_ADC3EN_POS   = 10U,   // ADC3 clock enable
    RCC_APB2ENR_ADC2EN_POS   = 9U,    // ADC2 clock enable
    RCC_APB2ENR_ADC1EN_POS   = 8U,    // ADC1 clock enable
    RCC_APB2ENR_USART6EN_POS = 5U,    // USART6 clock enable
    RCC_APB2ENR_USART1EN_POS = 4U,    // USART1 clock enable
    RCC_APB2ENR_TIM8EN_POS   = 1U,    // TIM8 clock enable
    RCC_APB2ENR_TIM1EN_POS   = 0U,    // TIM1 clock enable
} rcc_apb2enr_pos_e;

typedef enum : uint32_t {
    RCC_APB2ENR_TIM11EN  = (1U << RCC_APB2ENR_TIM11EN_POS),    // TIM11 clock enable
    RCC_APB2ENR_TIM10EN  = (1U << RCC_APB2ENR_TIM10EN_POS),    // TIM10 clock enable
    RCC_APB2ENR_TIM9EN   = (1U << RCC_APB2ENR_TIM9EN_POS),     // TIM9 clock enable
    RCC_APB2ENR_SYSCFGEN = (1U << RCC_APB2ENR_SYSCFGEN_POS),   // System configuration controller clock enable
    RCC_APB2ENR_SPI1EN   = (1U << RCC_APB2ENR_SPI1EN_POS),     // SPI1 clock enable
    RCC_APB2ENR_SDIOEN   = (1U << RCC_APB2ENR_SDIOEN_POS),     // SDIO clock enable
    RCC_APB2ENR_ADC3EN   = (1U << RCC_APB2ENR_ADC3EN_POS),     // ADC3 clock enable
    RCC_APB2ENR_ADC2EN   = (1U << RCC_APB2ENR_ADC2EN_POS),     // ADC2 clock enable
    RCC_APB2ENR_ADC1EN   = (1U << RCC_APB2ENR_ADC1EN_POS),     // ADC1 clock enable
    RCC_APB2ENR_USART6EN = (1U << RCC_APB2ENR_USART6EN_POS),   // USART6 clock enable
    RCC_APB2ENR_USART1EN = (1U << RCC_APB2ENR_USART1EN_POS),   // USART1 clock enable
    RCC_APB2ENR_TIM8EN   = (1U << RCC_APB2ENR_TIM8EN_POS),     // TIM8 clock enable
    RCC_APB2ENR_TIM1EN   = (1U << RCC_APB2ENR_TIM1EN_POS),     // TIM1 clock enable
} rcc_apb2enr_e;

typedef enum : uint32_t {
    RCC_APB2ENR_TIM11EN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_TIM10EN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_TIM9EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_SYSCFGEN_MASK = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_SPI1EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_SDIOEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_ADC3EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_ADC2EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_ADC1EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_USART6EN_MASK = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_USART1EN_MASK = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_TIM8EN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2ENR_TIM1EN_MASK   = 0b1U,   // 1 bit(s)
} rcc_apb2enr_mask_e;

typedef enum : uint32_t {
    RCC_AHB1LPENR_OTGHSULPILPEN_POS = 30U,   // USB OTG HS ULPI clock enable during Sleep mode
    RCC_AHB1LPENR_OTGHSLPEN_POS     = 29U,   // USB OTG HS clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACPTPLPEN_POS = 28U,   // Ethernet PTP clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACRXLPEN_POS  = 27U,   // Ethernet reception clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACTXLPEN_POS  = 26U,   // Ethernet transmission clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACLPEN_POS    = 25U,   // Ethernet MAC clock enable during Sleep mode
    RCC_AHB1LPENR_DMA2LPEN_POS      = 22U,   // DMA2 clock enable during Sleep mode
    RCC_AHB1LPENR_DMA1LPEN_POS      = 21U,   // DMA1 clock enable during Sleep mode
    RCC_AHB1LPENR_BKPSRAMLPEN_POS   = 18U,   // Backup SRAM interface clock enable during Sleep mode
    RCC_AHB1LPENR_SRAM2LPEN_POS     = 17U,   // SRAM 2 interface clock enable during Sleep mode
    RCC_AHB1LPENR_SRAM1LPEN_POS     = 16U,   // SRAM 1interface clock enable during Sleep mode
    RCC_AHB1LPENR_FLITFLPEN_POS     = 15U,   // Flash interface clock enable during Sleep mode
    RCC_AHB1LPENR_CRCLPEN_POS       = 12U,   // CRC clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOILPEN_POS     = 8U,    // IO port I clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOHLPEN_POS     = 7U,    // IO port H clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOGLPEN_POS     = 6U,    // IO port G clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOFLPEN_POS     = 5U,    // IO port F clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOELPEN_POS     = 4U,    // IO port E clock enable during Sleep mode
    RCC_AHB1LPENR_GPIODLPEN_POS     = 3U,    // IO port D clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOCLPEN_POS     = 2U,    // IO port C clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOBLPEN_POS     = 1U,    // IO port B clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOALPEN_POS     = 0U,    // IO port A clock enable during sleep mode
} rcc_ahb1lpenr_pos_e;

typedef enum : uint32_t {
    RCC_AHB1LPENR_OTGHSULPILPEN = (1U << RCC_AHB1LPENR_OTGHSULPILPEN_POS),   // USB OTG HS ULPI clock enable during Sleep mode
    RCC_AHB1LPENR_OTGHSLPEN     = (1U << RCC_AHB1LPENR_OTGHSLPEN_POS),       // USB OTG HS clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACPTPLPEN = (1U << RCC_AHB1LPENR_ETHMACPTPLPEN_POS),   // Ethernet PTP clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACRXLPEN  = (1U << RCC_AHB1LPENR_ETHMACRXLPEN_POS),    // Ethernet reception clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACTXLPEN  = (1U << RCC_AHB1LPENR_ETHMACTXLPEN_POS),    // Ethernet transmission clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACLPEN    = (1U << RCC_AHB1LPENR_ETHMACLPEN_POS),      // Ethernet MAC clock enable during Sleep mode
    RCC_AHB1LPENR_DMA2LPEN      = (1U << RCC_AHB1LPENR_DMA2LPEN_POS),        // DMA2 clock enable during Sleep mode
    RCC_AHB1LPENR_DMA1LPEN      = (1U << RCC_AHB1LPENR_DMA1LPEN_POS),        // DMA1 clock enable during Sleep mode
    RCC_AHB1LPENR_BKPSRAMLPEN   = (1U << RCC_AHB1LPENR_BKPSRAMLPEN_POS),     // Backup SRAM interface clock enable during Sleep mode
    RCC_AHB1LPENR_SRAM2LPEN     = (1U << RCC_AHB1LPENR_SRAM2LPEN_POS),       // SRAM 2 interface clock enable during Sleep mode
    RCC_AHB1LPENR_SRAM1LPEN     = (1U << RCC_AHB1LPENR_SRAM1LPEN_POS),       // SRAM 1interface clock enable during Sleep mode
    RCC_AHB1LPENR_FLITFLPEN     = (1U << RCC_AHB1LPENR_FLITFLPEN_POS),       // Flash interface clock enable during Sleep mode
    RCC_AHB1LPENR_CRCLPEN       = (1U << RCC_AHB1LPENR_CRCLPEN_POS),         // CRC clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOILPEN     = (1U << RCC_AHB1LPENR_GPIOILPEN_POS),       // IO port I clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOHLPEN     = (1U << RCC_AHB1LPENR_GPIOHLPEN_POS),       // IO port H clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOGLPEN     = (1U << RCC_AHB1LPENR_GPIOGLPEN_POS),       // IO port G clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOFLPEN     = (1U << RCC_AHB1LPENR_GPIOFLPEN_POS),       // IO port F clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOELPEN     = (1U << RCC_AHB1LPENR_GPIOELPEN_POS),       // IO port E clock enable during Sleep mode
    RCC_AHB1LPENR_GPIODLPEN     = (1U << RCC_AHB1LPENR_GPIODLPEN_POS),       // IO port D clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOCLPEN     = (1U << RCC_AHB1LPENR_GPIOCLPEN_POS),       // IO port C clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOBLPEN     = (1U << RCC_AHB1LPENR_GPIOBLPEN_POS),       // IO port B clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOALPEN     = (1U << RCC_AHB1LPENR_GPIOALPEN_POS),       // IO port A clock enable during sleep mode
} rcc_ahb1lpenr_e;

typedef enum : uint32_t {
    RCC_AHB1LPENR_OTGHSULPILPEN_MASK = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_OTGHSLPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_ETHMACPTPLPEN_MASK = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_ETHMACRXLPEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_ETHMACTXLPEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_ETHMACLPEN_MASK    = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_DMA2LPEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_DMA1LPEN_MASK      = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_BKPSRAMLPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_SRAM2LPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_SRAM1LPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_FLITFLPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_CRCLPEN_MASK       = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_GPIOILPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_GPIOHLPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_GPIOGLPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_GPIOFLPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_GPIOELPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_GPIODLPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_GPIOCLPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_GPIOBLPEN_MASK     = 0b1U,   // 1 bit(s)
    RCC_AHB1LPENR_GPIOALPEN_MASK     = 0b1U,   // 1 bit(s)
} rcc_ahb1lpenr_mask_e;

typedef enum : uint32_t {
    RCC_AHB2LPENR_OTGFSLPEN_POS = 7U,   // USB OTG FS clock enable during Sleep mode
    RCC_AHB2LPENR_RNGLPEN_POS   = 6U,   // Random number generator clock enable during Sleep mode
    RCC_AHB2LPENR_DCMILPEN_POS  = 0U,   // Camera interface enable during Sleep mode
} rcc_ahb2lpenr_pos_e;

typedef enum : uint32_t {
    RCC_AHB2LPENR_OTGFSLPEN = (1U << RCC_AHB2LPENR_OTGFSLPEN_POS),   // USB OTG FS clock enable during Sleep mode
    RCC_AHB2LPENR_RNGLPEN   = (1U << RCC_AHB2LPENR_RNGLPEN_POS),     // Random number generator clock enable during Sleep mode
    RCC_AHB2LPENR_DCMILPEN  = (1U << RCC_AHB2LPENR_DCMILPEN_POS),    // Camera interface enable during Sleep mode
} rcc_ahb2lpenr_e;

typedef enum : uint32_t {
    RCC_AHB2LPENR_OTGFSLPEN_MASK = 0b1U,   // 1 bit(s)
    RCC_AHB2LPENR_RNGLPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_AHB2LPENR_DCMILPEN_MASK  = 0b1U,   // 1 bit(s)
} rcc_ahb2lpenr_mask_e;

typedef enum : uint32_t {
    RCC_AHB3LPENR_FSMCLPEN_POS = 0U,   // Flexible static memory controller module clock enable during Sleep mode
} rcc_ahb3lpenr_pos_e;

typedef enum : uint32_t {
    RCC_AHB3LPENR_FSMCLPEN = (1U << RCC_AHB3LPENR_FSMCLPEN_POS),   // Flexible static memory controller module clock enable during Sleep mode
} rcc_ahb3lpenr_e;

typedef enum : uint32_t {
    RCC_AHB3LPENR_FSMCLPEN_MASK = 0b1U,   // 1 bit(s)
} rcc_ahb3lpenr_mask_e;

typedef enum : uint32_t {
    RCC_APB1LPENR_DACLPEN_POS    = 29U,   // DAC interface clock enable during Sleep mode
    RCC_APB1LPENR_PWRLPEN_POS    = 28U,   // Power interface clock enable during Sleep mode
    RCC_APB1LPENR_CAN2LPEN_POS   = 26U,   // CAN 2 clock enable during Sleep mode
    RCC_APB1LPENR_CAN1LPEN_POS   = 25U,   // CAN 1 clock enable during Sleep mode
    RCC_APB1LPENR_I2C3LPEN_POS   = 23U,   // I2C3 clock enable during Sleep mode
    RCC_APB1LPENR_I2C2LPEN_POS   = 22U,   // I2C2 clock enable during Sleep mode
    RCC_APB1LPENR_I2C1LPEN_POS   = 21U,   // I2C1 clock enable during Sleep mode
    RCC_APB1LPENR_UART5LPEN_POS  = 20U,   // UART5 clock enable during Sleep mode
    RCC_APB1LPENR_UART4LPEN_POS  = 19U,   // UART4 clock enable during Sleep mode
    RCC_APB1LPENR_USART3LPEN_POS = 18U,   // USART3 clock enable during Sleep mode
    RCC_APB1LPENR_USART2LPEN_POS = 17U,   // USART2 clock enable during Sleep mode
    RCC_APB1LPENR_SPI3LPEN_POS   = 15U,   // SPI3 clock enable during Sleep mode
    RCC_APB1LPENR_SPI2LPEN_POS   = 14U,   // SPI2 clock enable during Sleep mode
    RCC_APB1LPENR_WWDGLPEN_POS   = 11U,   // Window watchdog clock enable during Sleep mode
    RCC_APB1LPENR_TIM14LPEN_POS  = 8U,    // TIM14 clock enable during Sleep mode
    RCC_APB1LPENR_TIM13LPEN_POS  = 7U,    // TIM13 clock enable during Sleep mode
    RCC_APB1LPENR_TIM12LPEN_POS  = 6U,    // TIM12 clock enable during Sleep mode
    RCC_APB1LPENR_TIM7LPEN_POS   = 5U,    // TIM7 clock enable during Sleep mode
    RCC_APB1LPENR_TIM6LPEN_POS   = 4U,    // TIM6 clock enable during Sleep mode
    RCC_APB1LPENR_TIM5LPEN_POS   = 3U,    // TIM5 clock enable during Sleep mode
    RCC_APB1LPENR_TIM4LPEN_POS   = 2U,    // TIM4 clock enable during Sleep mode
    RCC_APB1LPENR_TIM3LPEN_POS   = 1U,    // TIM3 clock enable during Sleep mode
    RCC_APB1LPENR_TIM2LPEN_POS   = 0U,    // TIM2 clock enable during Sleep mode
} rcc_apb1lpenr_pos_e;

typedef enum : uint32_t {
    RCC_APB1LPENR_DACLPEN    = (1U << RCC_APB1LPENR_DACLPEN_POS),      // DAC interface clock enable during Sleep mode
    RCC_APB1LPENR_PWRLPEN    = (1U << RCC_APB1LPENR_PWRLPEN_POS),      // Power interface clock enable during Sleep mode
    RCC_APB1LPENR_CAN2LPEN   = (1U << RCC_APB1LPENR_CAN2LPEN_POS),     // CAN 2 clock enable during Sleep mode
    RCC_APB1LPENR_CAN1LPEN   = (1U << RCC_APB1LPENR_CAN1LPEN_POS),     // CAN 1 clock enable during Sleep mode
    RCC_APB1LPENR_I2C3LPEN   = (1U << RCC_APB1LPENR_I2C3LPEN_POS),     // I2C3 clock enable during Sleep mode
    RCC_APB1LPENR_I2C2LPEN   = (1U << RCC_APB1LPENR_I2C2LPEN_POS),     // I2C2 clock enable during Sleep mode
    RCC_APB1LPENR_I2C1LPEN   = (1U << RCC_APB1LPENR_I2C1LPEN_POS),     // I2C1 clock enable during Sleep mode
    RCC_APB1LPENR_UART5LPEN  = (1U << RCC_APB1LPENR_UART5LPEN_POS),    // UART5 clock enable during Sleep mode
    RCC_APB1LPENR_UART4LPEN  = (1U << RCC_APB1LPENR_UART4LPEN_POS),    // UART4 clock enable during Sleep mode
    RCC_APB1LPENR_USART3LPEN = (1U << RCC_APB1LPENR_USART3LPEN_POS),   // USART3 clock enable during Sleep mode
    RCC_APB1LPENR_USART2LPEN = (1U << RCC_APB1LPENR_USART2LPEN_POS),   // USART2 clock enable during Sleep mode
    RCC_APB1LPENR_SPI3LPEN   = (1U << RCC_APB1LPENR_SPI3LPEN_POS),     // SPI3 clock enable during Sleep mode
    RCC_APB1LPENR_SPI2LPEN   = (1U << RCC_APB1LPENR_SPI2LPEN_POS),     // SPI2 clock enable during Sleep mode
    RCC_APB1LPENR_WWDGLPEN   = (1U << RCC_APB1LPENR_WWDGLPEN_POS),     // Window watchdog clock enable during Sleep mode
    RCC_APB1LPENR_TIM14LPEN  = (1U << RCC_APB1LPENR_TIM14LPEN_POS),    // TIM14 clock enable during Sleep mode
    RCC_APB1LPENR_TIM13LPEN  = (1U << RCC_APB1LPENR_TIM13LPEN_POS),    // TIM13 clock enable during Sleep mode
    RCC_APB1LPENR_TIM12LPEN  = (1U << RCC_APB1LPENR_TIM12LPEN_POS),    // TIM12 clock enable during Sleep mode
    RCC_APB1LPENR_TIM7LPEN   = (1U << RCC_APB1LPENR_TIM7LPEN_POS),     // TIM7 clock enable during Sleep mode
    RCC_APB1LPENR_TIM6LPEN   = (1U << RCC_APB1LPENR_TIM6LPEN_POS),     // TIM6 clock enable during Sleep mode
    RCC_APB1LPENR_TIM5LPEN   = (1U << RCC_APB1LPENR_TIM5LPEN_POS),     // TIM5 clock enable during Sleep mode
    RCC_APB1LPENR_TIM4LPEN   = (1U << RCC_APB1LPENR_TIM4LPEN_POS),     // TIM4 clock enable during Sleep mode
    RCC_APB1LPENR_TIM3LPEN   = (1U << RCC_APB1LPENR_TIM3LPEN_POS),     // TIM3 clock enable during Sleep mode
    RCC_APB1LPENR_TIM2LPEN   = (1U << RCC_APB1LPENR_TIM2LPEN_POS),     // TIM2 clock enable during Sleep mode
} rcc_apb1lpenr_e;

typedef enum : uint32_t {
    RCC_APB1LPENR_DACLPEN_MASK    = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_PWRLPEN_MASK    = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_CAN2LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_CAN1LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_I2C3LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_I2C2LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_I2C1LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_UART5LPEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_UART4LPEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_USART3LPEN_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_USART2LPEN_MASK = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_SPI3LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_SPI2LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_WWDGLPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_TIM14LPEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_TIM13LPEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_TIM12LPEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_TIM7LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_TIM6LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_TIM5LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_TIM4LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_TIM3LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB1LPENR_TIM2LPEN_MASK   = 0b1U,   // 1 bit(s)
} rcc_apb1lpenr_mask_e;

typedef enum : uint32_t {
    RCC_APB2LPENR_TIM11LPEN_POS  = 18U,   // TIM11 clock enable during Sleep mode
    RCC_APB2LPENR_TIM10LPEN_POS  = 17U,   // TIM10 clock enable during Sleep mode
    RCC_APB2LPENR_TIM9LPEN_POS   = 16U,   // TIM9 clock enable during sleep mode
    RCC_APB2LPENR_SYSCFGLPEN_POS = 14U,   // System configuration controller clock enable during Sleep mode
    RCC_APB2LPENR_SPI1LPEN_POS   = 12U,   // SPI 1 clock enable during Sleep mode
    RCC_APB2LPENR_SDIOLPEN_POS   = 11U,   // SDIO clock enable during Sleep mode
    RCC_APB2LPENR_ADC3LPEN_POS   = 10U,   // ADC 3 clock enable during Sleep mode
    RCC_APB2LPENR_ADC2LPEN_POS   = 9U,    // ADC2 clock enable during Sleep mode
    RCC_APB2LPENR_ADC1LPEN_POS   = 8U,    // ADC1 clock enable during Sleep mode
    RCC_APB2LPENR_USART6LPEN_POS = 5U,    // USART6 clock enable during Sleep mode
    RCC_APB2LPENR_USART1LPEN_POS = 4U,    // USART1 clock enable during Sleep mode
    RCC_APB2LPENR_TIM8LPEN_POS   = 1U,    // TIM8 clock enable during Sleep mode
    RCC_APB2LPENR_TIM1LPEN_POS   = 0U,    // TIM1 clock enable during Sleep mode
} rcc_apb2lpenr_pos_e;

typedef enum : uint32_t {
    RCC_APB2LPENR_TIM11LPEN  = (1U << RCC_APB2LPENR_TIM11LPEN_POS),    // TIM11 clock enable during Sleep mode
    RCC_APB2LPENR_TIM10LPEN  = (1U << RCC_APB2LPENR_TIM10LPEN_POS),    // TIM10 clock enable during Sleep mode
    RCC_APB2LPENR_TIM9LPEN   = (1U << RCC_APB2LPENR_TIM9LPEN_POS),     // TIM9 clock enable during sleep mode
    RCC_APB2LPENR_SYSCFGLPEN = (1U << RCC_APB2LPENR_SYSCFGLPEN_POS),   // System configuration controller clock enable during Sleep mode
    RCC_APB2LPENR_SPI1LPEN   = (1U << RCC_APB2LPENR_SPI1LPEN_POS),     // SPI 1 clock enable during Sleep mode
    RCC_APB2LPENR_SDIOLPEN   = (1U << RCC_APB2LPENR_SDIOLPEN_POS),     // SDIO clock enable during Sleep mode
    RCC_APB2LPENR_ADC3LPEN   = (1U << RCC_APB2LPENR_ADC3LPEN_POS),     // ADC 3 clock enable during Sleep mode
    RCC_APB2LPENR_ADC2LPEN   = (1U << RCC_APB2LPENR_ADC2LPEN_POS),     // ADC2 clock enable during Sleep mode
    RCC_APB2LPENR_ADC1LPEN   = (1U << RCC_APB2LPENR_ADC1LPEN_POS),     // ADC1 clock enable during Sleep mode
    RCC_APB2LPENR_USART6LPEN = (1U << RCC_APB2LPENR_USART6LPEN_POS),   // USART6 clock enable during Sleep mode
    RCC_APB2LPENR_USART1LPEN = (1U << RCC_APB2LPENR_USART1LPEN_POS),   // USART1 clock enable during Sleep mode
    RCC_APB2LPENR_TIM8LPEN   = (1U << RCC_APB2LPENR_TIM8LPEN_POS),     // TIM8 clock enable during Sleep mode
    RCC_APB2LPENR_TIM1LPEN   = (1U << RCC_APB2LPENR_TIM1LPEN_POS),     // TIM1 clock enable during Sleep mode
} rcc_apb2lpenr_e;

typedef enum : uint32_t {
    RCC_APB2LPENR_TIM11LPEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_TIM10LPEN_MASK  = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_TIM9LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_SYSCFGLPEN_MASK = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_SPI1LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_SDIOLPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_ADC3LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_ADC2LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_ADC1LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_USART6LPEN_MASK = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_USART1LPEN_MASK = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_TIM8LPEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_APB2LPENR_TIM1LPEN_MASK   = 0b1U,   // 1 bit(s)
} rcc_apb2lpenr_mask_e;

typedef enum : uint32_t {
    RCC_BDCR_BDRST_POS   = 16U,   // Backup domain software reset
    RCC_BDCR_RTCEN_POS   = 15U,   // RTC clock enable
    RCC_BDCR_RTCSEL1_POS = 9U,    // RTC clock source selection
    RCC_BDCR_RTCSEL0_POS = 8U,    // RTC clock source selection
    RCC_BDCR_LSEBYP_POS  = 2U,    // External low-speed oscillator bypass
    RCC_BDCR_LSERDY_POS  = 1U,    // External low-speed oscillator ready
    RCC_BDCR_LSEON_POS   = 0U,    // External low-speed oscillator enable
} rcc_bdcr_pos_e;

typedef enum : uint32_t {
    RCC_BDCR_BDRST   = (1U << RCC_BDCR_BDRST_POS),     // Backup domain software reset
    RCC_BDCR_RTCEN   = (1U << RCC_BDCR_RTCEN_POS),     // RTC clock enable
    RCC_BDCR_RTCSEL1 = (1U << RCC_BDCR_RTCSEL1_POS),   // RTC clock source selection
    RCC_BDCR_RTCSEL0 = (1U << RCC_BDCR_RTCSEL0_POS),   // RTC clock source selection
    RCC_BDCR_LSEBYP  = (1U << RCC_BDCR_LSEBYP_POS),    // External low-speed oscillator bypass
    RCC_BDCR_LSERDY  = (1U << RCC_BDCR_LSERDY_POS),    // External low-speed oscillator ready
    RCC_BDCR_LSEON   = (1U << RCC_BDCR_LSEON_POS),     // External low-speed oscillator enable
} rcc_bdcr_e;

typedef enum : uint32_t {
    RCC_BDCR_BDRST_MASK   = 0b1U,   // 1 bit(s)
    RCC_BDCR_RTCEN_MASK   = 0b1U,   // 1 bit(s)
    RCC_BDCR_RTCSEL1_MASK = 0b1U,   // 1 bit(s)
    RCC_BDCR_RTCSEL0_MASK = 0b1U,   // 1 bit(s)
    RCC_BDCR_LSEBYP_MASK  = 0b1U,   // 1 bit(s)
    RCC_BDCR_LSERDY_MASK  = 0b1U,   // 1 bit(s)
    RCC_BDCR_LSEON_MASK   = 0b1U,   // 1 bit(s)
} rcc_bdcr_mask_e;

typedef enum : uint32_t {
    RCC_CSR_LPWRRSTF_POS = 31U,   // Low-power reset flag
    RCC_CSR_WWDGRSTF_POS = 30U,   // Window watchdog reset flag
    RCC_CSR_WDGRSTF_POS  = 29U,   // Independent watchdog reset flag
    RCC_CSR_SFTRSTF_POS  = 28U,   // Software reset flag
    RCC_CSR_PORRSTF_POS  = 27U,   // POR/PDR reset flag
    RCC_CSR_PADRSTF_POS  = 26U,   // PIN reset flag
    RCC_CSR_BORRSTF_POS  = 25U,   // BOR reset flag
    RCC_CSR_RMVF_POS     = 24U,   // Remove reset flag
    RCC_CSR_LSIRDY_POS   = 1U,    // Internal low-speed oscillator ready
    RCC_CSR_LSION_POS    = 0U,    // Internal low-speed oscillator enable
} rcc_csr_pos_e;

typedef enum : uint32_t {
    RCC_CSR_LPWRRSTF = (1U << RCC_CSR_LPWRRSTF_POS),   // Low-power reset flag
    RCC_CSR_WWDGRSTF = (1U << RCC_CSR_WWDGRSTF_POS),   // Window watchdog reset flag
    RCC_CSR_WDGRSTF  = (1U << RCC_CSR_WDGRSTF_POS),    // Independent watchdog reset flag
    RCC_CSR_SFTRSTF  = (1U << RCC_CSR_SFTRSTF_POS),    // Software reset flag
    RCC_CSR_PORRSTF  = (1U << RCC_CSR_PORRSTF_POS),    // POR/PDR reset flag
    RCC_CSR_PADRSTF  = (1U << RCC_CSR_PADRSTF_POS),    // PIN reset flag
    RCC_CSR_BORRSTF  = (1U << RCC_CSR_BORRSTF_POS),    // BOR reset flag
    RCC_CSR_RMVF     = (1U << RCC_CSR_RMVF_POS),       // Remove reset flag
    RCC_CSR_LSIRDY   = (1U << RCC_CSR_LSIRDY_POS),     // Internal low-speed oscillator ready
    RCC_CSR_LSION    = (1U << RCC_CSR_LSION_POS),      // Internal low-speed oscillator enable
} rcc_csr_e;

typedef enum : uint32_t {
    RCC_CSR_LPWRRSTF_MASK = 0b1U,   // 1 bit(s)
    RCC_CSR_WWDGRSTF_MASK = 0b1U,   // 1 bit(s)
    RCC_CSR_WDGRSTF_MASK  = 0b1U,   // 1 bit(s)
    RCC_CSR_SFTRSTF_MASK  = 0b1U,   // 1 bit(s)
    RCC_CSR_PORRSTF_MASK  = 0b1U,   // 1 bit(s)
    RCC_CSR_PADRSTF_MASK  = 0b1U,   // 1 bit(s)
    RCC_CSR_BORRSTF_MASK  = 0b1U,   // 1 bit(s)
    RCC_CSR_RMVF_MASK     = 0b1U,   // 1 bit(s)
    RCC_CSR_LSIRDY_MASK   = 0b1U,   // 1 bit(s)
    RCC_CSR_LSION_MASK    = 0b1U,   // 1 bit(s)
} rcc_csr_mask_e;

typedef enum : uint32_t {
    RCC_SSCGR_SSCGEN_POS    = 31U,   // Spread spectrum modulation enable
    RCC_SSCGR_SPREADSEL_POS = 30U,   // Spread Select
    RCC_SSCGR_INCSTEP_POS   = 13U,   // Incrementation step
    RCC_SSCGR_MODPER_POS    = 0U,    // Modulation period
} rcc_sscgr_pos_e;

typedef enum : uint32_t {
    RCC_SSCGR_SSCGEN    = (1U << RCC_SSCGR_SSCGEN_POS),      // Spread spectrum modulation enable
    RCC_SSCGR_SPREADSEL = (1U << RCC_SSCGR_SPREADSEL_POS),   // Spread Select
    RCC_SSCGR_INCSTEP   = (1U << RCC_SSCGR_INCSTEP_POS),     // Incrementation step
    RCC_SSCGR_MODPER    = (1U << RCC_SSCGR_MODPER_POS),      // Modulation period
} rcc_sscgr_e;

typedef enum : uint32_t {
    RCC_SSCGR_SSCGEN_MASK    = 0b1U,                 // 1 bit(s)
    RCC_SSCGR_SPREADSEL_MASK = 0b1U,                 // 1 bit(s)
    RCC_SSCGR_INCSTEP_MASK   = 0b111111111111111U,   // 15 bit(s)
    RCC_SSCGR_MODPER_MASK    = 0b1111111111111U,     // 13 bit(s)
} rcc_sscgr_mask_e;

typedef enum : uint32_t {
    RCC_PLLI2SCFGR_PLLI2SRX_POS = 28U,   // PLLI2S division factor for I2S clocks
    RCC_PLLI2SCFGR_PLLI2SNX_POS = 6U,    // PLLI2S multiplication factor for VCO
} rcc_plli2scfgr_pos_e;

typedef enum : uint32_t {
    RCC_PLLI2SCFGR_PLLI2SRX = (1U << RCC_PLLI2SCFGR_PLLI2SRX_POS),   // PLLI2S division factor for I2S clocks
    RCC_PLLI2SCFGR_PLLI2SNX = (1U << RCC_PLLI2SCFGR_PLLI2SNX_POS),   // PLLI2S multiplication factor for VCO
} rcc_plli2scfgr_e;

typedef enum : uint32_t {
    RCC_PLLI2SCFGR_PLLI2SRX_MASK = 0b111U,         // 3 bit(s)
    RCC_PLLI2SCFGR_PLLI2SNX_MASK = 0b111111111U,   // 9 bit(s)
} rcc_plli2scfgr_mask_e;

// clang-format on

//======================================================================================//}
//                  Private Function Prototypes
//======================================================================================//{

static void init_mco_gpio_pin(rcc_mco_sel_e mco_select);

//======================================================================================//}
//                  Public Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: rcc_lsi_config
//  Overview: Enable or disable the LSI oscillator.
//  Parameters:
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rcc_lsi_config(togglable_e toggle)
{
    switch (toggle)
    {
        case ENABLE:
            RCC->CSR |= RCC_CSR_LSION;
            break;
        case DISABLE:
            RCC->CSR &= ~RCC_CSR_LSION;
            break;
    }
}

//============================================================================//{
//  Function: rcc_lsi_is_rdy
//  Overview: Check if the LSI oscillator is ready.
//  Parameters:
//      None
//  Return:
//      true
//      false
//  Note: None
//============================================================================//}
bool rcc_lsi_is_rdy(void)
{
    return (RCC->CSR & RCC_CSR_LSIRDY) >> RCC_CSR_LSIRDY_POS;
}

//============================================================================//{
//  Function: rcc_gpio_togglable_clock
//  Overview: Enable or disable clock to the specified GPIO peripheral.
//  Parameters:
//      p_gpiox: Register structure for the GPIO port.
//          GPIOx (A-H)
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rcc_gpio_togglable_clock(gpio_reg_def const *const p_gpiox, togglable_e const toggle)
{

    switch (toggle)
    {
        case ENABLE:
            RCC->AHB1ENR |= (p_gpiox == GPIOA) ? RCC_AHB1ENR_GPIOAEN
                          : (p_gpiox == GPIOB) ? RCC_AHB1ENR_GPIOBEN
                          : (p_gpiox == GPIOC) ? RCC_AHB1ENR_GPIOCEN
                          : (p_gpiox == GPIOD) ? RCC_AHB1ENR_GPIODEN
                          : (p_gpiox == GPIOE) ? RCC_AHB1ENR_GPIOEEN
                          : (p_gpiox == GPIOF) ? RCC_AHB1ENR_GPIOFEN
                          : (p_gpiox == GPIOG) ? RCC_AHB1ENR_GPIOGEN
                          : (p_gpiox == GPIOH) ? RCC_AHB1ENR_GPIOHEN
                                               : 0;
            break;
        case DISABLE:
            RCC->AHB1RSTR |= (p_gpiox == GPIOA) ? RCC_AHB1RSTR_GPIOARST
                           : (p_gpiox == GPIOB) ? RCC_AHB1RSTR_GPIOBRST
                           : (p_gpiox == GPIOC) ? RCC_AHB1RSTR_GPIOCRST
                           : (p_gpiox == GPIOD) ? RCC_AHB1RSTR_GPIODRST
                           : (p_gpiox == GPIOE) ? RCC_AHB1RSTR_GPIOERST
                           : (p_gpiox == GPIOF) ? RCC_AHB1RSTR_GPIOFRST
                           : (p_gpiox == GPIOG) ? RCC_AHB1RSTR_GPIOGRST
                           : (p_gpiox == GPIOH) ? RCC_AHB1RSTR_GPIOHRST
                                                : 0;
            break;
    }
}

//============================================================================//{
//  Function: rcc_i2c_togglable_clock
//  Overview: Enable or disable clock to the specified I2C peripheral.
//  Parameters:
//      p_i2cx: Register structure for the I2C port.
//          I2Cx (1-3)
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rcc_i2c_togglable_clock(i2c_reg_def const *const p_i2cx, togglable_e const toggle)
{

    switch (toggle)
    {
        case ENABLE:
            RCC->APB1ENR |= (p_i2cx == I2C1) ? RCC_APB1ENR_I2C1EN
                          : (p_i2cx == I2C2) ? RCC_APB1ENR_I2C2EN
                          : (p_i2cx == I2C3) ? RCC_APB1ENR_I2C3EN
                                             : 0;
            break;
        case DISABLE:
            RCC->APB1RSTR |= (p_i2cx == I2C1) ? RCC_APB1RSTR_I2C1RST
                           : (p_i2cx == I2C2) ? RCC_APB1RSTR_I2C2RST
                           : (p_i2cx == I2C3) ? RCC_APB1RSTR_I2C3RST
                                              : 0;
            break;
    }
}

//============================================================================//{
//  Function: rcc_pwr_togglable_clock
//  Overview: Enable or disable clock to the PWR peripheral.
//  Parameters:
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rcc_pwr_togglable_clock(togglable_e const toggle)
{
    switch (toggle)
    {
        case ENABLE:
            RCC->APB1ENR |= RCC_APB1ENR_PWREN;
            break;
        case DISABLE:
            RCC->APB1ENR &= ~RCC_APB1ENR_PWREN;
            break;
    }
}

//============================================================================//{
//  Function: rcc_rtc_togglable_clock
//  Overview: Enable or disable clock to the RTC peripheral.
//  Parameters:
//      None
//  Return:
//      None
//  Note: Backup domain write protection must be disabled before this is allowed.
//============================================================================//}
void rcc_rtc_togglable_clock(togglable_e const toggle)
{
    switch (toggle)
    {
        case ENABLE:
            RCC->BDCR |= RCC_BDCR_RTCEN;
            break;
        case DISABLE:
            RCC->BDCR &= ~RCC_BDCR_RTCEN;
            break;
    }
}

//============================================================================//{
//  Function: rcc_syscfg_togglable_clock
//  Overview: Enable or disable clock to the SYSCFG peripheral.
//  Parameters:
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rcc_syscfg_togglable_clock(togglable_e const toggle)
{
    switch (toggle)
    {
        case ENABLE:
            RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
            break;
        case DISABLE:
            RCC->APB2ENR &= ~RCC_APB2ENR_SYSCFGEN;
            break;
    }
}

//============================================================================//{
//  Function: rcc_tim_togglable_clock
//  Overview: Enable or disable clock to the specified TIM peripheral.
//  Parameters:
//      p_timx: Timer register structure macro.
//          TIMx (1-14)
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rcc_tim_togglable_clock(tim_reg_def const *const p_timx, togglable_e const toggle)
{

    bus_types const tim_bus = tim_get_bus(p_timx);

    if ((ENABLE == toggle) && (APB1_BUS == tim_bus))
    {
        RCC->APB1ENR |= (p_timx == TIM2)  ? RCC_APB1ENR_TIM2EN
                      : (p_timx == TIM3)  ? RCC_APB1ENR_TIM3EN
                      : (p_timx == TIM4)  ? RCC_APB1ENR_TIM4EN
                      : (p_timx == TIM5)  ? RCC_APB1ENR_TIM5EN
                      : (p_timx == TIM6)  ? RCC_APB1ENR_TIM6EN
                      : (p_timx == TIM7)  ? RCC_APB1ENR_TIM7EN
                      : (p_timx == TIM12) ? RCC_APB1ENR_TIM12EN
                      : (p_timx == TIM13) ? RCC_APB1ENR_TIM13EN
                      : (p_timx == TIM14) ? RCC_APB1ENR_TIM14EN
                                          : 0;
    }
    else if ((ENABLE == toggle) && (APB2_BUS == tim_bus))
    {
        RCC->APB2ENR |= (p_timx == TIM1)  ? RCC_APB2ENR_TIM1EN
                      : (p_timx == TIM8)  ? RCC_APB2ENR_TIM8EN
                      : (p_timx == TIM9)  ? RCC_APB2ENR_TIM9EN
                      : (p_timx == TIM10) ? RCC_APB2ENR_TIM10EN
                      : (p_timx == TIM11) ? RCC_APB2ENR_TIM11EN
                                          : 0;
    }
    else if ((DISABLE == toggle) && (APB1_BUS == tim_bus))
    {
        RCC->APB1RSTR |= (p_timx == TIM2)  ? RCC_APB1RSTR_TIM2RST
                       : (p_timx == TIM3)  ? RCC_APB1RSTR_TIM3RST
                       : (p_timx == TIM4)  ? RCC_APB1RSTR_TIM4RST
                       : (p_timx == TIM5)  ? RCC_APB1RSTR_TIM5RST
                       : (p_timx == TIM6)  ? RCC_APB1RSTR_TIM6RST
                       : (p_timx == TIM7)  ? RCC_APB1RSTR_TIM7RST
                       : (p_timx == TIM12) ? RCC_APB1RSTR_TIM12RST
                       : (p_timx == TIM13) ? RCC_APB1RSTR_TIM13RST
                       : (p_timx == TIM14) ? RCC_APB1RSTR_TIM14RST
                                           : 0;
    }

    else if ((DISABLE == toggle) && (APB2_BUS == tim_bus))
    {
        RCC->APB2ENR |= (p_timx == TIM1)  ? RCC_APB2ENR_TIM1EN
                      : (p_timx == TIM8)  ? RCC_APB2ENR_TIM8EN
                      : (p_timx == TIM9)  ? RCC_APB2ENR_TIM9EN
                      : (p_timx == TIM10) ? RCC_APB2ENR_TIM10EN
                      : (p_timx == TIM11) ? RCC_APB2ENR_TIM11EN
                                          : 0;
    }
    else
    {
        ASSERT(false);
    }
}

//============================================================================//{
//  Function: rcc_usart_togglable_clock
//  Overview: Enable or disable clock to the specified USART peripheral.
//  Parameters:
//      p_usartx: USART register structure macro.
//          USARTx (1, 2, 3, 6) or
//          UARTx (4, 5)
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rcc_usart_togglable_clock(usart_reg_def const *const p_usartx, togglable_e const toggle)
{

    bus_types const usart_bus = get_usart_bus(p_usartx);

    if ((ENABLE == toggle) && (APB1_BUS == usart_bus))
    {

        RCC->APB1ENR |= (p_usartx == USART2) ? RCC_APB1ENR_USART2EN
                      : (p_usartx == USART3) ? RCC_APB1ENR_USART3EN
                      : (p_usartx == UART4)  ? RCC_APB1ENR_UART4EN
                      : (p_usartx == UART5)  ? RCC_APB1ENR_UART5EN
                                             : 0;
    }
    else if ((ENABLE == toggle) && (APB2_BUS == usart_bus))
    {
        RCC->APB2ENR |= (p_usartx == USART1) ? RCC_APB2ENR_USART1EN : (p_usartx == USART6) ? RCC_APB2ENR_USART6EN : 0;
    }
    else if ((DISABLE == toggle) && (APB1_BUS == usart_bus))
    {
        RCC->APB1RSTR |= (p_usartx == USART2) ? RCC_APB1ENR_USART2EN
                       : (p_usartx == USART3) ? RCC_APB1ENR_USART3EN
                       : (p_usartx == UART4)  ? RCC_APB1ENR_UART4EN
                       : (p_usartx == UART5)  ? RCC_APB1ENR_UART5EN
                                              : 0;
    }
    else if ((DISABLE == toggle) && (APB2_BUS == usart_bus))
    {
        RCC->APB2RSTR |= (p_usartx == USART1) ? RCC_APB2ENR_USART1EN : (p_usartx == USART6) ? RCC_APB2ENR_USART6EN : 0;
    }
    else
    {
        ASSERT(false);
    }
}

//============================================================================//{
//  Function: rcc_get_pll_freq_hz
//  Overview: Get the PLL clock frequency.
//  Parameters:
//      None
//  Return:
//      PLL clock frequency (hz)
//  Note: None
//============================================================================//}
uint32_t rcc_get_pll_freq_hz(void)
{
    uint32_t rcc_pllcfgr_reg;

    uint8_t pllm;
    uint8_t plln;

    uint8_t pllp_reg;
    uint8_t const pllp_values[] = {2, 4, 6, 8};
    uint8_t pllp;

    clock_sources_e pll_src;

    uint32_t vco_clk_freq;
    uint32_t pll_input_clk_freq = 0;
    uint32_t pll_output_clk_freq;

    // Get register values
    rcc_pllcfgr_reg = RCC->PLLCFGR;
    pllm            = ((rcc_pllcfgr_reg >> RCC_PLLCFGR_PLLM0_POS) & RCC_PLLCFGR_PLLM0_MASK);
    plln            = ((rcc_pllcfgr_reg >> RCC_PLLCFGR_PLLN0_POS) & RCC_PLLCFGR_PLLN0_MASK);
    pllp_reg        = ((rcc_pllcfgr_reg >> RCC_PLLCFGR_PLLP0_POS) & RCC_PLLCFGR_PLLP0_MASK);
    pllp            = pllp_values[pllp_reg];

    pll_src = ((rcc_pllcfgr_reg >> RCC_PLLCFGR_PLLSRC_POS) & RCC_PLLCFGR_PLLSRC_MASK);

    // Select PLL clock source frequency.
    switch (pll_src)
    {
        case CLOCK_SRC_HSI:
            pll_input_clk_freq = HSI_CLOCK_FREQ;
            break;
        case CLOCK_SRC_HSE:
            pll_input_clk_freq = HSE_CLOCK_FREQ;
            break;
        default:
            ASSERT(false);
            break;
    }

    // Calculations of PLL output frequency.
    vco_clk_freq        = (pll_input_clk_freq) * (plln / pllm);
    pll_output_clk_freq = vco_clk_freq / pllp;

    return pll_output_clk_freq;
}

//============================================================================//{
//  Function: rcc_get_sys_clock_freq_hz
//  Overview: Get the system clock frequency.
//  Parameters:
//      None
//  Return:
//      System clock frequency (hz)
//  Note: None
//============================================================================//}
uint32_t rcc_get_sys_clock_freq_hz(void)
{
    // Get active clock source (HSI/HSE/PLL).
    clock_sources_e clock_source = (clock_sources_e) ((RCC->CFGR >> RCC_CFGR_SWS0_POS) & RCC_CFGR_SWS0_MASK);
    ASSERT((clock_source == CLOCK_SRC_HSI) || (clock_source == CLOCK_SRC_HSE) || (clock_source == CLOCK_SRC_PLL));

    // Return clock depending on what the source is.
    switch (clock_source)
    {
        case CLOCK_SRC_HSI:
            return HSI_CLOCK_FREQ;
        case CLOCK_SRC_HSE:
            return HSE_CLOCK_FREQ;
        case CLOCK_SRC_PLL:
            return rcc_get_pll_freq_hz();
        default:
            ASSERT(false);
            return 0;
    }
}

//============================================================================//{
//  Function: rcc_get_bus_clock_freq_hz
//  Overview: Get the specified bus clock frequency.
//  Parameters:
//      bus: Different bus options to be returned.
//          AHB1_BUS
//          AHB2_BUS
//          APB1_BUS
//          APB2_BUS
//  Return:
//      Clock frequency (hz) for the given bus.
//  Note: None
//============================================================================//}
uint32_t rcc_get_bus_clock_freq_hz(bus_types const bus)
{
    uint16_t const ahb_clock_prescaler[] = {2, 4, 8, 16, 64, 128, 256, 512};
    uint16_t const apb_clock_prescaler[] = {2, 4, 8, 16};

    uint32_t sysclock_freq;

    // Highspeed = AHB
    // Lowspeed = APB
    uint8_t ahb_prescaler;
    uint32_t ahb_clock;

    uint8_t apb1_ppre1_reg;
    uint8_t apb1_prescaler;
    uint32_t apb1_clock;

    uint8_t apb2_ppre2_reg;
    uint8_t apb2_prescaler;
    uint32_t apb2_clock;

    // Calculate different clocks (AHB1/AHB2/APB1/APB2) from prescalers.

    // Get active system clock source (HSI/HSE/PLL) - Use function to get the clock for that source.
    sysclock_freq = rcc_get_sys_clock_freq_hz();

    // Calculate AHB clock
    ahb_prescaler = ((RCC->CFGR >> RCC_CFGR_HPRE_POS) & RCC_CFGR_HPRE_MASK);
    // If 4th bit is 1, select prescaler, otherwise the prescaler is 1.
    ahb_prescaler = ((ahb_prescaler & 0b1000) != 0) ? ahb_clock_prescaler[ahb_prescaler & ~(0b1000)] : 1;
    ahb_clock     = sysclock_freq / ahb_prescaler;

    switch (bus)
    {
        case AHB1_BUS:
        case AHB2_BUS:
            return ahb_clock;
        case APB1_BUS:
            apb1_ppre1_reg = ((RCC->CFGR >> RCC_CFGR_PPRE1_POS) & RCC_CFGR_PPRE1_MASK);
            apb1_prescaler = ((apb1_ppre1_reg & 0b100) != 0) ? apb_clock_prescaler[apb1_ppre1_reg & ~(0b100)] : 1;
            apb1_clock     = ahb_clock / apb1_prescaler;
            return apb1_clock;
        case APB2_BUS:
            apb2_ppre2_reg = ((RCC->CFGR >> RCC_CFGR_PPRE2_POS) & RCC_CFGR_PPRE2_MASK);
            apb2_prescaler = ((apb2_ppre2_reg & 0b100) != 0) ? apb_clock_prescaler[apb2_ppre2_reg & ~(0b100)] : 1;
            apb2_clock     = ahb_clock / apb2_prescaler;
            return apb2_clock;

        default:
            ASSERT(false);
            return 0;
    }
}

//============================================================================//{
//  Function: rcc_get_timer_clock_freq_hz
//  Overview: Get the timer clock frequency from the specified bus.
//  Parameters:
//      bus: Different bus options to be returned.
//          APB1_BUS
//          APB2_BUS
//  Return:
//      Clock frequency (hz) for the timer, given the corresponding bus its on.
//  Note: Use TIMER API function to get bus for timer.
//============================================================================//}
uint32_t rcc_get_timer_clock_freq_hz(bus_types const bus)
{
    uint32_t bus_clock          = rcc_get_bus_clock_freq_hz(bus);
    uint8_t bus_prescaler       = 0;
    uint8_t const APB_PRESCALER = 1 << 2;

    switch (bus)
    {
        case APB1_BUS:
            bus_prescaler = (RCC->CFGR >> RCC_CFGR_PPRE1_POS) & RCC_CFGR_PPRE1_MASK;
            break;
        case APB2_BUS:
            bus_prescaler = (RCC->CFGR >> RCC_CFGR_PPRE2_POS) & RCC_CFGR_PPRE2_MASK;
            break;
        default:
            ASSERT(false);
    }

    if (bus_prescaler & APB_PRESCALER)
    {
        return 2 * bus_clock;
    }

    return bus_clock;
}

//============================================================================//{
//  Function: rcc_sel_rtc_clk_src
//  Overview: Set the RTC clock source.
//  Parameters:
//      rtc_clk_sel: Clock source for RTC
//          RCC_BDCR_RTCSEL_NA
//          RCC_BDCR_RTCSEL_LSE
//          RCC_BDCR_RTCSEL_LSI
//          RCC_BDCR_RTCSEL_HSE
//  Return:
//      None
//  Note: None
//============================================================================//}
void rcc_set_rtc_clk_src(rcc_rtc_clk_src_e rtc_clk_sel)
{
    ASSERT((rtc_clk_sel == RCC_RTC_CLK_SRC_NA) || (rtc_clk_sel == RCC_RTC_CLK_SRC_LSE)
           || (rtc_clk_sel == RCC_RTC_CLK_SRC_LSI) || (rtc_clk_sel == RCC_RTC_CLK_SRC_HSE));

    RCC->BDCR &= ~(RCC_BDCR_RTCSEL_MASK << RCC_BDCR_RTCSEL0_POS);
    RCC->BDCR |= rtc_clk_sel << RCC_BDCR_RTCSEL0_POS;
}

//============================================================================//{
//  Function: rcc_set_rtc_hse_clk_pre
//  Overview: Set the HSE clock prescaler for the RTC peripheral.
//  Parameters:
//      hse_pre: Prescaler to divide HSE by.
//          RCC_CFGR_RTCPRE_NA
//          RCC_CFGR_RTCPRE_x (2 - 31)
//  Return:
//      None
//  Note: None
//============================================================================//}
void rcc_set_rtc_pre(rcc_rtc_hse_pre_e hse_pre)
{
    // NOTE: Fragile assert but much cleaner than checking against each enum value
    ASSERT((hse_pre >= RCC_RTC_HSE_PRE_NA) && (hse_pre <= RCC_RTC_HSE_PRE_31));

    RCC->CFGR &= ~(RCC_CFGR_RTCPRE_MASK << RCC_CFGR_RTCPRE_POS);
    RCC->CFGR |= hse_pre << RCC_CFGR_RTCPRE_POS;
}

//============================================================================//{
//  Function: rcc_get_rtc_clock_freq_hz
//  Overview: Get RTC clock frequency.
//  Parameters:
//      None
//  Return:
//      Clock frequency (hz) for the RTC peripheral.
//  Note: None
//============================================================================//}
uint32_t rcc_get_rtc_clock_freq_hz(void)
{
    uint32_t rtc_clk_freq_hz      = 0;
    uint8_t const RTC_CLK_SRC     = (rcc_rtc_clk_src_e) ((RCC->BDCR >> RCC_BDCR_RTCSEL0_POS) & RCC_BDCR_RTCSEL_MASK);
    uint8_t const RTC_HSE_CLK_PRE = (rcc_rtc_hse_pre_e) ((RCC->CFGR >> RCC_CFGR_RTCPRE_POS) & RCC_CFGR_RTCPRE_MASK);
    uint16_t const rtc_hse_pre_opts[] = {0,  0,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                                         16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

    switch (RTC_CLK_SRC)
    {
        case RCC_RTC_CLK_SRC_NA:
            ASSERT(false);
            break;
        case RCC_RTC_CLK_SRC_LSE:
            rtc_clk_freq_hz = LSE_CLOCK_FREQ;
            break;
        case RCC_RTC_CLK_SRC_LSI:
            rtc_clk_freq_hz = LSI_CLOCK_FREQ;
            break;
        case RCC_RTC_CLK_SRC_HSE:
            ASSERT(RTC_HSE_CLK_PRE != RCC_RTC_HSE_PRE_NA);
            rtc_clk_freq_hz = HSE_CLOCK_FREQ / rtc_hse_pre_opts[RTC_HSE_CLK_PRE];
            break;
    }

    return rtc_clk_freq_hz;
}

//============================================================================//{
//  Function: rcc_config_mco
//  Overview: Configure specified MCO pin/clock with specified prescaler.
//  Parameters:
//      var name: Which MCO pin and clock source to pull from.
//          RCC_MCO1_HSI_SRC
//          RCC_MCO1_LSE_SRC
//          RCC_MCO1_HSE_SRC
//          RCC_MCO1_PLL_SRC
//          RCC_MCO2_SYSCLK_SRC
//          RCC_MCO2_PLLI2S_SRC
//          RCC_MCO2_HSE_SRC
//          RCC_MCO2_PLL_SRC
//  Return:
//      None
//  Note: None
//============================================================================//}
void rcc_config_mco(rcc_mco_clock_src_e const mco_clk_src, rcc_mco_prescaler_e const mco_prescaler)
{

    switch (mco_clk_src)
    {

        case RCC_MCO1_HSI_SRC:
        case RCC_MCO1_LSE_SRC:
        case RCC_MCO1_HSE_SRC:
        case RCC_MCO1_PLL_SRC:
            RCC->CFGR |= (mco_clk_src << RCC_CFGR_MCO1_POS);
            if (mco_prescaler == RCC_MCO_PRE_1)
            {
                RCC->CFGR &= ~(RCC_CFGR_MCO1PRE_MASK << RCC_CFGR_MCO1PRE_POS);
            }
            else
            {
                RCC->CFGR |= (mco_prescaler + 2) << RCC_CFGR_MCO1PRE_POS;
            }
            init_mco_gpio_pin(RCC_MCO1_SEL);
            break;
        case RCC_MCO2_SYSCLK_SRC:
        case RCC_MCO2_PLLI2S_SRC:
        case RCC_MCO2_HSE_SRC:
        case RCC_MCO2_PLL_SRC:
            RCC->CFGR |= ((mco_clk_src - 4) << RCC_CFGR_MCO2_POS);
            if (mco_prescaler == RCC_MCO_PRE_1)
            {
                RCC->CFGR &= ~(RCC_CFGR_MCO2PRE_MASK << RCC_CFGR_MCO2PRE_POS);
            }
            else
            {
                RCC->CFGR |= (mco_prescaler + 2) << RCC_CFGR_MCO2PRE_POS;
            }
            init_mco_gpio_pin(RCC_MCO2_SEL);
            break;
        default:
            ASSERT(false);
    }
}

//======================================================================================//}
//                  Private Function Implementation
//======================================================================================//{

static void init_mco_gpio_pin(rcc_mco_sel_e const mco_select)
{
    gpio_handle mco_gpio_pin;
    mco_gpio_pin.gpio_conf.mode            = GPIO_MODE_ALT_FN;
    mco_gpio_pin.gpio_conf.output_type     = GPIO_OPTYPE_PUSH_PULL;
    mco_gpio_pin.gpio_conf.pullup_pulldown = GPIO_PUPD_NO;
    mco_gpio_pin.gpio_conf.alt_fn_num      = GPIO_ALT_FN_0;

    mco_gpio_pin.gpio_conf.it_trigger   = GPIO_IT_NA;
    mco_gpio_pin.gpio_conf.output_speed = GPIO_OSPEED_MEDIUM;

    switch (mco_select)
    {
        case RCC_MCO1_SEL:
            // MCO1 - PA8
            mco_gpio_pin.p_gpiox           = GPIOA;
            mco_gpio_pin.gpio_conf.pin_num = PIN_NUM_8;
            break;
        case RCC_MCO2_SEL:
            // MCO2 - PC9
            mco_gpio_pin.p_gpiox           = GPIOC;
            mco_gpio_pin.gpio_conf.pin_num = PIN_NUM_9;
            break;
        default:
            ASSERT(false);
    }
    gpio_init(&mco_gpio_pin);
}

// *End of file*
