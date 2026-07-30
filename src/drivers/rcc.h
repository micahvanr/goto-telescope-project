#ifndef RCC_H
#define RCC_H

#include "common.h"

//======================================================================================//}
//                  Address Definitions
//======================================================================================//{

typedef enum {
    RCC_BASE_ADDR = 0x40023800ul,
} rcc_base_addr_e;

//======================================================================================//}
//                  Peripheral Constants
//======================================================================================//{

// TODO: Change frequencies for custom board when applicable
typedef enum {
    HSI_CLOCK_FREQ = 16000000,
    HSE_CLOCK_FREQ = 8000000, // Only for discovery board -> will need to be changed for custom board
    LSE_CLOCK_FREQ = 0,       // N/A for discover board
    LSI_CLOCK_FREQ = 32000,
} clock_values_e;

typedef enum {
    CLOCK_SRC_HSI = 0,
    CLOCK_SRC_HSE = 1,
    CLOCK_SRC_PLL = 2,
    CLOCK_SRC_LSE = 3,
    CLOCK_SRC_LSI = 4,
} clock_sources_e;

typedef enum {
    AHB1_BUS = 0,
    AHB2_BUS = 1,
    APB1_BUS = 3,
    APB2_BUS = 4,
} bus_types;

typedef enum {
    RCC_MCO1_SEL,
    RCC_MCO2_SEL,
} rcc_mco_sel_e;

typedef enum {
    RCC_MCO_PRE_1 = 1,
    RCC_MCO_PRE_2 = 2,
    RCC_MCO_PRE_3 = 3,
    RCC_MCO_PRE_4 = 4,
    RCC_MCO_PRE_5 = 5,
} rcc_mco_prescaler_e;

typedef enum {
    RCC_MCO1_HSI_SRC    = 0,
    RCC_MCO1_LSE_SRC    = 1,
    RCC_MCO1_HSE_SRC    = 2,
    RCC_MCO1_PLL_SRC    = 3,
    RCC_MCO2_SYSCLK_SRC = 4,
    RCC_MCO2_PLLI2S_SRC = 5,
    RCC_MCO2_HSE_SRC    = 6,
    RCC_MCO2_PLL_SRC    = 7,
} rcc_mco_clock_src_e;

//======================================================================================//}
//                  Register Constants
//======================================================================================//{

typedef enum {
    RCC_CFGR_SWS_MASK     = 0b11,        // 2 bits
    RCC_PLLCFGR_PLLM_MASK = 0b111111,    // 6 bits
    RCC_PLLCFGR_PLLN_MASK = 0b111111111, // 9 bits
    RCC_PLLCFGR_PLLP_MASK = 0b11,        // 2 bits
} rcc_reg_misc_e;

typedef enum {
    RCC_CR_PLLI2SRDY_POS = 27, // PLLI2S clock ready flag
    RCC_CR_PLLI2SON_POS  = 26, // PLLI2S enable
    RCC_CR_PLLRDY_POS    = 25, // Main PLL (PLL) clock ready flag
    RCC_CR_PLLON_POS     = 24, // Main PLL (PLL) enable
    RCC_CR_CSSON_POS     = 19, // Clock security system enable
    RCC_CR_HSEBYP_POS    = 18, // HSE clock bypass
    RCC_CR_HSERDY_POS    = 17, // HSE clock ready flag
    RCC_CR_HSEON_POS     = 16, // HSE clock enable
    RCC_CR_HSICAL_POS    = 8,  // Internal high-speed clock calibration
    RCC_CR_HSITRIM_POS   = 3,  // Internal high-speed clock trimming
    RCC_CR_HSIRDY_POS    = 1,  // Internal high-speed clock ready flag
    RCC_CR_HSION_POS     = 0,  // Internal high-speed clock enable
} rcc_cr_pos_e;

typedef enum {
    RCC_CR_PLLI2SRDY = (1 << RCC_CR_PLLI2SRDY_POS), // PLLI2S clock ready flag
    RCC_CR_PLLI2SON  = (1 << RCC_CR_PLLI2SON_POS),  // PLLI2S enable
    RCC_CR_PLLRDY    = (1 << RCC_CR_PLLRDY_POS),    // Main PLL (PLL) clock ready flag
    RCC_CR_PLLON     = (1 << RCC_CR_PLLON_POS),     // Main PLL (PLL) enable
    RCC_CR_CSSON     = (1 << RCC_CR_CSSON_POS),     // Clock security system enable
    RCC_CR_HSEBYP    = (1 << RCC_CR_HSEBYP_POS),    // HSE clock bypass
    RCC_CR_HSERDY    = (1 << RCC_CR_HSERDY_POS),    // HSE clock ready flag
    RCC_CR_HSEON     = (1 << RCC_CR_HSEON_POS),     // HSE clock enable
    RCC_CR_HSICAL    = (1 << RCC_CR_HSICAL_POS),    // Internal high-speed clock calibration
    RCC_CR_HSITRIM   = (1 << RCC_CR_HSITRIM_POS),   // Internal high-speed clock trimming
    RCC_CR_HSIRDY    = (1 << RCC_CR_HSIRDY_POS),    // Internal high-speed clock ready flag
    RCC_CR_HSION     = (1 << RCC_CR_HSION_POS),     // Internal high-speed clock enable
} rcc_cr_e;

typedef enum {
    RCC_CR_PLLI2SRDY_MASK = 0b1,        // 1 bit(s)
    RCC_CR_PLLI2SON_MASK  = 0b1,        // 1 bit(s)
    RCC_CR_PLLRDY_MASK    = 0b1,        // 1 bit(s)
    RCC_CR_PLLON_MASK     = 0b1,        // 1 bit(s)
    RCC_CR_CSSON_MASK     = 0b1,        // 1 bit(s)
    RCC_CR_HSEBYP_MASK    = 0b1,        // 1 bit(s)
    RCC_CR_HSERDY_MASK    = 0b1,        // 1 bit(s)
    RCC_CR_HSEON_MASK     = 0b1,        // 1 bit(s)
    RCC_CR_HSICAL_MASK    = 0b11111111, // 8 bit(s)
    RCC_CR_HSITRIM_MASK   = 0b11111,    // 5 bit(s)
    RCC_CR_HSIRDY_MASK    = 0b1,        // 1 bit(s)
    RCC_CR_HSION_MASK     = 0b1,        // 1 bit(s)
} rcc_cr_mask_e;

// clang-format off
typedef enum {
    RCC_PLLCFGR_PLLQ3_POS  = 27,    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ2_POS  = 26,    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ1_POS  = 25,    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ0_POS  = 24,    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLSRC_POS = 22,    // Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
    RCC_PLLCFGR_PLLP1_POS  = 17,    // Main PLL (PLL) division factor for main system clock
    RCC_PLLCFGR_PLLP0_POS  = 16,    // Main PLL (PLL) division factor for main system clock
    RCC_PLLCFGR_PLLN8_POS  = 14,    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN7_POS  = 13,    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN6_POS  = 12,    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN5_POS  = 11,    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN4_POS  = 10,    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN3_POS  = 9,     // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN2_POS  = 8,     // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN1_POS  = 7,     // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN0_POS  = 6,     // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLM5_POS  = 5,     // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM4_POS  = 4,     // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM3_POS  = 3,     // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM2_POS  = 2,     // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM1_POS  = 1,     // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM0_POS  = 0,     // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
} rcc_pllcfgr_pos_e;

typedef enum {
    RCC_PLLCFGR_PLLQ3  = (1 << RCC_PLLCFGR_PLLQ3_POS),    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ2  = (1 << RCC_PLLCFGR_PLLQ2_POS),    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ1  = (1 << RCC_PLLCFGR_PLLQ1_POS),    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLQ0  = (1 << RCC_PLLCFGR_PLLQ0_POS),    // Main PLL (PLL) division factor for USB OTG FS, SDIO and random number generator clocks
    RCC_PLLCFGR_PLLSRC = (1 << RCC_PLLCFGR_PLLSRC_POS),   // Main PLL(PLL) and audio PLL (PLLI2S) entry clock source
    RCC_PLLCFGR_PLLP1  = (1 << RCC_PLLCFGR_PLLP1_POS),    // Main PLL (PLL) division factor for main system clock
    RCC_PLLCFGR_PLLP0  = (1 << RCC_PLLCFGR_PLLP0_POS),    // Main PLL (PLL) division factor for main system clock
    RCC_PLLCFGR_PLLN8  = (1 << RCC_PLLCFGR_PLLN8_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN7  = (1 << RCC_PLLCFGR_PLLN7_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN6  = (1 << RCC_PLLCFGR_PLLN6_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN5  = (1 << RCC_PLLCFGR_PLLN5_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN4  = (1 << RCC_PLLCFGR_PLLN4_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN3  = (1 << RCC_PLLCFGR_PLLN3_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN2  = (1 << RCC_PLLCFGR_PLLN2_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN1  = (1 << RCC_PLLCFGR_PLLN1_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLN0  = (1 << RCC_PLLCFGR_PLLN0_POS),    // Main PLL (PLL) multiplication factor for VCO
    RCC_PLLCFGR_PLLM5  = (1 << RCC_PLLCFGR_PLLM5_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM4  = (1 << RCC_PLLCFGR_PLLM4_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM3  = (1 << RCC_PLLCFGR_PLLM3_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM2  = (1 << RCC_PLLCFGR_PLLM2_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM1  = (1 << RCC_PLLCFGR_PLLM1_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
    RCC_PLLCFGR_PLLM0  = (1 << RCC_PLLCFGR_PLLM0_POS),    // Division factor for the main PLL (PLL) and audio PLL (PLLI2S) input clock
} rcc_pllcfgr_e;
// clang-format on 

typedef enum {
    RCC_PLLCFGR_PLLQ3_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLQ2_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLQ1_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLQ0_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLSRC_MASK = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLP1_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLP0_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLN8_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLN7_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLN6_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLN5_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLN4_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLN3_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLN2_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLN1_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLN0_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLM5_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLM4_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLM3_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLM2_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLM1_MASK  = 0b1,    // 1 bit(s)
    RCC_PLLCFGR_PLLM0_MASK  = 0b1,    // 1 bit(s)
} rcc_pllcfgr_mask_e;

typedef enum {
    RCC_CFGR_MCO2_POS    = 30,    // Microcontroller clock output 2
    RCC_CFGR_MCO2PRE_POS = 27,    // MCO2 prescaler
    RCC_CFGR_MCO1PRE_POS = 24,    // MCO1 prescaler
    RCC_CFGR_I2SSRC_POS  = 23,    // I2S clock selection
    RCC_CFGR_MCO1_POS    = 21,    // Microcontroller clock output 1
    RCC_CFGR_RTCPRE_POS  = 16,    // HSE division factor for RTC clock
    RCC_CFGR_PPRE2_POS   = 13,    // APB high-speed prescaler (APB2)
    RCC_CFGR_PPRE1_POS   = 10,    // APB Low speed prescaler (APB1)
    RCC_CFGR_HPRE_POS    = 4,     // AHB prescaler
    RCC_CFGR_SWS1_POS    = 3,     // System clock switch status
    RCC_CFGR_SWS0_POS    = 2,     // System clock switch status
    RCC_CFGR_SW1_POS     = 1,     // System clock switch
    RCC_CFGR_SW0_POS     = 0,     // System clock switch
} rcc_cfgr_pos_e;

typedef enum {
    RCC_CFGR_MCO2    = (1 << RCC_CFGR_MCO2_POS),      // Microcontroller clock output 2
    RCC_CFGR_MCO2PRE = (1 << RCC_CFGR_MCO2PRE_POS),   // MCO2 prescaler
    RCC_CFGR_MCO1PRE = (1 << RCC_CFGR_MCO1PRE_POS),   // MCO1 prescaler
    RCC_CFGR_I2SSRC  = (1 << RCC_CFGR_I2SSRC_POS),    // I2S clock selection
    RCC_CFGR_MCO1    = (1 << RCC_CFGR_MCO1_POS),      // Microcontroller clock output 1
    RCC_CFGR_RTCPRE  = (1 << RCC_CFGR_RTCPRE_POS),    // HSE division factor for RTC clock
    RCC_CFGR_PPRE2   = (1 << RCC_CFGR_PPRE2_POS),     // APB high-speed prescaler (APB2)
    RCC_CFGR_PPRE1   = (1 << RCC_CFGR_PPRE1_POS),     // APB Low speed prescaler (APB1)
    RCC_CFGR_HPRE    = (1 << RCC_CFGR_HPRE_POS),      // AHB prescaler
    RCC_CFGR_SWS1    = (1 << RCC_CFGR_SWS1_POS),      // System clock switch status
    RCC_CFGR_SWS0    = (1 << RCC_CFGR_SWS0_POS),      // System clock switch status
    RCC_CFGR_SW1     = (1 << RCC_CFGR_SW1_POS),       // System clock switch
    RCC_CFGR_SW0     = (1 << RCC_CFGR_SW0_POS),       // System clock switch
} rcc_cfgr_e;

typedef enum {
    RCC_CFGR_MCO2_MASK    = 0b11,       // 2 bit(s)
    RCC_CFGR_MCO2PRE_MASK = 0b111,      // 3 bit(s)
    RCC_CFGR_MCO1PRE_MASK = 0b111,      // 3 bit(s)
    RCC_CFGR_I2SSRC_MASK  = 0b1,        // 1 bit(s)
    RCC_CFGR_MCO1_MASK    = 0b11,       // 2 bit(s)
    RCC_CFGR_RTCPRE_MASK  = 0b11111,    // 5 bit(s)
    RCC_CFGR_PPRE2_MASK   = 0b111,      // 3 bit(s)
    RCC_CFGR_PPRE1_MASK   = 0b111,      // 3 bit(s)
    RCC_CFGR_HPRE_MASK    = 0b1111,     // 4 bit(s)
    RCC_CFGR_SWS1_MASK    = 0b1,        // 1 bit(s)
    RCC_CFGR_SWS0_MASK    = 0b1,        // 1 bit(s)
    RCC_CFGR_SW1_MASK     = 0b1,        // 1 bit(s)
    RCC_CFGR_SW0_MASK     = 0b1,        // 1 bit(s)
} rcc_cfgr_mask_e;

typedef enum {
    RCC_CIR_CSSC_POS        = 23,    // Clock security system interrupt clear
    RCC_CIR_PLLI2SRDYC_POS  = 21,    // PLLI2S ready interrupt clear
    RCC_CIR_PLLRDYC_POS     = 20,    // Main PLL(PLL) ready interrupt clear
    RCC_CIR_HSERDYC_POS     = 19,    // HSE ready interrupt clear
    RCC_CIR_HSIRDYC_POS     = 18,    // HSI ready interrupt clear
    RCC_CIR_LSERDYC_POS     = 17,    // LSE ready interrupt clear
    RCC_CIR_LSIRDYC_POS     = 16,    // LSI ready interrupt clear
    RCC_CIR_PLLI2SRDYIE_POS = 13,    // PLLI2S ready interrupt enable
    RCC_CIR_PLLRDYIE_POS    = 12,    // Main PLL (PLL) ready interrupt enable
    RCC_CIR_HSERDYIE_POS    = 11,    // HSE ready interrupt enable
    RCC_CIR_HSIRDYIE_POS    = 10,    // HSI ready interrupt enable
    RCC_CIR_LSERDYIE_POS    = 9,     // LSE ready interrupt enable
    RCC_CIR_LSIRDYIE_POS    = 8,     // LSI ready interrupt enable
    RCC_CIR_CSSF_POS        = 7,     // Clock security system interrupt flag
    RCC_CIR_PLLI2SRDYF_POS  = 5,     // PLLI2S ready interrupt flag
    RCC_CIR_PLLRDYF_POS     = 4,     // Main PLL (PLL) ready interrupt flag
    RCC_CIR_HSERDYF_POS     = 3,     // HSE ready interrupt flag
    RCC_CIR_HSIRDYF_POS     = 2,     // HSI ready interrupt flag
    RCC_CIR_LSERDYF_POS     = 1,     // LSE ready interrupt flag
    RCC_CIR_LSIRDYF_POS     = 0,     // LSI ready interrupt flag
} rcc_cir_pos_e;

typedef enum {
    RCC_CIR_CSSC        = (1 << RCC_CIR_CSSC_POS),          // Clock security system interrupt clear
    RCC_CIR_PLLI2SRDYC  = (1 << RCC_CIR_PLLI2SRDYC_POS),    // PLLI2S ready interrupt clear
    RCC_CIR_PLLRDYC     = (1 << RCC_CIR_PLLRDYC_POS),       // Main PLL(PLL) ready interrupt clear
    RCC_CIR_HSERDYC     = (1 << RCC_CIR_HSERDYC_POS),       // HSE ready interrupt clear
    RCC_CIR_HSIRDYC     = (1 << RCC_CIR_HSIRDYC_POS),       // HSI ready interrupt clear
    RCC_CIR_LSERDYC     = (1 << RCC_CIR_LSERDYC_POS),       // LSE ready interrupt clear
    RCC_CIR_LSIRDYC     = (1 << RCC_CIR_LSIRDYC_POS),       // LSI ready interrupt clear
    RCC_CIR_PLLI2SRDYIE = (1 << RCC_CIR_PLLI2SRDYIE_POS),   // PLLI2S ready interrupt enable
    RCC_CIR_PLLRDYIE    = (1 << RCC_CIR_PLLRDYIE_POS),      // Main PLL (PLL) ready interrupt enable
    RCC_CIR_HSERDYIE    = (1 << RCC_CIR_HSERDYIE_POS),      // HSE ready interrupt enable
    RCC_CIR_HSIRDYIE    = (1 << RCC_CIR_HSIRDYIE_POS),      // HSI ready interrupt enable
    RCC_CIR_LSERDYIE    = (1 << RCC_CIR_LSERDYIE_POS),      // LSE ready interrupt enable
    RCC_CIR_LSIRDYIE    = (1 << RCC_CIR_LSIRDYIE_POS),      // LSI ready interrupt enable
    RCC_CIR_CSSF        = (1 << RCC_CIR_CSSF_POS),          // Clock security system interrupt flag
    RCC_CIR_PLLI2SRDYF  = (1 << RCC_CIR_PLLI2SRDYF_POS),    // PLLI2S ready interrupt flag
    RCC_CIR_PLLRDYF     = (1 << RCC_CIR_PLLRDYF_POS),       // Main PLL (PLL) ready interrupt flag
    RCC_CIR_HSERDYF     = (1 << RCC_CIR_HSERDYF_POS),       // HSE ready interrupt flag
    RCC_CIR_HSIRDYF     = (1 << RCC_CIR_HSIRDYF_POS),       // HSI ready interrupt flag
    RCC_CIR_LSERDYF     = (1 << RCC_CIR_LSERDYF_POS),       // LSE ready interrupt flag
    RCC_CIR_LSIRDYF     = (1 << RCC_CIR_LSIRDYF_POS),       // LSI ready interrupt flag
} rcc_cir_e;

typedef enum {
    RCC_CIR_CSSC_MASK        = 0b1,    // 1 bit(s)
    RCC_CIR_PLLI2SRDYC_MASK  = 0b1,    // 1 bit(s)
    RCC_CIR_PLLRDYC_MASK     = 0b1,    // 1 bit(s)
    RCC_CIR_HSERDYC_MASK     = 0b1,    // 1 bit(s)
    RCC_CIR_HSIRDYC_MASK     = 0b1,    // 1 bit(s)
    RCC_CIR_LSERDYC_MASK     = 0b1,    // 1 bit(s)
    RCC_CIR_LSIRDYC_MASK     = 0b1,    // 1 bit(s)
    RCC_CIR_PLLI2SRDYIE_MASK = 0b1,    // 1 bit(s)
    RCC_CIR_PLLRDYIE_MASK    = 0b1,    // 1 bit(s)
    RCC_CIR_HSERDYIE_MASK    = 0b1,    // 1 bit(s)
    RCC_CIR_HSIRDYIE_MASK    = 0b1,    // 1 bit(s)
    RCC_CIR_LSERDYIE_MASK    = 0b1,    // 1 bit(s)
    RCC_CIR_LSIRDYIE_MASK    = 0b1,    // 1 bit(s)
    RCC_CIR_CSSF_MASK        = 0b1,    // 1 bit(s)
    RCC_CIR_PLLI2SRDYF_MASK  = 0b1,    // 1 bit(s)
    RCC_CIR_PLLRDYF_MASK     = 0b1,    // 1 bit(s)
    RCC_CIR_HSERDYF_MASK     = 0b1,    // 1 bit(s)
    RCC_CIR_HSIRDYF_MASK     = 0b1,    // 1 bit(s)
    RCC_CIR_LSERDYF_MASK     = 0b1,    // 1 bit(s)
    RCC_CIR_LSIRDYF_MASK     = 0b1,    // 1 bit(s)
} rcc_cir_mask_e;

typedef enum {
    RCC_AHB1RSTR_OTGHSRST_POS  = 29,    // USB OTG HS module reset
    RCC_AHB1RSTR_ETHMACRST_POS = 25,    // Ethernet MAC reset
    RCC_AHB1RSTR_DMA2RST_POS   = 22,    // DMA2 reset
    RCC_AHB1RSTR_DMA1RST_POS   = 21,    // DMA2 reset
    RCC_AHB1RSTR_CRCRST_POS    = 12,    // CRC reset
    RCC_AHB1RSTR_GPIOIRST_POS  = 8,     // IO port I reset
    RCC_AHB1RSTR_GPIOHRST_POS  = 7,     // IO port H reset
    RCC_AHB1RSTR_GPIOGRST_POS  = 6,     // IO port G reset
    RCC_AHB1RSTR_GPIOFRST_POS  = 5,     // IO port F reset
    RCC_AHB1RSTR_GPIOERST_POS  = 4,     // IO port E reset
    RCC_AHB1RSTR_GPIODRST_POS  = 3,     // IO port D reset
    RCC_AHB1RSTR_GPIOCRST_POS  = 2,     // IO port C reset
    RCC_AHB1RSTR_GPIOBRST_POS  = 1,     // IO port B reset
    RCC_AHB1RSTR_GPIOARST_POS  = 0,     // IO port A reset
} rcc_ahb1rstr_pos_e;

typedef enum {
    RCC_AHB1RSTR_OTGHSRST  = (1 << RCC_AHB1RSTR_OTGHSRST_POS),    // USB OTG HS module reset
    RCC_AHB1RSTR_ETHMACRST = (1 << RCC_AHB1RSTR_ETHMACRST_POS),   // Ethernet MAC reset
    RCC_AHB1RSTR_DMA2RST   = (1 << RCC_AHB1RSTR_DMA2RST_POS),     // DMA2 reset
    RCC_AHB1RSTR_DMA1RST   = (1 << RCC_AHB1RSTR_DMA1RST_POS),     // DMA2 reset
    RCC_AHB1RSTR_CRCRST    = (1 << RCC_AHB1RSTR_CRCRST_POS),      // CRC reset
    RCC_AHB1RSTR_GPIOIRST  = (1 << RCC_AHB1RSTR_GPIOIRST_POS),    // IO port I reset
    RCC_AHB1RSTR_GPIOHRST  = (1 << RCC_AHB1RSTR_GPIOHRST_POS),    // IO port H reset
    RCC_AHB1RSTR_GPIOGRST  = (1 << RCC_AHB1RSTR_GPIOGRST_POS),    // IO port G reset
    RCC_AHB1RSTR_GPIOFRST  = (1 << RCC_AHB1RSTR_GPIOFRST_POS),    // IO port F reset
    RCC_AHB1RSTR_GPIOERST  = (1 << RCC_AHB1RSTR_GPIOERST_POS),    // IO port E reset
    RCC_AHB1RSTR_GPIODRST  = (1 << RCC_AHB1RSTR_GPIODRST_POS),    // IO port D reset
    RCC_AHB1RSTR_GPIOCRST  = (1 << RCC_AHB1RSTR_GPIOCRST_POS),    // IO port C reset
    RCC_AHB1RSTR_GPIOBRST  = (1 << RCC_AHB1RSTR_GPIOBRST_POS),    // IO port B reset
    RCC_AHB1RSTR_GPIOARST  = (1 << RCC_AHB1RSTR_GPIOARST_POS),    // IO port A reset
} rcc_ahb1rstr_e;

typedef enum {
    RCC_AHB1RSTR_OTGHSRST_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_ETHMACRST_MASK = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_DMA2RST_MASK   = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_DMA1RST_MASK   = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_CRCRST_MASK    = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_GPIOIRST_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_GPIOHRST_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_GPIOGRST_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_GPIOFRST_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_GPIOERST_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_GPIODRST_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_GPIOCRST_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_GPIOBRST_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1RSTR_GPIOARST_MASK  = 0b1,    // 1 bit(s)
} rcc_ahb1rstr_mask_e;

typedef enum {
    RCC_AHB2RSTR_OTGFSRST_POS = 7,    // USB OTG FS module reset
    RCC_AHB2RSTR_RNGRST_POS   = 6,    // Random number generator module reset
    RCC_AHB2RSTR_DCMIRST_POS  = 0,    // Camera interface reset
} rcc_ahb2rstr_pos_e;

typedef enum {
    RCC_AHB2RSTR_OTGFSRST = (1 << RCC_AHB2RSTR_OTGFSRST_POS),   // USB OTG FS module reset
    RCC_AHB2RSTR_RNGRST   = (1 << RCC_AHB2RSTR_RNGRST_POS),     // Random number generator module reset
    RCC_AHB2RSTR_DCMIRST  = (1 << RCC_AHB2RSTR_DCMIRST_POS),    // Camera interface reset
} rcc_ahb2rstr_e;

typedef enum {
    RCC_AHB2RSTR_OTGFSRST_MASK = 0b1,    // 1 bit(s)
    RCC_AHB2RSTR_RNGRST_MASK   = 0b1,    // 1 bit(s)
    RCC_AHB2RSTR_DCMIRST_MASK  = 0b1,    // 1 bit(s)
} rcc_ahb2rstr_mask_e;

typedef enum {
    RCC_AHB3RSTR_FSMCRST_POS = 0,    // Flexible static memory controller module reset
} rcc_ahb3rstr_pos_e;

typedef enum {
    RCC_AHB3RSTR_FSMCRST = (1 << RCC_AHB3RSTR_FSMCRST_POS),   // Flexible static memory controller module reset
} rcc_ahb3rstr_e;

typedef enum {
    RCC_AHB3RSTR_FSMCRST_MASK = 0b1,    // 1 bit(s)
} rcc_ahb3rstr_mask_e;

typedef enum {
    RCC_APB1RSTR_DACRST_POS   = 29,    // DAC reset
    RCC_APB1RSTR_PWRRST_POS   = 28,    // Power interface reset
    RCC_APB1RSTR_CAN2RST_POS  = 26,    // CAN2 reset
    RCC_APB1RSTR_CAN1RST_POS  = 25,    // CAN1 reset
    RCC_APB1RSTR_I2C3RST_POS  = 23,    // I2C3 reset
    RCC_APB1RSTR_I2C2RST_POS  = 22,    // I2C 2 reset
    RCC_APB1RSTR_I2C1RST_POS  = 21,    // I2C 1 reset
    RCC_APB1RSTR_UART5RST_POS = 20,    // USART 5 reset
    RCC_APB1RSTR_UART4RST_POS = 19,    // USART 4 reset
    RCC_APB1RSTR_UART3RST_POS = 18,    // USART 3 reset
    RCC_APB1RSTR_UART2RST_POS = 17,    // USART 2 reset
    RCC_APB1RSTR_SPI3RST_POS  = 15,    // SPI 3 reset
    RCC_APB1RSTR_SPI2RST_POS  = 14,    // SPI 2 reset
    RCC_APB1RSTR_WWDGRST_POS  = 11,    // Window watchdog reset
    RCC_APB1RSTR_TIM14RST_POS = 8,     // TIM14 reset
    RCC_APB1RSTR_TIM13RST_POS = 7,     // TIM13 reset
    RCC_APB1RSTR_TIM12RST_POS = 6,     // TIM12 reset
    RCC_APB1RSTR_TIM7RST_POS  = 5,     // TIM7 reset
    RCC_APB1RSTR_TIM6RST_POS  = 4,     // TIM6 reset
    RCC_APB1RSTR_TIM5RST_POS  = 3,     // TIM5 reset
    RCC_APB1RSTR_TIM4RST_POS  = 2,     // TIM4 reset
    RCC_APB1RSTR_TIM3RST_POS  = 1,     // TIM3 reset
    RCC_APB1RSTR_TIM2RST_POS  = 0,     // TIM2 reset
} rcc_apb1rstr_pos_e;

typedef enum {
    RCC_APB1RSTR_DACRST   = (1 << RCC_APB1RSTR_DACRST_POS),     // DAC reset
    RCC_APB1RSTR_PWRRST   = (1 << RCC_APB1RSTR_PWRRST_POS),     // Power interface reset
    RCC_APB1RSTR_CAN2RST  = (1 << RCC_APB1RSTR_CAN2RST_POS),    // CAN2 reset
    RCC_APB1RSTR_CAN1RST  = (1 << RCC_APB1RSTR_CAN1RST_POS),    // CAN1 reset
    RCC_APB1RSTR_I2C3RST  = (1 << RCC_APB1RSTR_I2C3RST_POS),    // I2C3 reset
    RCC_APB1RSTR_I2C2RST  = (1 << RCC_APB1RSTR_I2C2RST_POS),    // I2C 2 reset
    RCC_APB1RSTR_I2C1RST  = (1 << RCC_APB1RSTR_I2C1RST_POS),    // I2C 1 reset
    RCC_APB1RSTR_UART5RST = (1 << RCC_APB1RSTR_UART5RST_POS),   // USART 5 reset
    RCC_APB1RSTR_UART4RST = (1 << RCC_APB1RSTR_UART4RST_POS),   // USART 4 reset
    RCC_APB1RSTR_UART3RST = (1 << RCC_APB1RSTR_UART3RST_POS),   // USART 3 reset
    RCC_APB1RSTR_UART2RST = (1 << RCC_APB1RSTR_UART2RST_POS),   // USART 2 reset
    RCC_APB1RSTR_SPI3RST  = (1 << RCC_APB1RSTR_SPI3RST_POS),    // SPI 3 reset
    RCC_APB1RSTR_SPI2RST  = (1 << RCC_APB1RSTR_SPI2RST_POS),    // SPI 2 reset
    RCC_APB1RSTR_WWDGRST  = (1 << RCC_APB1RSTR_WWDGRST_POS),    // Window watchdog reset
    RCC_APB1RSTR_TIM14RST = (1 << RCC_APB1RSTR_TIM14RST_POS),   // TIM14 reset
    RCC_APB1RSTR_TIM13RST = (1 << RCC_APB1RSTR_TIM13RST_POS),   // TIM13 reset
    RCC_APB1RSTR_TIM12RST = (1 << RCC_APB1RSTR_TIM12RST_POS),   // TIM12 reset
    RCC_APB1RSTR_TIM7RST  = (1 << RCC_APB1RSTR_TIM7RST_POS),    // TIM7 reset
    RCC_APB1RSTR_TIM6RST  = (1 << RCC_APB1RSTR_TIM6RST_POS),    // TIM6 reset
    RCC_APB1RSTR_TIM5RST  = (1 << RCC_APB1RSTR_TIM5RST_POS),    // TIM5 reset
    RCC_APB1RSTR_TIM4RST  = (1 << RCC_APB1RSTR_TIM4RST_POS),    // TIM4 reset
    RCC_APB1RSTR_TIM3RST  = (1 << RCC_APB1RSTR_TIM3RST_POS),    // TIM3 reset
    RCC_APB1RSTR_TIM2RST  = (1 << RCC_APB1RSTR_TIM2RST_POS),    // TIM2 reset
} rcc_apb1rstr_e;

typedef enum {
    RCC_APB1RSTR_DACRST_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_PWRRST_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_CAN2RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_CAN1RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_I2C3RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_I2C2RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_I2C1RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_UART5RST_MASK = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_UART4RST_MASK = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_UART3RST_MASK = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_UART2RST_MASK = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_SPI3RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_SPI2RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_WWDGRST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_TIM14RST_MASK = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_TIM13RST_MASK = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_TIM12RST_MASK = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_TIM7RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_TIM6RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_TIM5RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_TIM4RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_TIM3RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1RSTR_TIM2RST_MASK  = 0b1,    // 1 bit(s)
} rcc_apb1rstr_mask_e;

typedef enum {
    RCC_APB2RSTR_TIM11RST_POS  = 18,    // TIM11 reset
    RCC_APB2RSTR_TIM10RST_POS  = 17,    // TIM10 reset
    RCC_APB2RSTR_TIM9RST_POS   = 16,    // TIM9 reset
    RCC_APB2RSTR_SYSCFGRST_POS = 14,    // System configuration controller reset
    RCC_APB2RSTR_SPI1RST_POS   = 12,    // SPI 1 reset
    RCC_APB2RSTR_SDIORST_POS   = 11,    // SDIO reset
    RCC_APB2RSTR_ADCRST_POS    = 8,     // ADC interface reset (common to all ADCs)
    RCC_APB2RSTR_USART6RST_POS = 5,     // USART6 reset
    RCC_APB2RSTR_USART1RST_POS = 4,     // USART1 reset
    RCC_APB2RSTR_TIM8RST_POS   = 1,     // TIM8 reset
    RCC_APB2RSTR_TIM1RST_POS   = 0,     // TIM1 reset
} rcc_apb2rstr_pos_e;

typedef enum {
    RCC_APB2RSTR_TIM11RST  = (1 << RCC_APB2RSTR_TIM11RST_POS),    // TIM11 reset
    RCC_APB2RSTR_TIM10RST  = (1 << RCC_APB2RSTR_TIM10RST_POS),    // TIM10 reset
    RCC_APB2RSTR_TIM9RST   = (1 << RCC_APB2RSTR_TIM9RST_POS),     // TIM9 reset
    RCC_APB2RSTR_SYSCFGRST = (1 << RCC_APB2RSTR_SYSCFGRST_POS),   // System configuration controller reset
    RCC_APB2RSTR_SPI1RST   = (1 << RCC_APB2RSTR_SPI1RST_POS),     // SPI 1 reset
    RCC_APB2RSTR_SDIORST   = (1 << RCC_APB2RSTR_SDIORST_POS),     // SDIO reset
    RCC_APB2RSTR_ADCRST    = (1 << RCC_APB2RSTR_ADCRST_POS),      // ADC interface reset (common to all ADCs)
    RCC_APB2RSTR_USART6RST = (1 << RCC_APB2RSTR_USART6RST_POS),   // USART6 reset
    RCC_APB2RSTR_USART1RST = (1 << RCC_APB2RSTR_USART1RST_POS),   // USART1 reset
    RCC_APB2RSTR_TIM8RST   = (1 << RCC_APB2RSTR_TIM8RST_POS),     // TIM8 reset
    RCC_APB2RSTR_TIM1RST   = (1 << RCC_APB2RSTR_TIM1RST_POS),     // TIM1 reset
} rcc_apb2rstr_e;

typedef enum {
    RCC_APB2RSTR_TIM11RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB2RSTR_TIM10RST_MASK  = 0b1,    // 1 bit(s)
    RCC_APB2RSTR_TIM9RST_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2RSTR_SYSCFGRST_MASK = 0b1,    // 1 bit(s)
    RCC_APB2RSTR_SPI1RST_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2RSTR_SDIORST_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2RSTR_ADCRST_MASK    = 0b1,    // 1 bit(s)
    RCC_APB2RSTR_USART6RST_MASK = 0b1,    // 1 bit(s)
    RCC_APB2RSTR_USART1RST_MASK = 0b1,    // 1 bit(s)
    RCC_APB2RSTR_TIM8RST_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2RSTR_TIM1RST_MASK   = 0b1,    // 1 bit(s)
} rcc_apb2rstr_mask_e;

typedef enum {
    RCC_AHB1ENR_OTGHSULPIEN_POS  = 30,    // USB OTG HSULPI clock enable
    RCC_AHB1ENR_OTGHSEN_POS      = 29,    // USB OTG HS clock enable
    RCC_AHB1ENR_ETHMACPTPEN_POS  = 28,    // Ethernet PTP clock enable
    RCC_AHB1ENR_ETHMACRXEN_POS   = 27,    // Ethernet Reception clock enable
    RCC_AHB1ENR_ETHMACTXEN_POS   = 26,    // Ethernet Transmission clock enable
    RCC_AHB1ENR_ETHMACEN_POS     = 25,    // Ethernet MAC clock enable
    RCC_AHB1ENR_DMA2EN_POS       = 22,    // DMA2 clock enable
    RCC_AHB1ENR_DMA1EN_POS       = 21,    // DMA1 clock enable
    RCC_AHB1ENR_CCMDATARAMEN_POS = 20,    // CCM data RAM clock enable
    RCC_AHB1ENR_BKPSRAMEN_POS    = 18,    // Backup SRAM interface clock enable
    RCC_AHB1ENR_CRCEN_POS        = 12,    // CRC clock enable
    RCC_AHB1ENR_GPIOIEN_POS      = 8,     // IO port I clock enable
    RCC_AHB1ENR_GPIOHEN_POS      = 7,     // IO port H clock enable
    RCC_AHB1ENR_GPIOGEN_POS      = 6,     // IO port G clock enable
    RCC_AHB1ENR_GPIOFEN_POS      = 5,     // IO port F clock enable
    RCC_AHB1ENR_GPIOEEN_POS      = 4,     // IO port E clock enable
    RCC_AHB1ENR_GPIODEN_POS      = 3,     // IO port D clock enable
    RCC_AHB1ENR_GPIOCEN_POS      = 2,     // IO port C clock enable
    RCC_AHB1ENR_GPIOBEN_POS      = 1,     // IO port B clock enable
    RCC_AHB1ENR_GPIOAEN_POS      = 0,     // IO port A clock enable
} rcc_ahb1enr_pos_e;

typedef enum {
    RCC_AHB1ENR_OTGHSULPIEN  = (1 << RCC_AHB1ENR_OTGHSULPIEN_POS),    // USB OTG HSULPI clock enable
    RCC_AHB1ENR_OTGHSEN      = (1 << RCC_AHB1ENR_OTGHSEN_POS),        // USB OTG HS clock enable
    RCC_AHB1ENR_ETHMACPTPEN  = (1 << RCC_AHB1ENR_ETHMACPTPEN_POS),    // Ethernet PTP clock enable
    RCC_AHB1ENR_ETHMACRXEN   = (1 << RCC_AHB1ENR_ETHMACRXEN_POS),     // Ethernet Reception clock enable
    RCC_AHB1ENR_ETHMACTXEN   = (1 << RCC_AHB1ENR_ETHMACTXEN_POS),     // Ethernet Transmission clock enable
    RCC_AHB1ENR_ETHMACEN     = (1 << RCC_AHB1ENR_ETHMACEN_POS),       // Ethernet MAC clock enable
    RCC_AHB1ENR_DMA2EN       = (1 << RCC_AHB1ENR_DMA2EN_POS),         // DMA2 clock enable
    RCC_AHB1ENR_DMA1EN       = (1 << RCC_AHB1ENR_DMA1EN_POS),         // DMA1 clock enable
    RCC_AHB1ENR_CCMDATARAMEN = (1 << RCC_AHB1ENR_CCMDATARAMEN_POS),   // CCM data RAM clock enable
    RCC_AHB1ENR_BKPSRAMEN    = (1 << RCC_AHB1ENR_BKPSRAMEN_POS),      // Backup SRAM interface clock enable
    RCC_AHB1ENR_CRCEN        = (1 << RCC_AHB1ENR_CRCEN_POS),          // CRC clock enable
    RCC_AHB1ENR_GPIOIEN      = (1 << RCC_AHB1ENR_GPIOIEN_POS),        // IO port I clock enable
    RCC_AHB1ENR_GPIOHEN      = (1 << RCC_AHB1ENR_GPIOHEN_POS),        // IO port H clock enable
    RCC_AHB1ENR_GPIOGEN      = (1 << RCC_AHB1ENR_GPIOGEN_POS),        // IO port G clock enable
    RCC_AHB1ENR_GPIOFEN      = (1 << RCC_AHB1ENR_GPIOFEN_POS),        // IO port F clock enable
    RCC_AHB1ENR_GPIOEEN      = (1 << RCC_AHB1ENR_GPIOEEN_POS),        // IO port E clock enable
    RCC_AHB1ENR_GPIODEN      = (1 << RCC_AHB1ENR_GPIODEN_POS),        // IO port D clock enable
    RCC_AHB1ENR_GPIOCEN      = (1 << RCC_AHB1ENR_GPIOCEN_POS),        // IO port C clock enable
    RCC_AHB1ENR_GPIOBEN      = (1 << RCC_AHB1ENR_GPIOBEN_POS),        // IO port B clock enable
    RCC_AHB1ENR_GPIOAEN      = (1 << RCC_AHB1ENR_GPIOAEN_POS),        // IO port A clock enable
} rcc_ahb1enr_e;

typedef enum {
    RCC_AHB1ENR_OTGHSULPIEN_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_OTGHSEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_ETHMACPTPEN_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_ETHMACRXEN_MASK   = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_ETHMACTXEN_MASK   = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_ETHMACEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_DMA2EN_MASK       = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_DMA1EN_MASK       = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_CCMDATARAMEN_MASK = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_BKPSRAMEN_MASK    = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_CRCEN_MASK        = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_GPIOIEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_GPIOHEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_GPIOGEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_GPIOFEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_GPIOEEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_GPIODEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_GPIOCEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_GPIOBEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1ENR_GPIOAEN_MASK      = 0b1,    // 1 bit(s)
} rcc_ahb1enr_mask_e;

typedef enum {
    RCC_AHB2ENR_OTGFSEN_POS = 7,    // USB OTG FS clock enable
    RCC_AHB2ENR_RNGEN_POS   = 6,    // Random number generator clock enable
    RCC_AHB2ENR_DCMIEN_POS  = 0,    // Camera interface enable
} rcc_ahb2enr_pos_e;

typedef enum {
    RCC_AHB2ENR_OTGFSEN = (1 << RCC_AHB2ENR_OTGFSEN_POS),   // USB OTG FS clock enable
    RCC_AHB2ENR_RNGEN   = (1 << RCC_AHB2ENR_RNGEN_POS),     // Random number generator clock enable
    RCC_AHB2ENR_DCMIEN  = (1 << RCC_AHB2ENR_DCMIEN_POS),    // Camera interface enable
} rcc_ahb2enr_e;

typedef enum {
    RCC_AHB2ENR_OTGFSEN_MASK = 0b1,    // 1 bit(s)
    RCC_AHB2ENR_RNGEN_MASK   = 0b1,    // 1 bit(s)
    RCC_AHB2ENR_DCMIEN_MASK  = 0b1,    // 1 bit(s)
} rcc_ahb2enr_mask_e;

typedef enum {
    RCC_AHB3ENR_FSMCEN_POS = 0,    // Flexible static memory controller module clock enable
} rcc_ahb3enr_pos_e;

typedef enum {
    RCC_AHB3ENR_FSMCEN = (1 << RCC_AHB3ENR_FSMCEN_POS),   // Flexible static memory controller module clock enable
} rcc_ahb3enr_e;

typedef enum {
    RCC_AHB3ENR_FSMCEN_MASK = 0b1,    // 1 bit(s)
} rcc_ahb3enr_mask_e;

typedef enum {
    RCC_APB1ENR_DACEN_POS    = 29,    // DAC interface clock enable
    RCC_APB1ENR_PWREN_POS    = 28,    // Power interface clock enable
    RCC_APB1ENR_CAN2EN_POS   = 26,    // CAN 2 clock enable
    RCC_APB1ENR_CAN1EN_POS   = 25,    // CAN 1 clock enable
    RCC_APB1ENR_I2C3EN_POS   = 23,    // I2C3 clock enable
    RCC_APB1ENR_I2C2EN_POS   = 22,    // I2C2 clock enable
    RCC_APB1ENR_I2C1EN_POS   = 21,    // I2C1 clock enable
    RCC_APB1ENR_UART5EN_POS  = 20,    // UART5 clock enable
    RCC_APB1ENR_UART4EN_POS  = 19,    // UART4 clock enable
    RCC_APB1ENR_USART3EN_POS = 18,    // USART3 clock enable
    RCC_APB1ENR_USART2EN_POS = 17,    // USART 2 clock enable
    RCC_APB1ENR_SPI3EN_POS   = 15,    // SPI3 clock enable
    RCC_APB1ENR_SPI2EN_POS   = 14,    // SPI2 clock enable
    RCC_APB1ENR_WWDGEN_POS   = 11,    // Window watchdog clock enable
    RCC_APB1ENR_TIM14EN_POS  = 8,     // TIM14 clock enable
    RCC_APB1ENR_TIM13EN_POS  = 7,     // TIM13 clock enable
    RCC_APB1ENR_TIM12EN_POS  = 6,     // TIM12 clock enable
    RCC_APB1ENR_TIM7EN_POS   = 5,     // TIM7 clock enable
    RCC_APB1ENR_TIM6EN_POS   = 4,     // TIM6 clock enable
    RCC_APB1ENR_TIM5EN_POS   = 3,     // TIM5 clock enable
    RCC_APB1ENR_TIM4EN_POS   = 2,     // TIM4 clock enable
    RCC_APB1ENR_TIM3EN_POS   = 1,     // TIM3 clock enable
    RCC_APB1ENR_TIM2EN_POS   = 0,     // TIM2 clock enable
} rcc_apb1enr_pos_e;

typedef enum {
    RCC_APB1ENR_DACEN    = (1 << RCC_APB1ENR_DACEN_POS),      // DAC interface clock enable
    RCC_APB1ENR_PWREN    = (1 << RCC_APB1ENR_PWREN_POS),      // Power interface clock enable
    RCC_APB1ENR_CAN2EN   = (1 << RCC_APB1ENR_CAN2EN_POS),     // CAN 2 clock enable
    RCC_APB1ENR_CAN1EN   = (1 << RCC_APB1ENR_CAN1EN_POS),     // CAN 1 clock enable
    RCC_APB1ENR_I2C3EN   = (1 << RCC_APB1ENR_I2C3EN_POS),     // I2C3 clock enable
    RCC_APB1ENR_I2C2EN   = (1 << RCC_APB1ENR_I2C2EN_POS),     // I2C2 clock enable
    RCC_APB1ENR_I2C1EN   = (1 << RCC_APB1ENR_I2C1EN_POS),     // I2C1 clock enable
    RCC_APB1ENR_UART5EN  = (1 << RCC_APB1ENR_UART5EN_POS),    // UART5 clock enable
    RCC_APB1ENR_UART4EN  = (1 << RCC_APB1ENR_UART4EN_POS),    // UART4 clock enable
    RCC_APB1ENR_USART3EN = (1 << RCC_APB1ENR_USART3EN_POS),   // USART3 clock enable
    RCC_APB1ENR_USART2EN = (1 << RCC_APB1ENR_USART2EN_POS),   // USART 2 clock enable
    RCC_APB1ENR_SPI3EN   = (1 << RCC_APB1ENR_SPI3EN_POS),     // SPI3 clock enable
    RCC_APB1ENR_SPI2EN   = (1 << RCC_APB1ENR_SPI2EN_POS),     // SPI2 clock enable
    RCC_APB1ENR_WWDGEN   = (1 << RCC_APB1ENR_WWDGEN_POS),     // Window watchdog clock enable
    RCC_APB1ENR_TIM14EN  = (1 << RCC_APB1ENR_TIM14EN_POS),    // TIM14 clock enable
    RCC_APB1ENR_TIM13EN  = (1 << RCC_APB1ENR_TIM13EN_POS),    // TIM13 clock enable
    RCC_APB1ENR_TIM12EN  = (1 << RCC_APB1ENR_TIM12EN_POS),    // TIM12 clock enable
    RCC_APB1ENR_TIM7EN   = (1 << RCC_APB1ENR_TIM7EN_POS),     // TIM7 clock enable
    RCC_APB1ENR_TIM6EN   = (1 << RCC_APB1ENR_TIM6EN_POS),     // TIM6 clock enable
    RCC_APB1ENR_TIM5EN   = (1 << RCC_APB1ENR_TIM5EN_POS),     // TIM5 clock enable
    RCC_APB1ENR_TIM4EN   = (1 << RCC_APB1ENR_TIM4EN_POS),     // TIM4 clock enable
    RCC_APB1ENR_TIM3EN   = (1 << RCC_APB1ENR_TIM3EN_POS),     // TIM3 clock enable
    RCC_APB1ENR_TIM2EN   = (1 << RCC_APB1ENR_TIM2EN_POS),     // TIM2 clock enable
} rcc_apb1enr_e;

typedef enum {
    RCC_APB1ENR_DACEN_MASK    = 0b1,    // 1 bit(s)
    RCC_APB1ENR_PWREN_MASK    = 0b1,    // 1 bit(s)
    RCC_APB1ENR_CAN2EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_CAN1EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_I2C3EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_I2C2EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_I2C1EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_UART5EN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1ENR_UART4EN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1ENR_USART3EN_MASK = 0b1,    // 1 bit(s)
    RCC_APB1ENR_USART2EN_MASK = 0b1,    // 1 bit(s)
    RCC_APB1ENR_SPI3EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_SPI2EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_WWDGEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_TIM14EN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1ENR_TIM13EN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1ENR_TIM12EN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1ENR_TIM7EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_TIM6EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_TIM5EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_TIM4EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_TIM3EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1ENR_TIM2EN_MASK   = 0b1,    // 1 bit(s)
} rcc_apb1enr_mask_e;

typedef enum {
    RCC_APB2ENR_TIM11EN_POS  = 18,    // TIM11 clock enable
    RCC_APB2ENR_TIM10EN_POS  = 17,    // TIM10 clock enable
    RCC_APB2ENR_TIM9EN_POS   = 16,    // TIM9 clock enable
    RCC_APB2ENR_SYSCFGEN_POS = 14,    // System configuration controller clock enable
    RCC_APB2ENR_SPI1EN_POS   = 12,    // SPI1 clock enable
    RCC_APB2ENR_SDIOEN_POS   = 11,    // SDIO clock enable
    RCC_APB2ENR_ADC3EN_POS   = 10,    // ADC3 clock enable
    RCC_APB2ENR_ADC2EN_POS   = 9,     // ADC2 clock enable
    RCC_APB2ENR_ADC1EN_POS   = 8,     // ADC1 clock enable
    RCC_APB2ENR_USART6EN_POS = 5,     // USART6 clock enable
    RCC_APB2ENR_USART1EN_POS = 4,     // USART1 clock enable
    RCC_APB2ENR_TIM8EN_POS   = 1,     // TIM8 clock enable
    RCC_APB2ENR_TIM1EN_POS   = 0,     // TIM1 clock enable
} rcc_apb2enr_pos_e;

typedef enum {
    RCC_APB2ENR_TIM11EN  = (1 << RCC_APB2ENR_TIM11EN_POS),    // TIM11 clock enable
    RCC_APB2ENR_TIM10EN  = (1 << RCC_APB2ENR_TIM10EN_POS),    // TIM10 clock enable
    RCC_APB2ENR_TIM9EN   = (1 << RCC_APB2ENR_TIM9EN_POS),     // TIM9 clock enable
    RCC_APB2ENR_SYSCFGEN = (1 << RCC_APB2ENR_SYSCFGEN_POS),   // System configuration controller clock enable
    RCC_APB2ENR_SPI1EN   = (1 << RCC_APB2ENR_SPI1EN_POS),     // SPI1 clock enable
    RCC_APB2ENR_SDIOEN   = (1 << RCC_APB2ENR_SDIOEN_POS),     // SDIO clock enable
    RCC_APB2ENR_ADC3EN   = (1 << RCC_APB2ENR_ADC3EN_POS),     // ADC3 clock enable
    RCC_APB2ENR_ADC2EN   = (1 << RCC_APB2ENR_ADC2EN_POS),     // ADC2 clock enable
    RCC_APB2ENR_ADC1EN   = (1 << RCC_APB2ENR_ADC1EN_POS),     // ADC1 clock enable
    RCC_APB2ENR_USART6EN = (1 << RCC_APB2ENR_USART6EN_POS),   // USART6 clock enable
    RCC_APB2ENR_USART1EN = (1 << RCC_APB2ENR_USART1EN_POS),   // USART1 clock enable
    RCC_APB2ENR_TIM8EN   = (1 << RCC_APB2ENR_TIM8EN_POS),     // TIM8 clock enable
    RCC_APB2ENR_TIM1EN   = (1 << RCC_APB2ENR_TIM1EN_POS),     // TIM1 clock enable
} rcc_apb2enr_e;

typedef enum {
    RCC_APB2ENR_TIM11EN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB2ENR_TIM10EN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB2ENR_TIM9EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2ENR_SYSCFGEN_MASK = 0b1,    // 1 bit(s)
    RCC_APB2ENR_SPI1EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2ENR_SDIOEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2ENR_ADC3EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2ENR_ADC2EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2ENR_ADC1EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2ENR_USART6EN_MASK = 0b1,    // 1 bit(s)
    RCC_APB2ENR_USART1EN_MASK = 0b1,    // 1 bit(s)
    RCC_APB2ENR_TIM8EN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2ENR_TIM1EN_MASK   = 0b1,    // 1 bit(s)
} rcc_apb2enr_mask_e;

typedef enum {
    RCC_AHB1LPENR_OTGHSULPILPEN_POS = 30,    // USB OTG HS ULPI clock enable during Sleep mode
    RCC_AHB1LPENR_OTGHSLPEN_POS     = 29,    // USB OTG HS clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACPTPLPEN_POS = 28,    // Ethernet PTP clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACRXLPEN_POS  = 27,    // Ethernet reception clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACTXLPEN_POS  = 26,    // Ethernet transmission clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACLPEN_POS    = 25,    // Ethernet MAC clock enable during Sleep mode
    RCC_AHB1LPENR_DMA2LPEN_POS      = 22,    // DMA2 clock enable during Sleep mode
    RCC_AHB1LPENR_DMA1LPEN_POS      = 21,    // DMA1 clock enable during Sleep mode
    RCC_AHB1LPENR_BKPSRAMLPEN_POS   = 18,    // Backup SRAM interface clock enable during Sleep mode
    RCC_AHB1LPENR_SRAM2LPEN_POS     = 17,    // SRAM 2 interface clock enable during Sleep mode
    RCC_AHB1LPENR_SRAM1LPEN_POS     = 16,    // SRAM 1interface clock enable during Sleep mode
    RCC_AHB1LPENR_FLITFLPEN_POS     = 15,    // Flash interface clock enable during Sleep mode
    RCC_AHB1LPENR_CRCLPEN_POS       = 12,    // CRC clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOILPEN_POS     = 8,     // IO port I clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOHLPEN_POS     = 7,     // IO port H clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOGLPEN_POS     = 6,     // IO port G clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOFLPEN_POS     = 5,     // IO port F clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOELPEN_POS     = 4,     // IO port E clock enable during Sleep mode
    RCC_AHB1LPENR_GPIODLPEN_POS     = 3,     // IO port D clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOCLPEN_POS     = 2,     // IO port C clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOBLPEN_POS     = 1,     // IO port B clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOALPEN_POS     = 0,     // IO port A clock enable during sleep mode
} rcc_ahb1lpenr_pos_e;

typedef enum {
    RCC_AHB1LPENR_OTGHSULPILPEN = (1 << RCC_AHB1LPENR_OTGHSULPILPEN_POS),   // USB OTG HS ULPI clock enable during Sleep mode
    RCC_AHB1LPENR_OTGHSLPEN     = (1 << RCC_AHB1LPENR_OTGHSLPEN_POS),       // USB OTG HS clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACPTPLPEN = (1 << RCC_AHB1LPENR_ETHMACPTPLPEN_POS),   // Ethernet PTP clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACRXLPEN  = (1 << RCC_AHB1LPENR_ETHMACRXLPEN_POS),    // Ethernet reception clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACTXLPEN  = (1 << RCC_AHB1LPENR_ETHMACTXLPEN_POS),    // Ethernet transmission clock enable during Sleep mode
    RCC_AHB1LPENR_ETHMACLPEN    = (1 << RCC_AHB1LPENR_ETHMACLPEN_POS),      // Ethernet MAC clock enable during Sleep mode
    RCC_AHB1LPENR_DMA2LPEN      = (1 << RCC_AHB1LPENR_DMA2LPEN_POS),        // DMA2 clock enable during Sleep mode
    RCC_AHB1LPENR_DMA1LPEN      = (1 << RCC_AHB1LPENR_DMA1LPEN_POS),        // DMA1 clock enable during Sleep mode
    RCC_AHB1LPENR_BKPSRAMLPEN   = (1 << RCC_AHB1LPENR_BKPSRAMLPEN_POS),     // Backup SRAM interface clock enable during Sleep mode
    RCC_AHB1LPENR_SRAM2LPEN     = (1 << RCC_AHB1LPENR_SRAM2LPEN_POS),       // SRAM 2 interface clock enable during Sleep mode
    RCC_AHB1LPENR_SRAM1LPEN     = (1 << RCC_AHB1LPENR_SRAM1LPEN_POS),       // SRAM 1interface clock enable during Sleep mode
    RCC_AHB1LPENR_FLITFLPEN     = (1 << RCC_AHB1LPENR_FLITFLPEN_POS),       // Flash interface clock enable during Sleep mode
    RCC_AHB1LPENR_CRCLPEN       = (1 << RCC_AHB1LPENR_CRCLPEN_POS),         // CRC clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOILPEN     = (1 << RCC_AHB1LPENR_GPIOILPEN_POS),       // IO port I clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOHLPEN     = (1 << RCC_AHB1LPENR_GPIOHLPEN_POS),       // IO port H clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOGLPEN     = (1 << RCC_AHB1LPENR_GPIOGLPEN_POS),       // IO port G clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOFLPEN     = (1 << RCC_AHB1LPENR_GPIOFLPEN_POS),       // IO port F clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOELPEN     = (1 << RCC_AHB1LPENR_GPIOELPEN_POS),       // IO port E clock enable during Sleep mode
    RCC_AHB1LPENR_GPIODLPEN     = (1 << RCC_AHB1LPENR_GPIODLPEN_POS),       // IO port D clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOCLPEN     = (1 << RCC_AHB1LPENR_GPIOCLPEN_POS),       // IO port C clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOBLPEN     = (1 << RCC_AHB1LPENR_GPIOBLPEN_POS),       // IO port B clock enable during Sleep mode
    RCC_AHB1LPENR_GPIOALPEN     = (1 << RCC_AHB1LPENR_GPIOALPEN_POS),       // IO port A clock enable during sleep mode
} rcc_ahb1lpenr_e;

typedef enum {
    RCC_AHB1LPENR_OTGHSULPILPEN_MASK = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_OTGHSLPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_ETHMACPTPLPEN_MASK = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_ETHMACRXLPEN_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_ETHMACTXLPEN_MASK  = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_ETHMACLPEN_MASK    = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_DMA2LPEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_DMA1LPEN_MASK      = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_BKPSRAMLPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_SRAM2LPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_SRAM1LPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_FLITFLPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_CRCLPEN_MASK       = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_GPIOILPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_GPIOHLPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_GPIOGLPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_GPIOFLPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_GPIOELPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_GPIODLPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_GPIOCLPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_GPIOBLPEN_MASK     = 0b1,    // 1 bit(s)
    RCC_AHB1LPENR_GPIOALPEN_MASK     = 0b1,    // 1 bit(s)
} rcc_ahb1lpenr_mask_e;

typedef enum {
    RCC_AHB2LPENR_OTGFSLPEN_POS = 7,    // USB OTG FS clock enable during Sleep mode
    RCC_AHB2LPENR_RNGLPEN_POS   = 6,    // Random number generator clock enable during Sleep mode
    RCC_AHB2LPENR_DCMILPEN_POS  = 0,    // Camera interface enable during Sleep mode
} rcc_ahb2lpenr_pos_e;

typedef enum {
    RCC_AHB2LPENR_OTGFSLPEN = (1 << RCC_AHB2LPENR_OTGFSLPEN_POS),   // USB OTG FS clock enable during Sleep mode
    RCC_AHB2LPENR_RNGLPEN   = (1 << RCC_AHB2LPENR_RNGLPEN_POS),     // Random number generator clock enable during Sleep mode
    RCC_AHB2LPENR_DCMILPEN  = (1 << RCC_AHB2LPENR_DCMILPEN_POS),    // Camera interface enable during Sleep mode
} rcc_ahb2lpenr_e;

typedef enum {
    RCC_AHB2LPENR_OTGFSLPEN_MASK = 0b1,    // 1 bit(s)
    RCC_AHB2LPENR_RNGLPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_AHB2LPENR_DCMILPEN_MASK  = 0b1,    // 1 bit(s)
} rcc_ahb2lpenr_mask_e;

typedef enum {
    RCC_AHB3LPENR_FSMCLPEN_POS = 0,    // Flexible static memory controller module clock enable during Sleep mode
} rcc_ahb3lpenr_pos_e;

typedef enum {
    RCC_AHB3LPENR_FSMCLPEN = (1 << RCC_AHB3LPENR_FSMCLPEN_POS),   // Flexible static memory controller module clock enable during Sleep mode
} rcc_ahb3lpenr_e;

typedef enum {
    RCC_AHB3LPENR_FSMCLPEN_MASK = 0b1,    // 1 bit(s)
} rcc_ahb3lpenr_mask_e;

typedef enum {
    RCC_APB1LPENR_DACLPEN_POS    = 29,    // DAC interface clock enable during Sleep mode
    RCC_APB1LPENR_PWRLPEN_POS    = 28,    // Power interface clock enable during Sleep mode
    RCC_APB1LPENR_CAN2LPEN_POS   = 26,    // CAN 2 clock enable during Sleep mode
    RCC_APB1LPENR_CAN1LPEN_POS   = 25,    // CAN 1 clock enable during Sleep mode
    RCC_APB1LPENR_I2C3LPEN_POS   = 23,    // I2C3 clock enable during Sleep mode
    RCC_APB1LPENR_I2C2LPEN_POS   = 22,    // I2C2 clock enable during Sleep mode
    RCC_APB1LPENR_I2C1LPEN_POS   = 21,    // I2C1 clock enable during Sleep mode
    RCC_APB1LPENR_UART5LPEN_POS  = 20,    // UART5 clock enable during Sleep mode
    RCC_APB1LPENR_UART4LPEN_POS  = 19,    // UART4 clock enable during Sleep mode
    RCC_APB1LPENR_USART3LPEN_POS = 18,    // USART3 clock enable during Sleep mode
    RCC_APB1LPENR_USART2LPEN_POS = 17,    // USART2 clock enable during Sleep mode
    RCC_APB1LPENR_SPI3LPEN_POS   = 15,    // SPI3 clock enable during Sleep mode
    RCC_APB1LPENR_SPI2LPEN_POS   = 14,    // SPI2 clock enable during Sleep mode
    RCC_APB1LPENR_WWDGLPEN_POS   = 11,    // Window watchdog clock enable during Sleep mode
    RCC_APB1LPENR_TIM14LPEN_POS  = 8,     // TIM14 clock enable during Sleep mode
    RCC_APB1LPENR_TIM13LPEN_POS  = 7,     // TIM13 clock enable during Sleep mode
    RCC_APB1LPENR_TIM12LPEN_POS  = 6,     // TIM12 clock enable during Sleep mode
    RCC_APB1LPENR_TIM7LPEN_POS   = 5,     // TIM7 clock enable during Sleep mode
    RCC_APB1LPENR_TIM6LPEN_POS   = 4,     // TIM6 clock enable during Sleep mode
    RCC_APB1LPENR_TIM5LPEN_POS   = 3,     // TIM5 clock enable during Sleep mode
    RCC_APB1LPENR_TIM4LPEN_POS   = 2,     // TIM4 clock enable during Sleep mode
    RCC_APB1LPENR_TIM3LPEN_POS   = 1,     // TIM3 clock enable during Sleep mode
    RCC_APB1LPENR_TIM2LPEN_POS   = 0,     // TIM2 clock enable during Sleep mode
} rcc_apb1lpenr_pos_e;

typedef enum {
    RCC_APB1LPENR_DACLPEN    = (1 << RCC_APB1LPENR_DACLPEN_POS),      // DAC interface clock enable during Sleep mode
    RCC_APB1LPENR_PWRLPEN    = (1 << RCC_APB1LPENR_PWRLPEN_POS),      // Power interface clock enable during Sleep mode
    RCC_APB1LPENR_CAN2LPEN   = (1 << RCC_APB1LPENR_CAN2LPEN_POS),     // CAN 2 clock enable during Sleep mode
    RCC_APB1LPENR_CAN1LPEN   = (1 << RCC_APB1LPENR_CAN1LPEN_POS),     // CAN 1 clock enable during Sleep mode
    RCC_APB1LPENR_I2C3LPEN   = (1 << RCC_APB1LPENR_I2C3LPEN_POS),     // I2C3 clock enable during Sleep mode
    RCC_APB1LPENR_I2C2LPEN   = (1 << RCC_APB1LPENR_I2C2LPEN_POS),     // I2C2 clock enable during Sleep mode
    RCC_APB1LPENR_I2C1LPEN   = (1 << RCC_APB1LPENR_I2C1LPEN_POS),     // I2C1 clock enable during Sleep mode
    RCC_APB1LPENR_UART5LPEN  = (1 << RCC_APB1LPENR_UART5LPEN_POS),    // UART5 clock enable during Sleep mode
    RCC_APB1LPENR_UART4LPEN  = (1 << RCC_APB1LPENR_UART4LPEN_POS),    // UART4 clock enable during Sleep mode
    RCC_APB1LPENR_USART3LPEN = (1 << RCC_APB1LPENR_USART3LPEN_POS),   // USART3 clock enable during Sleep mode
    RCC_APB1LPENR_USART2LPEN = (1 << RCC_APB1LPENR_USART2LPEN_POS),   // USART2 clock enable during Sleep mode
    RCC_APB1LPENR_SPI3LPEN   = (1 << RCC_APB1LPENR_SPI3LPEN_POS),     // SPI3 clock enable during Sleep mode
    RCC_APB1LPENR_SPI2LPEN   = (1 << RCC_APB1LPENR_SPI2LPEN_POS),     // SPI2 clock enable during Sleep mode
    RCC_APB1LPENR_WWDGLPEN   = (1 << RCC_APB1LPENR_WWDGLPEN_POS),     // Window watchdog clock enable during Sleep mode
    RCC_APB1LPENR_TIM14LPEN  = (1 << RCC_APB1LPENR_TIM14LPEN_POS),    // TIM14 clock enable during Sleep mode
    RCC_APB1LPENR_TIM13LPEN  = (1 << RCC_APB1LPENR_TIM13LPEN_POS),    // TIM13 clock enable during Sleep mode
    RCC_APB1LPENR_TIM12LPEN  = (1 << RCC_APB1LPENR_TIM12LPEN_POS),    // TIM12 clock enable during Sleep mode
    RCC_APB1LPENR_TIM7LPEN   = (1 << RCC_APB1LPENR_TIM7LPEN_POS),     // TIM7 clock enable during Sleep mode
    RCC_APB1LPENR_TIM6LPEN   = (1 << RCC_APB1LPENR_TIM6LPEN_POS),     // TIM6 clock enable during Sleep mode
    RCC_APB1LPENR_TIM5LPEN   = (1 << RCC_APB1LPENR_TIM5LPEN_POS),     // TIM5 clock enable during Sleep mode
    RCC_APB1LPENR_TIM4LPEN   = (1 << RCC_APB1LPENR_TIM4LPEN_POS),     // TIM4 clock enable during Sleep mode
    RCC_APB1LPENR_TIM3LPEN   = (1 << RCC_APB1LPENR_TIM3LPEN_POS),     // TIM3 clock enable during Sleep mode
    RCC_APB1LPENR_TIM2LPEN   = (1 << RCC_APB1LPENR_TIM2LPEN_POS),     // TIM2 clock enable during Sleep mode
} rcc_apb1lpenr_e;

typedef enum {
    RCC_APB1LPENR_DACLPEN_MASK    = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_PWRLPEN_MASK    = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_CAN2LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_CAN1LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_I2C3LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_I2C2LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_I2C1LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_UART5LPEN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_UART4LPEN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_USART3LPEN_MASK = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_USART2LPEN_MASK = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_SPI3LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_SPI2LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_WWDGLPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_TIM14LPEN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_TIM13LPEN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_TIM12LPEN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_TIM7LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_TIM6LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_TIM5LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_TIM4LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_TIM3LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB1LPENR_TIM2LPEN_MASK   = 0b1,    // 1 bit(s)
} rcc_apb1lpenr_mask_e;

typedef enum {
    RCC_APB2LPENR_TIM11LPEN_POS  = 18,    // TIM11 clock enable during Sleep mode
    RCC_APB2LPENR_TIM10LPEN_POS  = 17,    // TIM10 clock enable during Sleep mode
    RCC_APB2LPENR_TIM9LPEN_POS   = 16,    // TIM9 clock enable during sleep mode
    RCC_APB2LPENR_SYSCFGLPEN_POS = 14,    // System configuration controller clock enable during Sleep mode
    RCC_APB2LPENR_SPI1LPEN_POS   = 12,    // SPI 1 clock enable during Sleep mode
    RCC_APB2LPENR_SDIOLPEN_POS   = 11,    // SDIO clock enable during Sleep mode
    RCC_APB2LPENR_ADC3LPEN_POS   = 10,    // ADC 3 clock enable during Sleep mode
    RCC_APB2LPENR_ADC2LPEN_POS   = 9,     // ADC2 clock enable during Sleep mode
    RCC_APB2LPENR_ADC1LPEN_POS   = 8,     // ADC1 clock enable during Sleep mode
    RCC_APB2LPENR_USART6LPEN_POS = 5,     // USART6 clock enable during Sleep mode
    RCC_APB2LPENR_USART1LPEN_POS = 4,     // USART1 clock enable during Sleep mode
    RCC_APB2LPENR_TIM8LPEN_POS   = 1,     // TIM8 clock enable during Sleep mode
    RCC_APB2LPENR_TIM1LPEN_POS   = 0,     // TIM1 clock enable during Sleep mode
} rcc_apb2lpenr_pos_e;

typedef enum {
    RCC_APB2LPENR_TIM11LPEN  = (1 << RCC_APB2LPENR_TIM11LPEN_POS),    // TIM11 clock enable during Sleep mode
    RCC_APB2LPENR_TIM10LPEN  = (1 << RCC_APB2LPENR_TIM10LPEN_POS),    // TIM10 clock enable during Sleep mode
    RCC_APB2LPENR_TIM9LPEN   = (1 << RCC_APB2LPENR_TIM9LPEN_POS),     // TIM9 clock enable during sleep mode
    RCC_APB2LPENR_SYSCFGLPEN = (1 << RCC_APB2LPENR_SYSCFGLPEN_POS),   // System configuration controller clock enable during Sleep mode
    RCC_APB2LPENR_SPI1LPEN   = (1 << RCC_APB2LPENR_SPI1LPEN_POS),     // SPI 1 clock enable during Sleep mode
    RCC_APB2LPENR_SDIOLPEN   = (1 << RCC_APB2LPENR_SDIOLPEN_POS),     // SDIO clock enable during Sleep mode
    RCC_APB2LPENR_ADC3LPEN   = (1 << RCC_APB2LPENR_ADC3LPEN_POS),     // ADC 3 clock enable during Sleep mode
    RCC_APB2LPENR_ADC2LPEN   = (1 << RCC_APB2LPENR_ADC2LPEN_POS),     // ADC2 clock enable during Sleep mode
    RCC_APB2LPENR_ADC1LPEN   = (1 << RCC_APB2LPENR_ADC1LPEN_POS),     // ADC1 clock enable during Sleep mode
    RCC_APB2LPENR_USART6LPEN = (1 << RCC_APB2LPENR_USART6LPEN_POS),   // USART6 clock enable during Sleep mode
    RCC_APB2LPENR_USART1LPEN = (1 << RCC_APB2LPENR_USART1LPEN_POS),   // USART1 clock enable during Sleep mode
    RCC_APB2LPENR_TIM8LPEN   = (1 << RCC_APB2LPENR_TIM8LPEN_POS),     // TIM8 clock enable during Sleep mode
    RCC_APB2LPENR_TIM1LPEN   = (1 << RCC_APB2LPENR_TIM1LPEN_POS),     // TIM1 clock enable during Sleep mode
} rcc_apb2lpenr_e;

typedef enum {
    RCC_APB2LPENR_TIM11LPEN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_TIM10LPEN_MASK  = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_TIM9LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_SYSCFGLPEN_MASK = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_SPI1LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_SDIOLPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_ADC3LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_ADC2LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_ADC1LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_USART6LPEN_MASK = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_USART1LPEN_MASK = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_TIM8LPEN_MASK   = 0b1,    // 1 bit(s)
    RCC_APB2LPENR_TIM1LPEN_MASK   = 0b1,    // 1 bit(s)
} rcc_apb2lpenr_mask_e;

typedef enum {
    RCC_BDCR_BDRST_POS   = 16,    // Backup domain software reset
    RCC_BDCR_RTCEN_POS   = 15,    // RTC clock enable
    RCC_BDCR_RTCSEL1_POS = 9,     // RTC clock source selection
    RCC_BDCR_RTCSEL0_POS = 8,     // RTC clock source selection
    RCC_BDCR_LSEBYP_POS  = 2,     // External low-speed oscillator bypass
    RCC_BDCR_LSERDY_POS  = 1,     // External low-speed oscillator ready
    RCC_BDCR_LSEON_POS   = 0,     // External low-speed oscillator enable
} rcc_bdcr_pos_e;

typedef enum {
    RCC_BDCR_BDRST   = (1 << RCC_BDCR_BDRST_POS),     // Backup domain software reset
    RCC_BDCR_RTCEN   = (1 << RCC_BDCR_RTCEN_POS),     // RTC clock enable
    RCC_BDCR_RTCSEL1 = (1 << RCC_BDCR_RTCSEL1_POS),   // RTC clock source selection
    RCC_BDCR_RTCSEL0 = (1 << RCC_BDCR_RTCSEL0_POS),   // RTC clock source selection
    RCC_BDCR_LSEBYP  = (1 << RCC_BDCR_LSEBYP_POS),    // External low-speed oscillator bypass
    RCC_BDCR_LSERDY  = (1 << RCC_BDCR_LSERDY_POS),    // External low-speed oscillator ready
    RCC_BDCR_LSEON   = (1 << RCC_BDCR_LSEON_POS),     // External low-speed oscillator enable
} rcc_bdcr_e;

typedef enum {
    RCC_BDCR_BDRST_MASK   = 0b1,    // 1 bit(s)
    RCC_BDCR_RTCEN_MASK   = 0b1,    // 1 bit(s)
    RCC_BDCR_RTCSEL1_MASK = 0b1,    // 1 bit(s)
    RCC_BDCR_RTCSEL0_MASK = 0b1,    // 1 bit(s)
    RCC_BDCR_LSEBYP_MASK  = 0b1,    // 1 bit(s)
    RCC_BDCR_LSERDY_MASK  = 0b1,    // 1 bit(s)
    RCC_BDCR_LSEON_MASK   = 0b1,    // 1 bit(s)
} rcc_bdcr_mask_e;

typedef enum {
    RCC_CSR_LPWRRSTF_POS = 31,    // Low-power reset flag
    RCC_CSR_WWDGRSTF_POS = 30,    // Window watchdog reset flag
    RCC_CSR_WDGRSTF_POS  = 29,    // Independent watchdog reset flag
    RCC_CSR_SFTRSTF_POS  = 28,    // Software reset flag
    RCC_CSR_PORRSTF_POS  = 27,    // POR/PDR reset flag
    RCC_CSR_PADRSTF_POS  = 26,    // PIN reset flag
    RCC_CSR_BORRSTF_POS  = 25,    // BOR reset flag
    RCC_CSR_RMVF_POS     = 24,    // Remove reset flag
    RCC_CSR_LSIRDY_POS   = 1,     // Internal low-speed oscillator ready
    RCC_CSR_LSION_POS    = 0,     // Internal low-speed oscillator enable
} rcc_csr_pos_e;

typedef enum {
    RCC_CSR_LPWRRSTF = (1 << RCC_CSR_LPWRRSTF_POS),   // Low-power reset flag
    RCC_CSR_WWDGRSTF = (1 << RCC_CSR_WWDGRSTF_POS),   // Window watchdog reset flag
    RCC_CSR_WDGRSTF  = (1 << RCC_CSR_WDGRSTF_POS),    // Independent watchdog reset flag
    RCC_CSR_SFTRSTF  = (1 << RCC_CSR_SFTRSTF_POS),    // Software reset flag
    RCC_CSR_PORRSTF  = (1 << RCC_CSR_PORRSTF_POS),    // POR/PDR reset flag
    RCC_CSR_PADRSTF  = (1 << RCC_CSR_PADRSTF_POS),    // PIN reset flag
    RCC_CSR_BORRSTF  = (1 << RCC_CSR_BORRSTF_POS),    // BOR reset flag
    RCC_CSR_RMVF     = (1 << RCC_CSR_RMVF_POS),       // Remove reset flag
    RCC_CSR_LSIRDY   = (1 << RCC_CSR_LSIRDY_POS),     // Internal low-speed oscillator ready
    RCC_CSR_LSION    = (1 << RCC_CSR_LSION_POS),      // Internal low-speed oscillator enable
} rcc_csr_e;

typedef enum {
    RCC_CSR_LPWRRSTF_MASK = 0b1,    // 1 bit(s)
    RCC_CSR_WWDGRSTF_MASK = 0b1,    // 1 bit(s)
    RCC_CSR_WDGRSTF_MASK  = 0b1,    // 1 bit(s)
    RCC_CSR_SFTRSTF_MASK  = 0b1,    // 1 bit(s)
    RCC_CSR_PORRSTF_MASK  = 0b1,    // 1 bit(s)
    RCC_CSR_PADRSTF_MASK  = 0b1,    // 1 bit(s)
    RCC_CSR_BORRSTF_MASK  = 0b1,    // 1 bit(s)
    RCC_CSR_RMVF_MASK     = 0b1,    // 1 bit(s)
    RCC_CSR_LSIRDY_MASK   = 0b1,    // 1 bit(s)
    RCC_CSR_LSION_MASK    = 0b1,    // 1 bit(s)
} rcc_csr_mask_e;

typedef enum {
    RCC_SSCGR_SSCGEN_POS    = 31,    // Spread spectrum modulation enable
    RCC_SSCGR_SPREADSEL_POS = 30,    // Spread Select
    RCC_SSCGR_INCSTEP_POS   = 13,    // Incrementation step
    RCC_SSCGR_MODPER_POS    = 0,     // Modulation period
} rcc_sscgr_pos_e;

typedef enum {
    RCC_SSCGR_SSCGEN    = (1 << RCC_SSCGR_SSCGEN_POS),      // Spread spectrum modulation enable
    RCC_SSCGR_SPREADSEL = (1 << RCC_SSCGR_SPREADSEL_POS),   // Spread Select
    RCC_SSCGR_INCSTEP   = (1 << RCC_SSCGR_INCSTEP_POS),     // Incrementation step
    RCC_SSCGR_MODPER    = (1 << RCC_SSCGR_MODPER_POS),      // Modulation period
} rcc_sscgr_e;

typedef enum {
    RCC_SSCGR_SSCGEN_MASK    = 0b1,                  // 1 bit(s)
    RCC_SSCGR_SPREADSEL_MASK = 0b1,                  // 1 bit(s)
    RCC_SSCGR_INCSTEP_MASK   = 0b111111111111111,    // 15 bit(s)
    RCC_SSCGR_MODPER_MASK    = 0b1111111111111,      // 13 bit(s)
} rcc_sscgr_mask_e;

typedef enum {
    RCC_PLLI2SCFGR_PLLI2SRX_POS = 28,    // PLLI2S division factor for I2S clocks
    RCC_PLLI2SCFGR_PLLI2SNX_POS = 6,     // PLLI2S multiplication factor for VCO
} rcc_plli2scfgr_pos_e;

typedef enum {
    RCC_PLLI2SCFGR_PLLI2SRX = (1 << RCC_PLLI2SCFGR_PLLI2SRX_POS),   // PLLI2S division factor for I2S clocks
    RCC_PLLI2SCFGR_PLLI2SNX = (1 << RCC_PLLI2SCFGR_PLLI2SNX_POS),   // PLLI2S multiplication factor for VCO
} rcc_plli2scfgr_e;

typedef enum {
    RCC_PLLI2SCFGR_PLLI2SRX_MASK = 0b111,          // 3 bit(s)
    RCC_PLLI2SCFGR_PLLI2SNX_MASK = 0b111111111,    // 9 bit(s)
} rcc_plli2scfgr_mask_e;

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

// RCC Register Definition Structure
typedef __vo struct {
    uint32_t CR;             // clock control register                                        Offset: 0x0
    uint32_t PLLCFGR;        // PLL configuration register                                    Offset: 0x4
    uint32_t CFGR;           // clock configuration register                                  Offset: 0x8
    uint32_t CIR;            // clock interrupt register                                      Offset: 0xC
    uint32_t AHB1RSTR;       // AHB1 peripheral reset register                                Offset: 0x10
    uint32_t AHB2RSTR;       // AHB2 peripheral reset register                                Offset: 0x14
    uint32_t AHB3RSTR;       // AHB3 peripheral reset register                                Offset: 0x18
    uint32_t reserved_1;     // Reserved 0x1C
    uint32_t APB1RSTR;       // APB1 peripheral reset register                                Offset: 0x20
    uint32_t APB2RSTR;       // APB2 peripheral reset register                                Offset: 0x24
    uint32_t reserved_2;     // Reserved 0x28
    uint32_t reserved_3;     // Reserved 0x2C
    uint32_t AHB1ENR;        // AHB1 peripheral clock register                                Offset: 0x30
    uint32_t AHB2ENR;        // AHB2 peripheral clock enable register                         Offset: 0x34
    uint32_t AHB3ENR;        // AHB3 peripheral clock enable register                         Offset: 0x38
    uint32_t reserved_4;     // Reserved 0x3C
    uint32_t APB1ENR;        // APB1 peripheral clock enable register                         Offset: 0x40
    uint32_t APB2ENR;        // APB2 peripheral clock enable register                         Offset: 0x44
    uint32_t reserved_5;     // Reserved 0x48
    uint32_t reserved_6;     // Reserved 0x4C
    uint32_t AHB1LPENR;      // AHB1 peripheral clock enable in low power mode register       Offset: 0x50
    uint32_t AHB2LPENR;      // AHB2 peripheral clock enable in low power mode register       Offset: 0x54
    uint32_t AHB3LPENR;      // AHB3 peripheral clock enable in low power mode register       Offset: 0x58
    uint32_t reserved_7;     // Reserved 0x5C
    uint32_t APB1LPENR;      // APB1 peripheral clock enable in low power mode register       Offset: 0x60
    uint32_t APB2LPENR;      // APB2 peripheral clock enabled in low power mode register      Offset: 0x64
    uint32_t reserved_8;     // Reserved 0x68
    uint32_t reserved_9;     // Reserved 0x6C
    uint32_t BDCR;           // Backup domain control register                                Offset: 0x70
    uint32_t CSR;            // clock control & status register                               Offset: 0x74
    uint32_t reserved_10;    // Reserved 0x78
    uint32_t reserved_11;    // Reserved 0x7C
    uint32_t SSCGR;          // spread spectrum clock generation register                     Offset: 0x80
    uint32_t PLLI2SCFGR;     // PLLI2S configuration register                                 Offset: 0x84
} rcc_reg_def;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define RCC ((rcc_reg_def *)RCC_BASE_ADDR)

//======================================================================================//}
//                  Peripheral Function APIs
//======================================================================================//{

uint32_t rcc_get_pll_freq_hz(void);
uint32_t rcc_get_sys_clock_freq_hz(void);
uint32_t rcc_get_bus_clock_freq_hz(bus_types bus);
uint32_t rcc_get_timer_clock_freq_hz(bus_types bus);
void rcc_mco_config(rcc_mco_clock_src_e mco_clk_src, rcc_mco_prescaler_e mco_prescaler);

#endif
