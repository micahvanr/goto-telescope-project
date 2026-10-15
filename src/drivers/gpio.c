//======================================================================================//{
//  File: gpio.c
//  Overview: Provides functions for initializing, configuring, and using the GPIO peripheral.
//  Features:
//      Configure pin according to settings
//      Set pin state
//      Read pin state
//      Toggle pin state
//      Interrupt detection
//  Notes: None
//======================================================================================//{

//} Include Statements
//=========================================//{

#include "gpio.h"
#include "assert_handler.h"
#include "exti.h"
#include "nvic.h"
#include "rcc.h"
#include "syscfg.h"

//======================================================================================//}
//                  Private Constants
//======================================================================================//{

//} Initialization Handler Constants
//=========================================//{

typedef enum : uint32_t {
    GPIO_NOT_INITIALIZED = 0,
    GPIO_INITIALIZED     = 1,
} gpio_init_check_e;

//========================================================//}
//          Register Constants
//========================================================//{

// clang-format off
typedef enum : uint32_t {
    GPIO_MODER_MODER15_POS = 30U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER14_POS = 28U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER13_POS = 26U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER12_POS = 24U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER11_POS = 22U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER10_POS = 20U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER9_POS  = 18U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER8_POS  = 16U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER7_POS  = 14U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER6_POS  = 12U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER5_POS  = 10U,   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER4_POS  = 8U,    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER3_POS  = 6U,    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER2_POS  = 4U,    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER1_POS  = 2U,    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER0_POS  = 0U,    // Port x configuration bits (y = 0..15)
} gpio_moder_pos_e;

typedef enum : uint32_t {
    GPIO_MODER_MODER15 = (1U << GPIO_MODER_MODER15_POS),   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER14 = (1U << GPIO_MODER_MODER14_POS),   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER13 = (1U << GPIO_MODER_MODER13_POS),   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER12 = (1U << GPIO_MODER_MODER12_POS),   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER11 = (1U << GPIO_MODER_MODER11_POS),   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER10 = (1U << GPIO_MODER_MODER10_POS),   // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER9  = (1U << GPIO_MODER_MODER9_POS),    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER8  = (1U << GPIO_MODER_MODER8_POS),    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER7  = (1U << GPIO_MODER_MODER7_POS),    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER6  = (1U << GPIO_MODER_MODER6_POS),    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER5  = (1U << GPIO_MODER_MODER5_POS),    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER4  = (1U << GPIO_MODER_MODER4_POS),    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER3  = (1U << GPIO_MODER_MODER3_POS),    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER2  = (1U << GPIO_MODER_MODER2_POS),    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER1  = (1U << GPIO_MODER_MODER1_POS),    // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER0  = (1U << GPIO_MODER_MODER0_POS),    // Port x configuration bits (y = 0..15)
} gpio_moder_e;

typedef enum : uint32_t {
    GPIO_MODER_MODER15_MASK = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER14_MASK = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER13_MASK = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER12_MASK = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER11_MASK = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER10_MASK = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER9_MASK  = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER8_MASK  = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER7_MASK  = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER6_MASK  = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER5_MASK  = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER4_MASK  = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER3_MASK  = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER2_MASK  = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER1_MASK  = 0b11U,   // 2 bit(s)
    GPIO_MODER_MODER0_MASK  = 0b11U,   // 2 bit(s)
} gpio_moder_mask_e;

typedef enum : uint32_t {
    GPIO_OTYPER_OT15_POS = 15U,   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT14_POS = 14U,   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT13_POS = 13U,   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT12_POS = 12U,   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT11_POS = 11U,   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT10_POS = 10U,   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT9_POS  = 9U,    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT8_POS  = 8U,    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT7_POS  = 7U,    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT6_POS  = 6U,    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT5_POS  = 5U,    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT4_POS  = 4U,    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT3_POS  = 3U,    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT2_POS  = 2U,    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT1_POS  = 1U,    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT0_POS  = 0U,    // Port x configuration bits (y = 0..15)
} gpio_otyper_pos_e;

typedef enum : uint32_t {
    GPIO_OTYPER_OT15 = (1U << GPIO_OTYPER_OT15_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT14 = (1U << GPIO_OTYPER_OT14_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT13 = (1U << GPIO_OTYPER_OT13_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT12 = (1U << GPIO_OTYPER_OT12_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT11 = (1U << GPIO_OTYPER_OT11_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT10 = (1U << GPIO_OTYPER_OT10_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT9  = (1U << GPIO_OTYPER_OT9_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT8  = (1U << GPIO_OTYPER_OT8_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT7  = (1U << GPIO_OTYPER_OT7_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT6  = (1U << GPIO_OTYPER_OT6_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT5  = (1U << GPIO_OTYPER_OT5_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT4  = (1U << GPIO_OTYPER_OT4_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT3  = (1U << GPIO_OTYPER_OT3_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT2  = (1U << GPIO_OTYPER_OT2_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT1  = (1U << GPIO_OTYPER_OT1_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT0  = (1U << GPIO_OTYPER_OT0_POS),    // Port x configuration bits (y = 0..15)
} gpio_otyper_e;

typedef enum : uint32_t {
    GPIO_OTYPER_OT15_MASK = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT14_MASK = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT13_MASK = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT12_MASK = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT11_MASK = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT10_MASK = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT9_MASK  = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT8_MASK  = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT7_MASK  = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT6_MASK  = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT5_MASK  = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT4_MASK  = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT3_MASK  = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT2_MASK  = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT1_MASK  = 0b1U,   // 1 bit(s)
    GPIO_OTYPER_OT0_MASK  = 0b1U,   // 1 bit(s)
} gpio_otyper_mask_e;

typedef enum : uint32_t {
    GPIO_OSPEEDR_OSPEEDR15_POS = 30U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR14_POS = 28U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR13_POS = 26U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR12_POS = 24U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR11_POS = 22U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR10_POS = 20U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR9_POS  = 18U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR8_POS  = 16U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR7_POS  = 14U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR6_POS  = 12U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR5_POS  = 10U,   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR4_POS  = 8U,    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR3_POS  = 6U,    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR2_POS  = 4U,    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR1_POS  = 2U,    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR0_POS  = 0U,    // Port x configuration bits (y = 0..15)
} gpio_ospeedr_pos_e;

typedef enum : uint32_t {
    GPIO_OSPEEDR_OSPEEDR15 = (1U << GPIO_OSPEEDR_OSPEEDR15_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR14 = (1U << GPIO_OSPEEDR_OSPEEDR14_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR13 = (1U << GPIO_OSPEEDR_OSPEEDR13_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR12 = (1U << GPIO_OSPEEDR_OSPEEDR12_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR11 = (1U << GPIO_OSPEEDR_OSPEEDR11_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR10 = (1U << GPIO_OSPEEDR_OSPEEDR10_POS),   // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR9  = (1U << GPIO_OSPEEDR_OSPEEDR9_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR8  = (1U << GPIO_OSPEEDR_OSPEEDR8_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR7  = (1U << GPIO_OSPEEDR_OSPEEDR7_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR6  = (1U << GPIO_OSPEEDR_OSPEEDR6_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR5  = (1U << GPIO_OSPEEDR_OSPEEDR5_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR4  = (1U << GPIO_OSPEEDR_OSPEEDR4_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR3  = (1U << GPIO_OSPEEDR_OSPEEDR3_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR2  = (1U << GPIO_OSPEEDR_OSPEEDR2_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR1  = (1U << GPIO_OSPEEDR_OSPEEDR1_POS),    // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR0  = (1U << GPIO_OSPEEDR_OSPEEDR0_POS),    // Port x configuration bits (y = 0..15)
} gpio_ospeedr_e;

typedef enum : uint32_t {
    GPIO_OSPEEDR_OSPEEDR15_MASK = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR14_MASK = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR13_MASK = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR12_MASK = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR11_MASK = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR10_MASK = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR9_MASK  = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR8_MASK  = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR7_MASK  = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR6_MASK  = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR5_MASK  = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR4_MASK  = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR3_MASK  = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR2_MASK  = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR1_MASK  = 0b11U,   // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR0_MASK  = 0b11U,   // 2 bit(s)
} gpio_ospeedr_mask_e;

typedef enum : uint32_t {
    GPIO_PUPDR_PUPDR15_POS = 30U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR14_POS = 28U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR13_POS = 26U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR12_POS = 24U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR11_POS = 22U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR10_POS = 20U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR9_POS  = 18U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR8_POS  = 16U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR7_POS  = 14U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR6_POS  = 12U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR5_POS  = 10U,   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR4_POS  = 8U,    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR3_POS  = 6U,    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR2_POS  = 4U,    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR1_POS  = 2U,    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR0_POS  = 0U,    // Port x configuration bits (y = 0..15)
} gpio_pupdr_pos_e;

typedef enum : uint32_t {
    GPIO_PUPDR_PUPDR15 = (1U << GPIO_PUPDR_PUPDR15_POS),   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR14 = (1U << GPIO_PUPDR_PUPDR14_POS),   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR13 = (1U << GPIO_PUPDR_PUPDR13_POS),   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR12 = (1U << GPIO_PUPDR_PUPDR12_POS),   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR11 = (1U << GPIO_PUPDR_PUPDR11_POS),   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR10 = (1U << GPIO_PUPDR_PUPDR10_POS),   // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR9  = (1U << GPIO_PUPDR_PUPDR9_POS),    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR8  = (1U << GPIO_PUPDR_PUPDR8_POS),    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR7  = (1U << GPIO_PUPDR_PUPDR7_POS),    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR6  = (1U << GPIO_PUPDR_PUPDR6_POS),    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR5  = (1U << GPIO_PUPDR_PUPDR5_POS),    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR4  = (1U << GPIO_PUPDR_PUPDR4_POS),    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR3  = (1U << GPIO_PUPDR_PUPDR3_POS),    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR2  = (1U << GPIO_PUPDR_PUPDR2_POS),    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR1  = (1U << GPIO_PUPDR_PUPDR1_POS),    // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR0  = (1U << GPIO_PUPDR_PUPDR0_POS),    // Port x configuration bits (y = 0..15)
} gpio_pupdr_e;

typedef enum : uint32_t {
    GPIO_PUPDR_PUPDR15_MASK = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR14_MASK = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR13_MASK = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR12_MASK = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR11_MASK = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR10_MASK = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR9_MASK  = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR8_MASK  = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR7_MASK  = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR6_MASK  = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR5_MASK  = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR4_MASK  = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR3_MASK  = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR2_MASK  = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR1_MASK  = 0b11U,   // 2 bit(s)
    GPIO_PUPDR_PUPDR0_MASK  = 0b11U,   // 2 bit(s)
} gpio_pupdr_mask_e;

typedef enum : uint32_t {
    GPIO_IDR_IDR15_POS = 15U,   // Port input data (y = 0..15)
    GPIO_IDR_IDR14_POS = 14U,   // Port input data (y = 0..15)
    GPIO_IDR_IDR13_POS = 13U,   // Port input data (y = 0..15)
    GPIO_IDR_IDR12_POS = 12U,   // Port input data (y = 0..15)
    GPIO_IDR_IDR11_POS = 11U,   // Port input data (y = 0..15)
    GPIO_IDR_IDR10_POS = 10U,   // Port input data (y = 0..15)
    GPIO_IDR_IDR9_POS  = 9U,    // Port input data (y = 0..15)
    GPIO_IDR_IDR8_POS  = 8U,    // Port input data (y = 0..15)
    GPIO_IDR_IDR7_POS  = 7U,    // Port input data (y = 0..15)
    GPIO_IDR_IDR6_POS  = 6U,    // Port input data (y = 0..15)
    GPIO_IDR_IDR5_POS  = 5U,    // Port input data (y = 0..15)
    GPIO_IDR_IDR4_POS  = 4U,    // Port input data (y = 0..15)
    GPIO_IDR_IDR3_POS  = 3U,    // Port input data (y = 0..15)
    GPIO_IDR_IDR2_POS  = 2U,    // Port input data (y = 0..15)
    GPIO_IDR_IDR1_POS  = 1U,    // Port input data (y = 0..15)
    GPIO_IDR_IDR0_POS  = 0U,    // Port input data (y = 0..15)
} gpio_idr_pos_e;

typedef enum : uint32_t {
    GPIO_IDR_IDR15 = (1U << GPIO_IDR_IDR15_POS),   // Port input data (y = 0..15)
    GPIO_IDR_IDR14 = (1U << GPIO_IDR_IDR14_POS),   // Port input data (y = 0..15)
    GPIO_IDR_IDR13 = (1U << GPIO_IDR_IDR13_POS),   // Port input data (y = 0..15)
    GPIO_IDR_IDR12 = (1U << GPIO_IDR_IDR12_POS),   // Port input data (y = 0..15)
    GPIO_IDR_IDR11 = (1U << GPIO_IDR_IDR11_POS),   // Port input data (y = 0..15)
    GPIO_IDR_IDR10 = (1U << GPIO_IDR_IDR10_POS),   // Port input data (y = 0..15)
    GPIO_IDR_IDR9  = (1U << GPIO_IDR_IDR9_POS),    // Port input data (y = 0..15)
    GPIO_IDR_IDR8  = (1U << GPIO_IDR_IDR8_POS),    // Port input data (y = 0..15)
    GPIO_IDR_IDR7  = (1U << GPIO_IDR_IDR7_POS),    // Port input data (y = 0..15)
    GPIO_IDR_IDR6  = (1U << GPIO_IDR_IDR6_POS),    // Port input data (y = 0..15)
    GPIO_IDR_IDR5  = (1U << GPIO_IDR_IDR5_POS),    // Port input data (y = 0..15)
    GPIO_IDR_IDR4  = (1U << GPIO_IDR_IDR4_POS),    // Port input data (y = 0..15)
    GPIO_IDR_IDR3  = (1U << GPIO_IDR_IDR3_POS),    // Port input data (y = 0..15)
    GPIO_IDR_IDR2  = (1U << GPIO_IDR_IDR2_POS),    // Port input data (y = 0..15)
    GPIO_IDR_IDR1  = (1U << GPIO_IDR_IDR1_POS),    // Port input data (y = 0..15)
    GPIO_IDR_IDR0  = (1U << GPIO_IDR_IDR0_POS),    // Port input data (y = 0..15)
} gpio_idr_e;

typedef enum : uint32_t {
    GPIO_IDR_IDR15_MASK = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR14_MASK = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR13_MASK = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR12_MASK = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR11_MASK = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR10_MASK = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR9_MASK  = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR8_MASK  = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR7_MASK  = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR6_MASK  = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR5_MASK  = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR4_MASK  = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR3_MASK  = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR2_MASK  = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR1_MASK  = 0b1U,   // 1 bit(s)
    GPIO_IDR_IDR0_MASK  = 0b1U,   // 1 bit(s)
} gpio_idr_mask_e;

typedef enum : uint32_t {
    GPIO_ODR_ODR15_POS = 15U,   // Port output data (y = 0..15)
    GPIO_ODR_ODR14_POS = 14U,   // Port output data (y = 0..15)
    GPIO_ODR_ODR13_POS = 13U,   // Port output data (y = 0..15)
    GPIO_ODR_ODR12_POS = 12U,   // Port output data (y = 0..15)
    GPIO_ODR_ODR11_POS = 11U,   // Port output data (y = 0..15)
    GPIO_ODR_ODR10_POS = 10U,   // Port output data (y = 0..15)
    GPIO_ODR_ODR9_POS  = 9U,    // Port output data (y = 0..15)
    GPIO_ODR_ODR8_POS  = 8U,    // Port output data (y = 0..15)
    GPIO_ODR_ODR7_POS  = 7U,    // Port output data (y = 0..15)
    GPIO_ODR_ODR6_POS  = 6U,    // Port output data (y = 0..15)
    GPIO_ODR_ODR5_POS  = 5U,    // Port output data (y = 0..15)
    GPIO_ODR_ODR4_POS  = 4U,    // Port output data (y = 0..15)
    GPIO_ODR_ODR3_POS  = 3U,    // Port output data (y = 0..15)
    GPIO_ODR_ODR2_POS  = 2U,    // Port output data (y = 0..15)
    GPIO_ODR_ODR1_POS  = 1U,    // Port output data (y = 0..15)
    GPIO_ODR_ODR0_POS  = 0U,    // Port output data (y = 0..15)
} gpio_odr_pos_e;

typedef enum : uint32_t {
    GPIO_ODR_ODR15 = (1U << GPIO_ODR_ODR15_POS),   // Port output data (y = 0..15)
    GPIO_ODR_ODR14 = (1U << GPIO_ODR_ODR14_POS),   // Port output data (y = 0..15)
    GPIO_ODR_ODR13 = (1U << GPIO_ODR_ODR13_POS),   // Port output data (y = 0..15)
    GPIO_ODR_ODR12 = (1U << GPIO_ODR_ODR12_POS),   // Port output data (y = 0..15)
    GPIO_ODR_ODR11 = (1U << GPIO_ODR_ODR11_POS),   // Port output data (y = 0..15)
    GPIO_ODR_ODR10 = (1U << GPIO_ODR_ODR10_POS),   // Port output data (y = 0..15)
    GPIO_ODR_ODR9  = (1U << GPIO_ODR_ODR9_POS),    // Port output data (y = 0..15)
    GPIO_ODR_ODR8  = (1U << GPIO_ODR_ODR8_POS),    // Port output data (y = 0..15)
    GPIO_ODR_ODR7  = (1U << GPIO_ODR_ODR7_POS),    // Port output data (y = 0..15)
    GPIO_ODR_ODR6  = (1U << GPIO_ODR_ODR6_POS),    // Port output data (y = 0..15)
    GPIO_ODR_ODR5  = (1U << GPIO_ODR_ODR5_POS),    // Port output data (y = 0..15)
    GPIO_ODR_ODR4  = (1U << GPIO_ODR_ODR4_POS),    // Port output data (y = 0..15)
    GPIO_ODR_ODR3  = (1U << GPIO_ODR_ODR3_POS),    // Port output data (y = 0..15)
    GPIO_ODR_ODR2  = (1U << GPIO_ODR_ODR2_POS),    // Port output data (y = 0..15)
    GPIO_ODR_ODR1  = (1U << GPIO_ODR_ODR1_POS),    // Port output data (y = 0..15)
    GPIO_ODR_ODR0  = (1U << GPIO_ODR_ODR0_POS),    // Port output data (y = 0..15)
} gpio_odr_e;

typedef enum : uint32_t {
    GPIO_ODR_ODR15_MASK = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR14_MASK = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR13_MASK = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR12_MASK = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR11_MASK = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR10_MASK = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR9_MASK  = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR8_MASK  = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR7_MASK  = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR6_MASK  = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR5_MASK  = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR4_MASK  = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR3_MASK  = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR2_MASK  = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR1_MASK  = 0b1U,   // 1 bit(s)
    GPIO_ODR_ODR0_MASK  = 0b1U,   // 1 bit(s)
} gpio_odr_mask_e;

typedef enum : uint32_t {
    GPIO_BSRR_BR15_POS = 31U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR14_POS = 30U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR13_POS = 29U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR12_POS = 28U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR11_POS = 27U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR10_POS = 26U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR9_POS  = 25U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR8_POS  = 24U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR7_POS  = 23U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR6_POS  = 22U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR5_POS  = 21U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR4_POS  = 20U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR3_POS  = 19U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR2_POS  = 18U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR1_POS  = 17U,   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR0_POS  = 16U,   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS15_POS = 15U,   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS14_POS = 14U,   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS13_POS = 13U,   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS12_POS = 12U,   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS11_POS = 11U,   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS10_POS = 10U,   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS9_POS  = 9U,    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS8_POS  = 8U,    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS7_POS  = 7U,    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS6_POS  = 6U,    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS5_POS  = 5U,    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS4_POS  = 4U,    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS3_POS  = 3U,    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS2_POS  = 2U,    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS1_POS  = 1U,    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS0_POS  = 0U,    // Port x set bit y (y= 0..15)
} gpio_bsrr_pos_e;

typedef enum : uint32_t {
    GPIO_BSRR_BR15 = (1U << GPIO_BSRR_BR15_POS),   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR14 = (1U << GPIO_BSRR_BR14_POS),   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR13 = (1U << GPIO_BSRR_BR13_POS),   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR12 = (1U << GPIO_BSRR_BR12_POS),   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR11 = (1U << GPIO_BSRR_BR11_POS),   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR10 = (1U << GPIO_BSRR_BR10_POS),   // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR9  = (1U << GPIO_BSRR_BR9_POS),    // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR8  = (1U << GPIO_BSRR_BR8_POS),    // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR7  = (1U << GPIO_BSRR_BR7_POS),    // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR6  = (1U << GPIO_BSRR_BR6_POS),    // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR5  = (1U << GPIO_BSRR_BR5_POS),    // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR4  = (1U << GPIO_BSRR_BR4_POS),    // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR3  = (1U << GPIO_BSRR_BR3_POS),    // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR2  = (1U << GPIO_BSRR_BR2_POS),    // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR1  = (1U << GPIO_BSRR_BR1_POS),    // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR0  = (1U << GPIO_BSRR_BR0_POS),    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS15 = (1U << GPIO_BSRR_BS15_POS),   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS14 = (1U << GPIO_BSRR_BS14_POS),   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS13 = (1U << GPIO_BSRR_BS13_POS),   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS12 = (1U << GPIO_BSRR_BS12_POS),   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS11 = (1U << GPIO_BSRR_BS11_POS),   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS10 = (1U << GPIO_BSRR_BS10_POS),   // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS9  = (1U << GPIO_BSRR_BS9_POS),    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS8  = (1U << GPIO_BSRR_BS8_POS),    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS7  = (1U << GPIO_BSRR_BS7_POS),    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS6  = (1U << GPIO_BSRR_BS6_POS),    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS5  = (1U << GPIO_BSRR_BS5_POS),    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS4  = (1U << GPIO_BSRR_BS4_POS),    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS3  = (1U << GPIO_BSRR_BS3_POS),    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS2  = (1U << GPIO_BSRR_BS2_POS),    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS1  = (1U << GPIO_BSRR_BS1_POS),    // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS0  = (1U << GPIO_BSRR_BS0_POS),    // Port x set bit y (y= 0..15)
} gpio_bsrr_e;

typedef enum : uint32_t {
    GPIO_BSRR_BR15_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR14_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR13_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR12_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR11_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR10_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR9_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR8_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR7_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR6_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR5_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR4_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR3_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR2_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR1_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BR0_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS15_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS14_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS13_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS12_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS11_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS10_MASK = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS9_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS8_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS7_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS6_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS5_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS4_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS3_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS2_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS1_MASK  = 0b1U,   // 1 bit(s)
    GPIO_BSRR_BS0_MASK  = 0b1U,   // 1 bit(s)
} gpio_bsrr_mask_e;

typedef enum : uint32_t {
    GPIO_LCKR_LCKK_POS  = 16U,   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK15_POS = 15U,   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK14_POS = 14U,   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK13_POS = 13U,   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK12_POS = 12U,   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK11_POS = 11U,   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK10_POS = 10U,   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK9_POS  = 9U,    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK8_POS  = 8U,    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK7_POS  = 7U,    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK6_POS  = 6U,    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK5_POS  = 5U,    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK4_POS  = 4U,    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK3_POS  = 3U,    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK2_POS  = 2U,    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK1_POS  = 1U,    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK0_POS  = 0U,    // Port x lock bit y (y= 0..15)
} gpio_lckr_pos_e;

typedef enum : uint32_t {
    GPIO_LCKR_LCKK  = (1U << GPIO_LCKR_LCKK_POS),    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK15 = (1U << GPIO_LCKR_LCK15_POS),   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK14 = (1U << GPIO_LCKR_LCK14_POS),   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK13 = (1U << GPIO_LCKR_LCK13_POS),   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK12 = (1U << GPIO_LCKR_LCK12_POS),   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK11 = (1U << GPIO_LCKR_LCK11_POS),   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK10 = (1U << GPIO_LCKR_LCK10_POS),   // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK9  = (1U << GPIO_LCKR_LCK9_POS),    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK8  = (1U << GPIO_LCKR_LCK8_POS),    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK7  = (1U << GPIO_LCKR_LCK7_POS),    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK6  = (1U << GPIO_LCKR_LCK6_POS),    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK5  = (1U << GPIO_LCKR_LCK5_POS),    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK4  = (1U << GPIO_LCKR_LCK4_POS),    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK3  = (1U << GPIO_LCKR_LCK3_POS),    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK2  = (1U << GPIO_LCKR_LCK2_POS),    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK1  = (1U << GPIO_LCKR_LCK1_POS),    // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK0  = (1U << GPIO_LCKR_LCK0_POS),    // Port x lock bit y (y= 0..15)
} gpio_lckr_e;

typedef enum : uint32_t {
    GPIO_LCKR_LCKK_MASK  = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK15_MASK = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK14_MASK = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK13_MASK = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK12_MASK = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK11_MASK = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK10_MASK = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK9_MASK  = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK8_MASK  = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK7_MASK  = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK6_MASK  = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK5_MASK  = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK4_MASK  = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK3_MASK  = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK2_MASK  = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK1_MASK  = 0b1U,   // 1 bit(s)
    GPIO_LCKR_LCK0_MASK  = 0b1U,   // 1 bit(s)
} gpio_lckr_mask_e;

typedef enum : uint32_t {
    GPIO_AFRL_AFRL7_POS = 28U,   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL6_POS = 24U,   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL5_POS = 20U,   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL4_POS = 16U,   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL3_POS = 12U,   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL2_POS = 8U,    // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL1_POS = 4U,    // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL0_POS = 0U,    // Alternate function selection for port x bit y (y = 0..7)
} gpio_afrl_pos_e;

typedef enum : uint32_t {
    GPIO_AFRL_AFRL7 = (1U << GPIO_AFRL_AFRL7_POS),   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL6 = (1U << GPIO_AFRL_AFRL6_POS),   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL5 = (1U << GPIO_AFRL_AFRL5_POS),   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL4 = (1U << GPIO_AFRL_AFRL4_POS),   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL3 = (1U << GPIO_AFRL_AFRL3_POS),   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL2 = (1U << GPIO_AFRL_AFRL2_POS),   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL1 = (1U << GPIO_AFRL_AFRL1_POS),   // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL0 = (1U << GPIO_AFRL_AFRL0_POS),   // Alternate function selection for port x bit y (y = 0..7)
} gpio_afrl_e;

typedef enum : uint32_t {
    GPIO_AFRL_AFRL7_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRL_AFRL6_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRL_AFRL5_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRL_AFRL4_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRL_AFRL3_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRL_AFRL2_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRL_AFRL1_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRL_AFRL0_MASK = 0b1111U,   // 4 bit(s)
} gpio_afrl_mask_e;

typedef enum : uint32_t {
    GPIO_AFRH_AFRH15_POS = 28U,   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH14_POS = 24U,   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH13_POS = 20U,   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH12_POS = 16U,   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH11_POS = 12U,   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH10_POS = 8U,    // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH9_POS  = 4U,    // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH8_POS  = 0U,    // Alternate function selection for port x bit y (y = 8..15)
} gpio_afrh_pos_e;

typedef enum : uint32_t {
    GPIO_AFRH_AFRH15 = (1U << GPIO_AFRH_AFRH15_POS),   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH14 = (1U << GPIO_AFRH_AFRH14_POS),   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH13 = (1U << GPIO_AFRH_AFRH13_POS),   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH12 = (1U << GPIO_AFRH_AFRH12_POS),   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH11 = (1U << GPIO_AFRH_AFRH11_POS),   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH10 = (1U << GPIO_AFRH_AFRH10_POS),   // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH9  = (1U << GPIO_AFRH_AFRH9_POS),    // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH8  = (1U << GPIO_AFRH_AFRH8_POS),    // Alternate function selection for port x bit y (y = 8..15)
} gpio_afrh_e;

typedef enum : uint32_t {
    GPIO_AFRH_AFRH15_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRH_AFRH14_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRH_AFRH13_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRH_AFRH12_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRH_AFRH11_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRH_AFRH10_MASK = 0b1111U,   // 4 bit(s)
    GPIO_AFRH_AFRH9_MASK  = 0b1111U,   // 4 bit(s)
    GPIO_AFRH_AFRH8_MASK  = 0b1111U,   // 4 bit(s)
} gpio_afrh_mask_e;

// clang-format on

//======================================================================================//}
//                  Global Variables
//======================================================================================//{

// This variable is used to check/set a pins initialzation status.
// It is an array with a length of 8 and each element is 16 bits wide.
// Each element in the array represents a port and each bit inside the variable represents a pin.
// If a bit in a specific element is 0, that means it has not been initialized. If it is a 1, it has been.
volatile uint16_t g_gpio_pin_init[8] = {0};

//======================================================================================//}
//                  Private Function Prototypes
//======================================================================================//{

// Assert helper functions
static void assert_gpio_handle_param(gpio_handle const *const p_gpio_handle);
static inline void set_gpio_init_status(gpio_reg_def const *const p_gpiox, pin_number_e pin_num);
static inline gpio_init_check_e get_gpio_init_status(gpio_reg_def const *const p_gpiox, pin_number_e pin_num);

// General helper functions
static inline uint8_t map_gpio_ports_to_num(gpio_reg_def const *const p_gpiox);
static inline uint8_t map_exti_to_irq_num(exti_lines_e line_num);

//======================================================================================//}
//                  Public Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: gpio_init
//  Overview: Initialize the specified GPIO port according to settings in handle.
//  Parameters:
//      p_gpio_handle: Structure with the corresponding settings to configure the GPIO pin.
//  Return:
//      None
//  Note: The structure must be initialized with 0 before use to get the default value.
//      Otherwise be sure to set every member element to a value.
//============================================================================//}
void gpio_init(gpio_handle *const p_gpio_handle)
{
    pin_number_e pin_num = p_gpio_handle->gpio_conf.pin_num;

    // Ensure handle structure values are valid.
    assert_gpio_handle_param(p_gpio_handle);

    // Initialize gpio pin init structure.
    set_gpio_init_status(p_gpio_handle->p_gpiox, pin_num);

    // Enable RCC clock for peripheral port.
    rcc_gpio_togglable_clock(p_gpio_handle->p_gpiox, ENABLE);

    // Set alternate function mode
    // NOTE: Done before rest of config otherwise it may trigger undesired pin bouncing.
    if (p_gpio_handle->gpio_conf.alt_fn_num != GPIO_ALT_FN_NA)
    {
        uint8_t afr_reg;
        afr_reg = pin_num / 8;
        p_gpio_handle->p_gpiox->AFR[afr_reg] &= ~(GPIO_AFRL_AFRL0_MASK << (4 * (pin_num % 8)));
        p_gpio_handle->p_gpiox->AFR[afr_reg] |= (p_gpio_handle->gpio_conf.alt_fn_num << (4 * (pin_num % 8)));
    }

    // Set gpio mode
    p_gpio_handle->p_gpiox->MODER &= ~(GPIO_MODER_MODER0_MASK << (2 * pin_num));
    p_gpio_handle->p_gpiox->MODER |= (p_gpio_handle->gpio_conf.mode << (2 * pin_num));

    // Set output type
    p_gpio_handle->p_gpiox->OTYPER &= ~(GPIO_OTYPER_OT0_MASK << pin_num);
    p_gpio_handle->p_gpiox->OTYPER |= (p_gpio_handle->gpio_conf.output_type << pin_num);

    // Set output speed
    p_gpio_handle->p_gpiox->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR0_MASK << (2 * pin_num));
    p_gpio_handle->p_gpiox->OSPEEDR |= (p_gpio_handle->gpio_conf.output_speed << (2 * pin_num));

    // Set pull up or pull down resistors
    p_gpio_handle->p_gpiox->PUPDR &= ~(GPIO_PUPDR_PUPDR0_MASK << (2 * pin_num));
    p_gpio_handle->p_gpiox->PUPDR |= (p_gpio_handle->gpio_conf.pullup_pulldown << (2 * pin_num));

    // Set interrupt mode
    if (p_gpio_handle->gpio_conf.it_trigger != GPIO_IT_NA)
    {
        gpio_config_it(p_gpio_handle, ENABLE);
    }
}

//============================================================================//{
//  Function: gpio_reset
//  Overview: Reset the specified GPIO port.
//  Parameters:
//      p_gpiox: GPIO register structure macro.
//          GPIOx (A-H)
//  Return:
//      None
//  Note: None
//============================================================================//}
void gpio_reset(gpio_reg_def const *const p_gpiox)
{
    RCC->AHB1ENR |= (1 << map_gpio_ports_to_num(p_gpiox));
}

//============================================================================//{
//  Function: gpio_config_it
//  Overview: Enable or disable interrupts for the specified GPIO port according to settings inside handle.
//  Parameters:
//      p_gpio_handle: Structure with the corresponding settings to configure the GPIO pin.
//      toggle:
//          ENABLE
//          DISABLE
//  Return:
//      None
//  Note: None
//============================================================================//}
void gpio_config_it(gpio_handle const *const p_gpio_handle, togglable_e const toggle)
{
    exti_lines_e exti_line_num = (exti_lines_e) p_gpio_handle->gpio_conf.pin_num;

    syscfg_config_exti(map_gpio_ports_to_num(p_gpio_handle->p_gpiox), p_gpio_handle->gpio_conf.pin_num, toggle);

    rcc_syscfg_togglable_clock(ENABLE);

    syscfg_config_exti(map_gpio_ports_to_num(p_gpio_handle->p_gpiox), p_gpio_handle->gpio_conf.pin_num, toggle);

    exti_config_it(exti_line_num, toggle);

    switch (p_gpio_handle->gpio_conf.it_trigger)
    {
        case GPIO_IT_RT:
            exti_config_rising_edge(exti_line_num, toggle);
            break;
        case GPIO_IT_FT:
            exti_config_falling_edge(exti_line_num, toggle);
            break;
        case GPIO_IT_RFT:
            exti_config_falling_edge(exti_line_num, toggle);
            exti_config_rising_edge(exti_line_num, toggle);
            break;
        case GPIO_IT_NA:
            break;
    }

    nvic_irq_config(map_exti_to_irq_num(exti_line_num), ENABLE);
}

//============================================================================//{
//  Function: gpio_it_handler
//  Overview: Handle the specified EXTI line's interrupts.
//  Parameters:
//      exti_line: EXTI line that needs to be cleared.
//          EXTI_LINE_NUM_x (0-22)
//  Return:
//      None
//  Note: This function should be called by the applications IT handler.
//============================================================================//}
void gpio_it_handler(exti_lines_e const exti_line)
{
    exti_clear_pending(exti_line);
}

//============================================================================//{
//  Function: gpio_write
//  Overview: Set the specified pin high or low.
//  Parameters:
//      p_gpiox: GPIO register structure macro.
//          GPIOx (A-H)
//      pin_num:
//          PIN_NUM_x (0-15)
//      pin_level:
//          HIGH
//          LOW
//  Return:
//      None
//  Note: None
//============================================================================//}
void gpio_write(gpio_reg_def *const p_gpiox, pin_number_e const pin_num, pin_logic_level_e const pin_level)
{
    ASSERT(get_gpio_init_status(p_gpiox, pin_num) == GPIO_INITIALIZED);
    switch (pin_level)
    {
        case HIGH:
            p_gpiox->ODR |= (1 << pin_num);
            break;
        case LOW:
            p_gpiox->ODR &= ~(1 << pin_num);
            break;
    }
}

//============================================================================//{
//  Function: gpio_read
//  Overview: Reads a pin on a given port and returns high or low.
//  Parameters:
//      p_gpiox: GPIO register structure macro.
//          GPIOx (A-H)
//      pin_num:
//          PIN_NUM_x (0-15)
//  Return:
//      pin_logic_level_e:
//          HIGH
//          LOW
//  Note: None
//============================================================================//}
pin_logic_level_e gpio_read(gpio_reg_def const *const p_gpiox, pin_number_e const pin_num)
{
    ASSERT(get_gpio_init_status(p_gpiox, pin_num) == GPIO_INITIALIZED);
    return (pin_logic_level_e) (p_gpiox->IDR & (1 << pin_num));
}

//============================================================================//{
//  Function: gpio_toggle
//  Overview: Toggle the specified pin.
//  Parameters:
//      p_gpiox: GPIO register structure macro.
//          GPIOx (A-H)
//      pin_num:
//          PIN_NUM_x (0-15)
//  Return:
//      None
//  Note: None
//============================================================================//}
void gpio_toggle(gpio_reg_def *const p_gpiox, pin_number_e const pin_num)
{
    ASSERT(get_gpio_init_status(p_gpiox, pin_num) == GPIO_INITIALIZED);
    p_gpiox->ODR ^= (1 << pin_num);
}

//======================================================================================//}
//                  Private Function Implementation
//======================================================================================//{

static void assert_gpio_handle_param(gpio_handle const *const p_gpio_handle)
{
    // clang-format off
    uint8_t found_setting = false;

    // Peripheral check
    found_setting = (p_gpio_handle->p_gpiox == GPIOA) ? true
                  : (p_gpio_handle->p_gpiox == GPIOB) ? true
                  : (p_gpio_handle->p_gpiox == GPIOC) ? true
                  : (p_gpio_handle->p_gpiox == GPIOD) ? true
                  : (p_gpio_handle->p_gpiox == GPIOE) ? true
                  : (p_gpio_handle->p_gpiox == GPIOF) ? true
                  : (p_gpio_handle->p_gpiox == GPIOG) ? true
                  : (p_gpio_handle->p_gpiox == GPIOH) ? true
                                                      : false;

    // Alternate function check
    switch (p_gpio_handle->gpio_conf.alt_fn_num) {
    case GPIO_ALT_FN_0:  found_setting = true; break;
    case GPIO_ALT_FN_1:  found_setting = true; break;
    case GPIO_ALT_FN_2:  found_setting = true; break;
    case GPIO_ALT_FN_3:  found_setting = true; break;
    case GPIO_ALT_FN_4:  found_setting = true; break;
    case GPIO_ALT_FN_5:  found_setting = true; break;
    case GPIO_ALT_FN_6:  found_setting = true; break;
    case GPIO_ALT_FN_7:  found_setting = true; break;
    case GPIO_ALT_FN_8:  found_setting = true; break;
    case GPIO_ALT_FN_9:  found_setting = true; break;
    case GPIO_ALT_FN_10: found_setting = true; break;
    case GPIO_ALT_FN_11: found_setting = true; break;
    case GPIO_ALT_FN_12: found_setting = true; break;
    case GPIO_ALT_FN_13: found_setting = true; break;
    case GPIO_ALT_FN_14: found_setting = true; break;
    case GPIO_ALT_FN_15: found_setting = true; break;
    case GPIO_ALT_FN_NA: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Interrupt trigger check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.it_trigger) {
    case GPIO_IT_NA:  found_setting = true; break;
    case GPIO_IT_RT:  found_setting = true; break;
    case GPIO_IT_FT:  found_setting = true; break;
    case GPIO_IT_RFT: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Mode check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.mode) {
    case GPIO_MODE_INPUT:  found_setting = true; break;
    case GPIO_MODE_OUTPUT: found_setting = true; break;
    case GPIO_MODE_ALT_FN: found_setting = true; break;
    case GPIO_MODE_ANALOG: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Output speed check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.output_speed) {
    case GPIO_OSPEED_LOW:       found_setting = true; break;
    case GPIO_OSPEED_MEDIUM:    found_setting = true; break;
    case GPIO_OSPEED_FAST:      found_setting = true; break;
    case GPIO_OSPEED_VERY_FAST: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Output type check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.output_type) {
    case GPIO_OPTYPE_PUSH_PULL:  found_setting = true; break;
    case GPIO_OPTYPE_OPEN_DRAIN: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Pin number check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.pin_num) {
    case PIN_NUM_0:  found_setting = true; break;
    case PIN_NUM_1:  found_setting = true; break;
    case PIN_NUM_2:  found_setting = true; break;
    case PIN_NUM_3:  found_setting = true; break;
    case PIN_NUM_4:  found_setting = true; break;
    case PIN_NUM_5:  found_setting = true; break;
    case PIN_NUM_6:  found_setting = true; break;
    case PIN_NUM_7:  found_setting = true; break;
    case PIN_NUM_8:  found_setting = true; break;
    case PIN_NUM_9:  found_setting = true; break;
    case PIN_NUM_10: found_setting = true; break;
    case PIN_NUM_11: found_setting = true; break;
    case PIN_NUM_12: found_setting = true; break;
    case PIN_NUM_13: found_setting = true; break;
    case PIN_NUM_14: found_setting = true; break;
    case PIN_NUM_15: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Pullup/pulldown type check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.pullup_pulldown) {
    case GPIO_PUPD_NO:   found_setting = true; break;
    case GPIO_PULL_UP:   found_setting = true; break;
    case GPIO_PULL_DOWN: found_setting = true; break;
    }
    ASSERT(found_setting);
    // clang-format on
}

// Map GPIO ports to codes for setting GPIO interrupts and other functions.
static inline uint8_t map_gpio_ports_to_num(gpio_reg_def const *const p_gpiox)
{
    return (p_gpiox == GPIOA) ? 0
         : (p_gpiox == GPIOB) ? 1
         : (p_gpiox == GPIOC) ? 2
         : (p_gpiox == GPIOD) ? 3
         : (p_gpiox == GPIOE) ? 4
         : (p_gpiox == GPIOF) ? 5
         : (p_gpiox == GPIOG) ? 6
         : (p_gpiox == GPIOH) ? 7
                              : 0;
}

// Map GPIO ports to codes for setting GPIO interrupts and other functions.
static inline uint8_t map_exti_to_irq_num(exti_lines_e const line_num)
{
    return (line_num == EXTI_LINE_NUM_0)                                      ? EXTI0_IRQ_NUM_6
         : (line_num == EXTI_LINE_NUM_1)                                      ? EXTI1_IRQ_NUM_7
         : (line_num == EXTI_LINE_NUM_2)                                      ? EXTI2_IRQ_NUM_8
         : (line_num == EXTI_LINE_NUM_3)                                      ? EXTI3_IRQ_NUM_9
         : (line_num == EXTI_LINE_NUM_4)                                      ? EXTI3_IRQ_NUM_9
         : ((line_num >= EXTI_LINE_NUM_5) && (line_num <= EXTI_LINE_NUM_9))   ? EXTI9_5_IRQ_NUM_23
         : ((line_num >= EXTI_LINE_NUM_10) && (line_num <= EXTI_LINE_NUM_15)) ? EXTI15_10_IRQ_NUM_40
                                                                              : 0;
}

static inline void set_gpio_init_status(gpio_reg_def const *const p_gpiox, pin_number_e const pin_num)
{
    g_gpio_pin_init[map_gpio_ports_to_num(p_gpiox)] |= (1 << pin_num);
}

static inline gpio_init_check_e get_gpio_init_status(gpio_reg_def const *const p_gpiox, pin_number_e const pin_num)
{
    // Get port initialization pins from list.
    uint16_t port_init = g_gpio_pin_init[map_gpio_ports_to_num(p_gpiox)];
    // Mask the port to get the bit for the pin number.
    uint8_t pin_initialized = ((port_init & (1 << pin_num)) >> pin_num);

    return pin_initialized;
}

// *End of file*
