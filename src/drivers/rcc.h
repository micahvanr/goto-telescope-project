//======================================================================================//{
//  File: rcc.h
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

#ifndef RCC_H
#define RCC_H

//} Include Statements
//=========================================//{

#include "gpio.h"
#include "i2c.h"
#include "stm32f4xx.h"
#include "tim.h"
#include "usart.h"
#include <stdbool.h>

//======================================================================================//}
//                  Public Constants
//======================================================================================//{

//} Address Definitions
//=========================================//{

typedef enum : uint32_t {
    RCC_BASE_ADDR = ((AHB1_BASE_ADDR) + (0x3800U)),
} rcc_base_addr_e;

//} Config Options (If multiple configs exist, name and separate them)
//=========================================//{

//} API Function Argument Options
//=========================================//{

typedef enum : uint8_t {
    RCC_MCO_PRE_1 = 1,
    RCC_MCO_PRE_2 = 2,
    RCC_MCO_PRE_3 = 3,
    RCC_MCO_PRE_4 = 4,
    RCC_MCO_PRE_5 = 5,
} rcc_mco_prescaler_e;

typedef enum : uint8_t {
    RCC_MCO1_HSI_SRC    = 0,
    RCC_MCO1_LSE_SRC    = 1,
    RCC_MCO1_HSE_SRC    = 2,
    RCC_MCO1_PLL_SRC    = 3,
    RCC_MCO2_SYSCLK_SRC = 4,
    RCC_MCO2_PLLI2S_SRC = 5,
    RCC_MCO2_HSE_SRC    = 6,
    RCC_MCO2_PLL_SRC    = 7,
} rcc_mco_clock_src_e;

//} Callback Options
//=========================================//{

//} Interrupt Handler Constants
//=========================================//{

//} Misc. Constants
//=========================================//{

// TODO: Change frequencies for custom board when applicable.
typedef enum : uint32_t {
    HSI_CLOCK_FREQ = 16000000,
    HSE_CLOCK_FREQ = 8000000, // Only for discovery board -> will need to be changed for custom board
    LSE_CLOCK_FREQ = 0,       // N/A for discover board
    LSI_CLOCK_FREQ = 32000,
} clock_values_e;

typedef enum : uint8_t {
    CLOCK_SRC_HSI = 0,
    CLOCK_SRC_HSE = 1,
    CLOCK_SRC_PLL = 2,
    CLOCK_SRC_LSE = 3,
    CLOCK_SRC_LSI = 4,
} clock_sources_e;

typedef enum : uint8_t {
    RCC_MCO1_SEL,
    RCC_MCO2_SEL,
} rcc_mco_sel_e;

typedef enum : uint32_t {
    RCC_RTC_CLK_SRC_NA  = 0b00,
    RCC_RTC_CLK_SRC_LSE = 0b01,
    RCC_RTC_CLK_SRC_LSI = 0b10,
    RCC_RTC_CLK_SRC_HSE = 0b11,
} rcc_rtc_clk_src_e;

typedef enum : uint32_t {
    RCC_RTC_HSE_PRE_NA = 0b00000,
    RCC_RTC_HSE_PRE_2  = 0b00010,
    RCC_RTC_HSE_PRE_3  = 0b00011,
    RCC_RTC_HSE_PRE_4  = 0b00100,
    RCC_RTC_HSE_PRE_5  = 0b00101,
    RCC_RTC_HSE_PRE_6  = 0b00110,
    RCC_RTC_HSE_PRE_7  = 0b00111,
    RCC_RTC_HSE_PRE_8  = 0b01000,
    RCC_RTC_HSE_PRE_9  = 0b01001,
    RCC_RTC_HSE_PRE_10 = 0b01010,
    RCC_RTC_HSE_PRE_11 = 0b01011,
    RCC_RTC_HSE_PRE_12 = 0b01100,
    RCC_RTC_HSE_PRE_13 = 0b01101,
    RCC_RTC_HSE_PRE_14 = 0b01110,
    RCC_RTC_HSE_PRE_15 = 0b01111,
    RCC_RTC_HSE_PRE_16 = 0b10000,
    RCC_RTC_HSE_PRE_17 = 0b10001,
    RCC_RTC_HSE_PRE_18 = 0b10010,
    RCC_RTC_HSE_PRE_19 = 0b10011,
    RCC_RTC_HSE_PRE_20 = 0b10100,
    RCC_RTC_HSE_PRE_21 = 0b10101,
    RCC_RTC_HSE_PRE_22 = 0b10110,
    RCC_RTC_HSE_PRE_23 = 0b10111,
    RCC_RTC_HSE_PRE_24 = 0b11000,
    RCC_RTC_HSE_PRE_25 = 0b11001,
    RCC_RTC_HSE_PRE_26 = 0b11010,
    RCC_RTC_HSE_PRE_27 = 0b11011,
    RCC_RTC_HSE_PRE_28 = 0b11100,
    RCC_RTC_HSE_PRE_29 = 0b11101,
    RCC_RTC_HSE_PRE_30 = 0b11110,
    RCC_RTC_HSE_PRE_31 = 0b11111,
} rcc_rtc_hse_pre_e;

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

// RCC Register Definition Structure
typedef volatile struct {
    uint32_t CR;          // clock control register                                        Offset: 0x0
    uint32_t PLLCFGR;     // PLL configuration register                                    Offset: 0x4
    uint32_t CFGR;        // clock configuration register                                  Offset: 0x8
    uint32_t CIR;         // clock interrupt register                                      Offset: 0xC
    uint32_t AHB1RSTR;    // AHB1 peripheral reset register                                Offset: 0x10
    uint32_t AHB2RSTR;    // AHB2 peripheral reset register                                Offset: 0x14
    uint32_t AHB3RSTR;    // AHB3 peripheral reset register                                Offset: 0x18
    uint32_t reserved_1;  // Reserved 0x1C
    uint32_t APB1RSTR;    // APB1 peripheral reset register                                Offset: 0x20
    uint32_t APB2RSTR;    // APB2 peripheral reset register                                Offset: 0x24
    uint32_t reserved_2;  // Reserved 0x28
    uint32_t reserved_3;  // Reserved 0x2C
    uint32_t AHB1ENR;     // AHB1 peripheral clock register                                Offset: 0x30
    uint32_t AHB2ENR;     // AHB2 peripheral clock enable register                         Offset: 0x34
    uint32_t AHB3ENR;     // AHB3 peripheral clock enable register                         Offset: 0x38
    uint32_t reserved_4;  // Reserved 0x3C
    uint32_t APB1ENR;     // APB1 peripheral clock enable register                         Offset: 0x40
    uint32_t APB2ENR;     // APB2 peripheral clock enable register                         Offset: 0x44
    uint32_t reserved_5;  // Reserved 0x48
    uint32_t reserved_6;  // Reserved 0x4C
    uint32_t AHB1LPENR;   // AHB1 peripheral clock enable in low power mode register       Offset: 0x50
    uint32_t AHB2LPENR;   // AHB2 peripheral clock enable in low power mode register       Offset: 0x54
    uint32_t AHB3LPENR;   // AHB3 peripheral clock enable in low power mode register       Offset: 0x58
    uint32_t reserved_7;  // Reserved 0x5C
    uint32_t APB1LPENR;   // APB1 peripheral clock enable in low power mode register       Offset: 0x60
    uint32_t APB2LPENR;   // APB2 peripheral clock enabled in low power mode register      Offset: 0x64
    uint32_t reserved_8;  // Reserved 0x68
    uint32_t reserved_9;  // Reserved 0x6C
    uint32_t BDCR;        // Backup domain control register                                Offset: 0x70
    uint32_t CSR;         // clock control & status register                               Offset: 0x74
    uint32_t reserved_10; // Reserved 0x78
    uint32_t reserved_11; // Reserved 0x7C
    uint32_t SSCGR;       // spread spectrum clock generation register                     Offset: 0x80
    uint32_t PLLI2SCFGR;  // PLLI2S configuration register                                 Offset: 0x84
} rcc_reg_def;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define RCC ((rcc_reg_def *) RCC_BASE_ADDR)

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

void rcc_lsi_config(togglable_e toggle);
bool rcc_lsi_is_rdy(void);

void rcc_gpio_togglable_clock(gpio_reg_def const *const p_gpiox, togglable_e toggle);
void rcc_i2c_togglable_clock(i2c_reg_def const *const p_i2cx, togglable_e toggle);
void rcc_pwr_togglable_clock(togglable_e toggle);
void rcc_rtc_togglable_clock(togglable_e toggle);
void rcc_syscfg_togglable_clock(togglable_e toggle);
void rcc_tim_togglable_clock(tim_reg_def const *const p_timx, togglable_e toggle);
void rcc_usart_togglable_clock(usart_reg_def const *const p_usartx, togglable_e toggle);

uint32_t rcc_get_pll_freq_hz(void);
uint32_t rcc_get_sys_clock_freq_hz(void);
uint32_t rcc_get_bus_clock_freq_hz(bus_types bus);
uint32_t rcc_get_timer_clock_freq_hz(bus_types bus);

void rcc_set_rtc_clk_src(rcc_rtc_clk_src_e rtc_clk_sel);
void rcc_set_rtc_pre(rcc_rtc_hse_pre_e hse_pre);
uint32_t rcc_get_rtc_clock_freq_hz(void);

void rcc_config_mco(rcc_mco_clock_src_e mco_clk_src, rcc_mco_prescaler_e mco_prescaler);

#endif // RCC_H

// *End of file*
