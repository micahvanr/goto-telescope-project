#ifndef RCC_H
#define RCC_H

#include "gpio.h"
#include "stm32f4xx.h"
/****************************************************************************************************
                                        Address Definitions
****************************************************************************************************/
#define RCC_BASE_ADDR (0x40023800ul)

/****************************************************************************************************
                                                Macros and Other Enums
****************************************************************************************************/

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

// Register enums

// RCC_PLL_CFGR
typedef enum {
    RCC_PLL_CFGR_PLLM_POS   = 0,
    RCC_PLL_CFGR_PLLN_POS   = 6,
    RCC_PLL_CFGR_PLLP_POS   = 16,
    RCC_PLL_CFGR_PLLSRC_POS = 22,
    RCC_PLL_CFGR_PLLQ_POS   = 24,
} rcc_pll_cfgr_pos_e;

typedef enum {
    RCC_PLL_CFGR_PLLM_MASK   = 0b111111,    // 6 bits
    RCC_PLL_CFGR_PLLN_MASK   = 0b111111111, // 9 bits
    RCC_PLL_CFGR_PLLP_MASK   = 0b11,
    RCC_PLL_CFGR_PLLSRC_MASK = 0b1,
    RCC_PLL_CFGR_PLLQ_MASK   = 0b1111, // 4 bits
} rcc_pll_cfgr_mask_e;

// RCC_CFGR
typedef enum {
    RCC_CFGR_SW_POS       = 0,
    RCC_CFGR_SWS_POS      = 2,
    RCC_CFGR_HPRE_POS     = 4,
    RCC_CFGR_PPRE1_POS    = 10,
    RCC_CFGR_PPRE2_POS    = 13,
    RCC_CFGR_RTCPRE_POS   = 16,
    RCC_CFGR_MCO1_POS     = 21,
    RCC_CFGR_I2SSRC_POS   = 23,
    RCC_CFGR_MCO1_PRE_POS = 24,
    RCC_CFGR_MCO2_PRE_POS = 27,
    RCC_CFGR_MCO2_POS     = 30,
} rcc_cfgr_pos_e;

typedef enum {
    RCC_CFGR_SW_MASK       = 0b11,
    RCC_CFGR_SWS_MASK      = 0b11,
    RCC_CFGR_HPRE_MASK     = 0b1111, // 4 bits
    RCC_CFGR_PPRE1_MASK    = 0b111,
    RCC_CFGR_PPRE2_MASK    = 0b111,
    RCC_CFGR_RTCPRE_MASK   = 0b11111, // 5 bits
    RCC_CFGR_MCO1_MASK     = 0b111,
    RCC_CFGR_I2SSRC_MASK   = 0b1,
    RCC_CFGR_MCO1_PRE_MASK = 0b111,
    RCC_CFGR_MCO2_PRE_MASK = 0b111,
    RCC_CFGR_MCO2_MASK     = 0b11,
} rcc_cfgr_mask_e;

// RCC_APB1
typedef enum {
    RCC_APB1ENR_TIM2_POS   = 0,
    RCC_APB1ENR_TIM3_POS   = 1,
    RCC_APB1ENR_TIM4_POS   = 2,
    RCC_APB1ENR_TIM5_POS   = 3,
    RCC_APB1ENR_TIM6_POS   = 4,
    RCC_APB1ENR_TIM7_POS   = 5,
    RCC_APB1ENR_TIM12_POS  = 6,
    RCC_APB1ENR_TIM13_POS  = 7,
    RCC_APB1ENR_TIM14_POS  = 8,
    RCC_APB1ENR_WWDG_POS   = 11,
    RCC_APB1ENR_SPI2_POS   = 14,
    RCC_APB1ENR_SPI3_POS   = 15,
    RCC_APB1ENR_USART2_POS = 17,
    RCC_APB1ENR_USART3_POS = 18,
    RCC_APB1ENR_UART4_POS  = 19,
    RCC_APB1ENR_UART5_POS  = 20,
    RCC_APB1ENR_I2C1_POS   = 21,
    RCC_APB1ENR_I2C2_POS   = 22,
    RCC_APB1ENR_I2C3_POS   = 23,
    RCC_APB1ENR_CAN1_POS   = 25,
    RCC_APB1ENR_CAN2_POS   = 26,
    RCC_APB1ENR_PWR_POS    = 28,
    RCC_APB1ENR_DAC_POS    = 29,
} rcc_apb1enr_pos_e;

typedef enum {
    RCC_APB1ENR_TIM2   = (1 << RCC_APB1ENR_TIM2_POS),
    RCC_APB1ENR_TIM3   = (1 << RCC_APB1ENR_TIM3_POS),
    RCC_APB1ENR_TIM4   = (1 << RCC_APB1ENR_TIM4_POS),
    RCC_APB1ENR_TIM5   = (1 << RCC_APB1ENR_TIM5_POS),
    RCC_APB1ENR_TIM6   = (1 << RCC_APB1ENR_TIM6_POS),
    RCC_APB1ENR_TIM7   = (1 << RCC_APB1ENR_TIM7_POS),
    RCC_APB1ENR_TIM12  = (1 << RCC_APB1ENR_TIM12_POS),
    RCC_APB1ENR_TIM13  = (1 << RCC_APB1ENR_TIM13_POS),
    RCC_APB1ENR_TIM14  = (1 << RCC_APB1ENR_TIM14_POS),
    RCC_APB1ENR_WWDG   = (1 << RCC_APB1ENR_WWDG_POS),
    RCC_APB1ENR_SPI2   = (1 << RCC_APB1ENR_SPI2_POS),
    RCC_APB1ENR_SPI3   = (1 << RCC_APB1ENR_SPI3_POS),
    RCC_APB1ENR_USART2 = (1 << RCC_APB1ENR_USART2_POS),
    RCC_APB1ENR_USART3 = (1 << RCC_APB1ENR_USART3_POS),
    RCC_APB1ENR_UART4  = (1 << RCC_APB1ENR_UART4_POS),
    RCC_APB1ENR_UART5  = (1 << RCC_APB1ENR_UART5_POS),
    RCC_APB1ENR_I2C1   = (1 << RCC_APB1ENR_I2C1_POS),
    RCC_APB1ENR_I2C2   = (1 << RCC_APB1ENR_I2C2_POS),
    RCC_APB1ENR_I2C3   = (1 << RCC_APB1ENR_I2C3_POS),
    RCC_APB1ENR_CAN1   = (1 << RCC_APB1ENR_CAN1_POS),
    RCC_APB1ENR_CAN2   = (1 << RCC_APB1ENR_CAN2_POS),
    RCC_APB1ENR_PWR    = (1 << RCC_APB1ENR_PWR_POS),
    RCC_APB1ENR_DAC    = (1 << RCC_APB1ENR_DAC_POS),
} rcc_apb1enr_e;

// RCC_APB2
typedef enum {
    RCC_APB2ENR_TIM1_POS   = 0,
    RCC_APB2ENR_TIM8_POS   = 2,
    RCC_APB2ENR_USART1_POS = 4,
    RCC_APB2ENR_USART6_POS = 5,
    RCC_APB2ENR_ADC1_POS   = 8,
    RCC_APB2ENR_ADC2_POS   = 9,
    RCC_APB2ENR_ADC3_POS   = 10,
    RCC_APB2ENR_SDIO_POS   = 11,
    RCC_APB2ENR_SPI1_POS   = 12,
    RCC_APB2ENR_SYSCFG_POS = 14,
    RCC_APB2ENR_TIM9_POS   = 16,
    RCC_APB2ENR_TIM10_POS  = 17,
    RCC_APB2ENR_TIM11_POS  = 18,
} rcc_apb2enr_pos_e;

typedef enum {
    RCC_APB2ENR_TIM1   = (1 << RCC_APB2ENR_TIM1_POS),
    RCC_APB2ENR_TIM8   = (1 << RCC_APB2ENR_TIM8_POS),
    RCC_APB2ENR_USART1 = (1 << RCC_APB2ENR_USART1_POS),
    RCC_APB2ENR_USART6 = (1 << RCC_APB2ENR_USART6_POS),
    RCC_APB2ENR_ADC1   = (1 << RCC_APB2ENR_ADC1_POS),
    RCC_APB2ENR_ADC2   = (1 << RCC_APB2ENR_ADC2_POS),
    RCC_APB2ENR_ADC3   = (1 << RCC_APB2ENR_ADC3_POS),
    RCC_APB2ENR_SDIO   = (1 << RCC_APB2ENR_SDIO_POS),
    RCC_APB2ENR_SPI1   = (1 << RCC_APB2ENR_SPI1_POS),
    RCC_APB2ENR_SYSCFG = (1 << RCC_APB2ENR_SYSCFG_POS),
    RCC_APB2ENR_TIM9   = (1 << RCC_APB2ENR_TIM9_POS),
    RCC_APB2ENR_TIM10  = (1 << RCC_APB2ENR_TIM10_POS),
    RCC_APB2ENR_TIM11  = (1 << RCC_APB2ENR_TIM11_POS),
} rcc_apb2enr_e;

/****************************************************************************************************
 Register Structure Definitions
****************************************************************************************************/

// RCC Register Definition Structure
// clang-format off
typedef __vo struct {
	uint32_t CR;				// RCC clock control register										Offset: 0x00
	uint32_t PLLCFGR;			// RCC PLL configuration register									Offset: 0x04
	uint32_t CFGR;				// RCC clock configuration register									Offset: 0x08
	uint32_t CIR;				// RCC clock interrupt register										Offset: 0x0C
	uint32_t AHB1RSTR;			// RCC AHB1 peripheral reset register								Offset: 0x10
	uint32_t AHB2RSTR;			// RCC AHB2 peripheral reset register								Offset: 0x14
	uint32_t AHB3RSTR;			// RCC AHB3 peripheral reset register								Offset: 0x18
	uint32_t RESERVED0;			// Reserved 0x1C
	uint32_t APB1RSTR;			// RCC APB1 peripheral reset register								Offset: 0x20
	uint32_t APB2RSTR;			// RCC APB2 peripheral reset register								Offset: 0x24
	uint32_t RESERVED1;			// Reserved 0x28
	uint32_t RESERVED2;			// Reserved 0x2C
	uint32_t AHB1ENR;			// RCC AHB1 peripheral clock enable register						Offset: 0x30
	uint32_t AHB2ENR;			// RCC AHB2 peripheral clock enable register						Offset: 0x34
	uint32_t AHB3ENR;			// RCC AHB3 peripheral clock enable register						Offset: 0x38
	uint32_t RESERVED3;			// Reserved 0x3C
	uint32_t APB1ENR;			// RCC APB1 peripheral clock enable register						Offset: 0x40
	uint32_t APB2ENR;			// RCC APB2 peripheral clock enable register						Offset: 0x44
	uint32_t RESERVED4;			// Reserved 0x48
	uint32_t RESERVED5;			// Reserved 0x4C
	uint32_t AHB1LPENR;		    // RCC AHB1 peripheral clock enable in low power mode register		Offset: 0x50
	uint32_t AHB2LPENR;		    // RCC AHB2 peripheral clock enable in low power mode register		Offset: 0x54
	uint32_t AHB3LPENR;		    // RCC AHB3 peripheral clock enable in low power mode register		Offset: 0x58
	uint32_t RESERVED6;			// Reserved 0x5C
	uint32_t APB1LPENR;		    // RCC APB1 peripheral clock enable in low power mode register		Offset: 0x60
	uint32_t APB2LPENR;		    // RCC APB2 peripheral clock enabled in low power mode 				Offset: 0x64
	uint32_t RESERVED7;			// Reserved 0x68
	uint32_t RESERVED8;			// Reserved 0x6C
	uint32_t RCC_BDCR;			// RCC Backup domain control register								Offset: 0x70
	uint32_t RCC_CSR;			// RCC clock control & status register								Offset: 0x74
	uint32_t RESERVED9;			// Reserved 0x78
	uint32_t RESERVED10;		// Reserved 0x7C
	uint32_t RCC_SSCGR;		    // RCC spread spectrum clock generation register					Offset: 0x80
	uint32_t PLLI2SCFGR;		// RCC PLLI2S configuration register								Offset: 0x84

}rcc_reg_def;
// clang-format on

/****************************************************************************************************
                                    Peripheral Structure Definitions
****************************************************************************************************/

#define RCC ((rcc_reg_def *)RCC_BASE_ADDR)

/****************************************************************************************************
                                    Peripheral Function APIs
****************************************************************************************************/

uint32_t rcc_get_pll_freq_hz(void);
uint32_t rcc_get_sys_clock_freq_hz(void);
uint32_t rcc_get_bus_clock_freq_hz(bus_types bus);
void rcc_mco_config(rcc_mco_clock_src_e mco_clk_src, rcc_mco_prescaler_e mco_prescaler);

#endif
