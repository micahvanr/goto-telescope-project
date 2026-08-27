#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx.h"

//======================================================================================//
//                  Address Definitions
//======================================================================================//

typedef enum {
    GPIOA_BASE_ADDR = ((AHB1_BASE_ADDR) + (0x0000u)),
    GPIOB_BASE_ADDR = ((AHB1_BASE_ADDR) + (0x0400u)),
    GPIOC_BASE_ADDR = ((AHB1_BASE_ADDR) + (0x0800u)),
    GPIOD_BASE_ADDR = ((AHB1_BASE_ADDR) + (0x0C00u)),
    GPIOE_BASE_ADDR = ((AHB1_BASE_ADDR) + (0x1000u)),
    GPIOF_BASE_ADDR = ((AHB1_BASE_ADDR) + (0x1400u)),
    GPIOG_BASE_ADDR = ((AHB1_BASE_ADDR) + (0x1800u)),
    GPIOH_BASE_ADDR = ((AHB1_BASE_ADDR) + (0x1C00u)),
} gpio_base_addr_e;

//======================================================================================//
//                  Peripheral Constants
//======================================================================================//

typedef enum {
    GPIO_NOT_INITIALIZED = 0,
    GPIO_INITIALIZED     = 1,
} gpio_init_check_e;

//  GPIO Handle Structure Possible Values
//=========================================//

typedef enum {
    GPIO_MODE_INPUT  = 0b00,
    GPIO_MODE_OUTPUT = 0b01,
    GPIO_MODE_ALT_FN = 0b10,
    GPIO_MODE_ANALOG = 0b11,
} gpio_modes_e;

typedef enum {
    GPIO_IT_NA  = 0,
    GPIO_IT_RT  = 1,
    GPIO_IT_FT  = 2,
    GPIO_IT_RFT = 3,
} gpio_it_trigger_e;

typedef enum {
    GPIO_ALT_FN_0  = 0b0000,
    GPIO_ALT_FN_1  = 0b0001,
    GPIO_ALT_FN_2  = 0b0010,
    GPIO_ALT_FN_3  = 0b0011,
    GPIO_ALT_FN_4  = 0b0100,
    GPIO_ALT_FN_5  = 0b0101,
    GPIO_ALT_FN_6  = 0b0110,
    GPIO_ALT_FN_7  = 0b0111,
    GPIO_ALT_FN_8  = 0b1000,
    GPIO_ALT_FN_9  = 0b1001,
    GPIO_ALT_FN_10 = 0b1010,
    GPIO_ALT_FN_11 = 0b1011,
    GPIO_ALT_FN_12 = 0b1100,
    GPIO_ALT_FN_13 = 0b1101,
    GPIO_ALT_FN_14 = 0b1110,
    GPIO_ALT_FN_15 = 0b1111,
    GPIO_ALT_FN_NA,
} gpio_alt_fn_e;

typedef enum {
    GPIO_OPTYPE_PUSH_PULL  = 0b0,
    GPIO_OPTYPE_OPEN_DRAIN = 0b1,
} gpio_output_type_e;

typedef enum {
    GPIO_OSPEED_LOW       = 0b00,
    GPIO_OSPEED_MEDIUM    = 0b01,
    GPIO_OSPEED_FAST      = 0b10,
    GPIO_OSPEED_VERY_FAST = 0b11,
} gpio_output_speed_e;

typedef enum {
    GPIO_PUPD_NO   = 0b00,
    GPIO_PULL_UP   = 0b01,
    GPIO_PULL_DOWN = 0b10,
} gpio_pullup_pulldown_e;

typedef enum {
    EXTI_LINE_NO_0  = 0,
    EXTI_LINE_NO_1  = 1,
    EXTI_LINE_NO_2  = 2,
    EXTI_LINE_NO_3  = 3,
    EXTI_LINE_NO_4  = 4,
    EXTI_LINE_NO_5  = 5,
    EXTI_LINE_NO_6  = 6,
    EXTI_LINE_NO_7  = 7,
    EXTI_LINE_NO_8  = 8,
    EXTI_LINE_NO_9  = 9,
    EXTI_LINE_NO_10 = 10,
    EXTI_LINE_NO_11 = 11,
    EXTI_LINE_NO_12 = 12,
    EXTI_LINE_NO_13 = 13,
    EXTI_LINE_NO_14 = 14,
    EXTI_LINE_NO_15 = 15,
    EXTI_LINE_NO_16 = 16,
    EXTI_LINE_NO_17 = 17,
    EXTI_LINE_NO_18 = 18,
    EXTI_LINE_NO_19 = 19,
    EXTI_LINE_NO_20 = 20,
    EXTI_LINE_NO_21 = 21,
    EXTI_LINE_NO_22 = 22,
} exti_lines_e;

//======================================================================================//
//                  Register Constants
//======================================================================================//

typedef enum {
    GPIO_MODER_MODER15_POS = 30, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER14_POS = 28, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER13_POS = 26, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER12_POS = 24, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER11_POS = 22, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER10_POS = 20, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER9_POS  = 18, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER8_POS  = 16, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER7_POS  = 14, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER6_POS  = 12, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER5_POS  = 10, // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER4_POS  = 8,  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER3_POS  = 6,  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER2_POS  = 4,  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER1_POS  = 2,  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER0_POS  = 0,  // Port x configuration bits (y = 0..15)
} gpio_moder_pos_e;

typedef enum {
    GPIO_MODER_MODER15 = (1 << GPIO_MODER_MODER15_POS), // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER14 = (1 << GPIO_MODER_MODER14_POS), // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER13 = (1 << GPIO_MODER_MODER13_POS), // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER12 = (1 << GPIO_MODER_MODER12_POS), // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER11 = (1 << GPIO_MODER_MODER11_POS), // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER10 = (1 << GPIO_MODER_MODER10_POS), // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER9  = (1 << GPIO_MODER_MODER9_POS),  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER8  = (1 << GPIO_MODER_MODER8_POS),  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER7  = (1 << GPIO_MODER_MODER7_POS),  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER6  = (1 << GPIO_MODER_MODER6_POS),  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER5  = (1 << GPIO_MODER_MODER5_POS),  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER4  = (1 << GPIO_MODER_MODER4_POS),  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER3  = (1 << GPIO_MODER_MODER3_POS),  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER2  = (1 << GPIO_MODER_MODER2_POS),  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER1  = (1 << GPIO_MODER_MODER1_POS),  // Port x configuration bits (y = 0..15)
    GPIO_MODER_MODER0  = (1 << GPIO_MODER_MODER0_POS),  // Port x configuration bits (y = 0..15)
} gpio_moder_e;

typedef enum {
    GPIO_MODER_MODER15_MASK = 0b11, // 2 bit(s)
    GPIO_MODER_MODER14_MASK = 0b11, // 2 bit(s)
    GPIO_MODER_MODER13_MASK = 0b11, // 2 bit(s)
    GPIO_MODER_MODER12_MASK = 0b11, // 2 bit(s)
    GPIO_MODER_MODER11_MASK = 0b11, // 2 bit(s)
    GPIO_MODER_MODER10_MASK = 0b11, // 2 bit(s)
    GPIO_MODER_MODER9_MASK  = 0b11, // 2 bit(s)
    GPIO_MODER_MODER8_MASK  = 0b11, // 2 bit(s)
    GPIO_MODER_MODER7_MASK  = 0b11, // 2 bit(s)
    GPIO_MODER_MODER6_MASK  = 0b11, // 2 bit(s)
    GPIO_MODER_MODER5_MASK  = 0b11, // 2 bit(s)
    GPIO_MODER_MODER4_MASK  = 0b11, // 2 bit(s)
    GPIO_MODER_MODER3_MASK  = 0b11, // 2 bit(s)
    GPIO_MODER_MODER2_MASK  = 0b11, // 2 bit(s)
    GPIO_MODER_MODER1_MASK  = 0b11, // 2 bit(s)
    GPIO_MODER_MODER0_MASK  = 0b11, // 2 bit(s)
} gpio_moder_mask_e;

typedef enum {
    GPIO_OTYPER_OT15_POS = 15, // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT14_POS = 14, // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT13_POS = 13, // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT12_POS = 12, // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT11_POS = 11, // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT10_POS = 10, // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT9_POS  = 9,  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT8_POS  = 8,  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT7_POS  = 7,  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT6_POS  = 6,  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT5_POS  = 5,  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT4_POS  = 4,  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT3_POS  = 3,  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT2_POS  = 2,  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT1_POS  = 1,  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT0_POS  = 0,  // Port x configuration bits (y = 0..15)
} gpio_otyper_pos_e;

typedef enum {
    GPIO_OTYPER_OT15 = (1 << GPIO_OTYPER_OT15_POS), // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT14 = (1 << GPIO_OTYPER_OT14_POS), // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT13 = (1 << GPIO_OTYPER_OT13_POS), // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT12 = (1 << GPIO_OTYPER_OT12_POS), // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT11 = (1 << GPIO_OTYPER_OT11_POS), // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT10 = (1 << GPIO_OTYPER_OT10_POS), // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT9  = (1 << GPIO_OTYPER_OT9_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT8  = (1 << GPIO_OTYPER_OT8_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT7  = (1 << GPIO_OTYPER_OT7_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT6  = (1 << GPIO_OTYPER_OT6_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT5  = (1 << GPIO_OTYPER_OT5_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT4  = (1 << GPIO_OTYPER_OT4_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT3  = (1 << GPIO_OTYPER_OT3_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT2  = (1 << GPIO_OTYPER_OT2_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT1  = (1 << GPIO_OTYPER_OT1_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OTYPER_OT0  = (1 << GPIO_OTYPER_OT0_POS),  // Port x configuration bits (y = 0..15)
} gpio_otyper_e;

typedef enum {
    GPIO_OTYPER_OT15_MASK = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT14_MASK = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT13_MASK = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT12_MASK = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT11_MASK = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT10_MASK = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT9_MASK  = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT8_MASK  = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT7_MASK  = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT6_MASK  = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT5_MASK  = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT4_MASK  = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT3_MASK  = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT2_MASK  = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT1_MASK  = 0b1, // 1 bit(s)
    GPIO_OTYPER_OT0_MASK  = 0b1, // 1 bit(s)
} gpio_otyper_mask_e;

typedef enum {
    GPIO_OSPEEDR_OSPEEDR15_POS = 30, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR14_POS = 28, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR13_POS = 26, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR12_POS = 24, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR11_POS = 22, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR10_POS = 20, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR9_POS  = 18, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR8_POS  = 16, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR7_POS  = 14, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR6_POS  = 12, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR5_POS  = 10, // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR4_POS  = 8,  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR3_POS  = 6,  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR2_POS  = 4,  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR1_POS  = 2,  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR0_POS  = 0,  // Port x configuration bits (y = 0..15)
} gpio_ospeedr_pos_e;

typedef enum {
    GPIO_OSPEEDR_OSPEEDR15 = (1 << GPIO_OSPEEDR_OSPEEDR15_POS), // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR14 = (1 << GPIO_OSPEEDR_OSPEEDR14_POS), // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR13 = (1 << GPIO_OSPEEDR_OSPEEDR13_POS), // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR12 = (1 << GPIO_OSPEEDR_OSPEEDR12_POS), // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR11 = (1 << GPIO_OSPEEDR_OSPEEDR11_POS), // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR10 = (1 << GPIO_OSPEEDR_OSPEEDR10_POS), // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR9  = (1 << GPIO_OSPEEDR_OSPEEDR9_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR8  = (1 << GPIO_OSPEEDR_OSPEEDR8_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR7  = (1 << GPIO_OSPEEDR_OSPEEDR7_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR6  = (1 << GPIO_OSPEEDR_OSPEEDR6_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR5  = (1 << GPIO_OSPEEDR_OSPEEDR5_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR4  = (1 << GPIO_OSPEEDR_OSPEEDR4_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR3  = (1 << GPIO_OSPEEDR_OSPEEDR3_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR2  = (1 << GPIO_OSPEEDR_OSPEEDR2_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR1  = (1 << GPIO_OSPEEDR_OSPEEDR1_POS),  // Port x configuration bits (y = 0..15)
    GPIO_OSPEEDR_OSPEEDR0  = (1 << GPIO_OSPEEDR_OSPEEDR0_POS),  // Port x configuration bits (y = 0..15)
} gpio_ospeedr_e;

typedef enum {
    GPIO_OSPEEDR_OSPEEDR15_MASK = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR14_MASK = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR13_MASK = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR12_MASK = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR11_MASK = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR10_MASK = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR9_MASK  = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR8_MASK  = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR7_MASK  = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR6_MASK  = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR5_MASK  = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR4_MASK  = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR3_MASK  = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR2_MASK  = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR1_MASK  = 0b11, // 2 bit(s)
    GPIO_OSPEEDR_OSPEEDR0_MASK  = 0b11, // 2 bit(s)
} gpio_ospeedr_mask_e;

typedef enum {
    GPIO_PUPDR_PUPDR15_POS = 30, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR14_POS = 28, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR13_POS = 26, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR12_POS = 24, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR11_POS = 22, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR10_POS = 20, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR9_POS  = 18, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR8_POS  = 16, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR7_POS  = 14, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR6_POS  = 12, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR5_POS  = 10, // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR4_POS  = 8,  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR3_POS  = 6,  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR2_POS  = 4,  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR1_POS  = 2,  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR0_POS  = 0,  // Port x configuration bits (y = 0..15)
} gpio_pupdr_pos_e;

typedef enum {
    GPIO_PUPDR_PUPDR15 = (1 << GPIO_PUPDR_PUPDR15_POS), // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR14 = (1 << GPIO_PUPDR_PUPDR14_POS), // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR13 = (1 << GPIO_PUPDR_PUPDR13_POS), // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR12 = (1 << GPIO_PUPDR_PUPDR12_POS), // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR11 = (1 << GPIO_PUPDR_PUPDR11_POS), // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR10 = (1 << GPIO_PUPDR_PUPDR10_POS), // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR9  = (1 << GPIO_PUPDR_PUPDR9_POS),  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR8  = (1 << GPIO_PUPDR_PUPDR8_POS),  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR7  = (1 << GPIO_PUPDR_PUPDR7_POS),  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR6  = (1 << GPIO_PUPDR_PUPDR6_POS),  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR5  = (1 << GPIO_PUPDR_PUPDR5_POS),  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR4  = (1 << GPIO_PUPDR_PUPDR4_POS),  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR3  = (1 << GPIO_PUPDR_PUPDR3_POS),  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR2  = (1 << GPIO_PUPDR_PUPDR2_POS),  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR1  = (1 << GPIO_PUPDR_PUPDR1_POS),  // Port x configuration bits (y = 0..15)
    GPIO_PUPDR_PUPDR0  = (1 << GPIO_PUPDR_PUPDR0_POS),  // Port x configuration bits (y = 0..15)
} gpio_pupdr_e;

typedef enum {
    GPIO_PUPDR_PUPDR15_MASK = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR14_MASK = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR13_MASK = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR12_MASK = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR11_MASK = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR10_MASK = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR9_MASK  = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR8_MASK  = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR7_MASK  = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR6_MASK  = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR5_MASK  = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR4_MASK  = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR3_MASK  = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR2_MASK  = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR1_MASK  = 0b11, // 2 bit(s)
    GPIO_PUPDR_PUPDR0_MASK  = 0b11, // 2 bit(s)
} gpio_pupdr_mask_e;

typedef enum {
    GPIO_IDR_IDR15_POS = 15, // Port input data (y = 0..15)
    GPIO_IDR_IDR14_POS = 14, // Port input data (y = 0..15)
    GPIO_IDR_IDR13_POS = 13, // Port input data (y = 0..15)
    GPIO_IDR_IDR12_POS = 12, // Port input data (y = 0..15)
    GPIO_IDR_IDR11_POS = 11, // Port input data (y = 0..15)
    GPIO_IDR_IDR10_POS = 10, // Port input data (y = 0..15)
    GPIO_IDR_IDR9_POS  = 9,  // Port input data (y = 0..15)
    GPIO_IDR_IDR8_POS  = 8,  // Port input data (y = 0..15)
    GPIO_IDR_IDR7_POS  = 7,  // Port input data (y = 0..15)
    GPIO_IDR_IDR6_POS  = 6,  // Port input data (y = 0..15)
    GPIO_IDR_IDR5_POS  = 5,  // Port input data (y = 0..15)
    GPIO_IDR_IDR4_POS  = 4,  // Port input data (y = 0..15)
    GPIO_IDR_IDR3_POS  = 3,  // Port input data (y = 0..15)
    GPIO_IDR_IDR2_POS  = 2,  // Port input data (y = 0..15)
    GPIO_IDR_IDR1_POS  = 1,  // Port input data (y = 0..15)
    GPIO_IDR_IDR0_POS  = 0,  // Port input data (y = 0..15)
} gpio_idr_pos_e;

typedef enum {
    GPIO_IDR_IDR15 = (1 << GPIO_IDR_IDR15_POS), // Port input data (y = 0..15)
    GPIO_IDR_IDR14 = (1 << GPIO_IDR_IDR14_POS), // Port input data (y = 0..15)
    GPIO_IDR_IDR13 = (1 << GPIO_IDR_IDR13_POS), // Port input data (y = 0..15)
    GPIO_IDR_IDR12 = (1 << GPIO_IDR_IDR12_POS), // Port input data (y = 0..15)
    GPIO_IDR_IDR11 = (1 << GPIO_IDR_IDR11_POS), // Port input data (y = 0..15)
    GPIO_IDR_IDR10 = (1 << GPIO_IDR_IDR10_POS), // Port input data (y = 0..15)
    GPIO_IDR_IDR9  = (1 << GPIO_IDR_IDR9_POS),  // Port input data (y = 0..15)
    GPIO_IDR_IDR8  = (1 << GPIO_IDR_IDR8_POS),  // Port input data (y = 0..15)
    GPIO_IDR_IDR7  = (1 << GPIO_IDR_IDR7_POS),  // Port input data (y = 0..15)
    GPIO_IDR_IDR6  = (1 << GPIO_IDR_IDR6_POS),  // Port input data (y = 0..15)
    GPIO_IDR_IDR5  = (1 << GPIO_IDR_IDR5_POS),  // Port input data (y = 0..15)
    GPIO_IDR_IDR4  = (1 << GPIO_IDR_IDR4_POS),  // Port input data (y = 0..15)
    GPIO_IDR_IDR3  = (1 << GPIO_IDR_IDR3_POS),  // Port input data (y = 0..15)
    GPIO_IDR_IDR2  = (1 << GPIO_IDR_IDR2_POS),  // Port input data (y = 0..15)
    GPIO_IDR_IDR1  = (1 << GPIO_IDR_IDR1_POS),  // Port input data (y = 0..15)
    GPIO_IDR_IDR0  = (1 << GPIO_IDR_IDR0_POS),  // Port input data (y = 0..15)
} gpio_idr_e;

typedef enum {
    GPIO_IDR_IDR15_MASK = 0b1, // 1 bit(s)
    GPIO_IDR_IDR14_MASK = 0b1, // 1 bit(s)
    GPIO_IDR_IDR13_MASK = 0b1, // 1 bit(s)
    GPIO_IDR_IDR12_MASK = 0b1, // 1 bit(s)
    GPIO_IDR_IDR11_MASK = 0b1, // 1 bit(s)
    GPIO_IDR_IDR10_MASK = 0b1, // 1 bit(s)
    GPIO_IDR_IDR9_MASK  = 0b1, // 1 bit(s)
    GPIO_IDR_IDR8_MASK  = 0b1, // 1 bit(s)
    GPIO_IDR_IDR7_MASK  = 0b1, // 1 bit(s)
    GPIO_IDR_IDR6_MASK  = 0b1, // 1 bit(s)
    GPIO_IDR_IDR5_MASK  = 0b1, // 1 bit(s)
    GPIO_IDR_IDR4_MASK  = 0b1, // 1 bit(s)
    GPIO_IDR_IDR3_MASK  = 0b1, // 1 bit(s)
    GPIO_IDR_IDR2_MASK  = 0b1, // 1 bit(s)
    GPIO_IDR_IDR1_MASK  = 0b1, // 1 bit(s)
    GPIO_IDR_IDR0_MASK  = 0b1, // 1 bit(s)
} gpio_idr_mask_e;

typedef enum {
    GPIO_ODR_ODR15_POS = 15, // Port output data (y = 0..15)
    GPIO_ODR_ODR14_POS = 14, // Port output data (y = 0..15)
    GPIO_ODR_ODR13_POS = 13, // Port output data (y = 0..15)
    GPIO_ODR_ODR12_POS = 12, // Port output data (y = 0..15)
    GPIO_ODR_ODR11_POS = 11, // Port output data (y = 0..15)
    GPIO_ODR_ODR10_POS = 10, // Port output data (y = 0..15)
    GPIO_ODR_ODR9_POS  = 9,  // Port output data (y = 0..15)
    GPIO_ODR_ODR8_POS  = 8,  // Port output data (y = 0..15)
    GPIO_ODR_ODR7_POS  = 7,  // Port output data (y = 0..15)
    GPIO_ODR_ODR6_POS  = 6,  // Port output data (y = 0..15)
    GPIO_ODR_ODR5_POS  = 5,  // Port output data (y = 0..15)
    GPIO_ODR_ODR4_POS  = 4,  // Port output data (y = 0..15)
    GPIO_ODR_ODR3_POS  = 3,  // Port output data (y = 0..15)
    GPIO_ODR_ODR2_POS  = 2,  // Port output data (y = 0..15)
    GPIO_ODR_ODR1_POS  = 1,  // Port output data (y = 0..15)
    GPIO_ODR_ODR0_POS  = 0,  // Port output data (y = 0..15)
} gpio_odr_pos_e;

typedef enum {
    GPIO_ODR_ODR15 = (1 << GPIO_ODR_ODR15_POS), // Port output data (y = 0..15)
    GPIO_ODR_ODR14 = (1 << GPIO_ODR_ODR14_POS), // Port output data (y = 0..15)
    GPIO_ODR_ODR13 = (1 << GPIO_ODR_ODR13_POS), // Port output data (y = 0..15)
    GPIO_ODR_ODR12 = (1 << GPIO_ODR_ODR12_POS), // Port output data (y = 0..15)
    GPIO_ODR_ODR11 = (1 << GPIO_ODR_ODR11_POS), // Port output data (y = 0..15)
    GPIO_ODR_ODR10 = (1 << GPIO_ODR_ODR10_POS), // Port output data (y = 0..15)
    GPIO_ODR_ODR9  = (1 << GPIO_ODR_ODR9_POS),  // Port output data (y = 0..15)
    GPIO_ODR_ODR8  = (1 << GPIO_ODR_ODR8_POS),  // Port output data (y = 0..15)
    GPIO_ODR_ODR7  = (1 << GPIO_ODR_ODR7_POS),  // Port output data (y = 0..15)
    GPIO_ODR_ODR6  = (1 << GPIO_ODR_ODR6_POS),  // Port output data (y = 0..15)
    GPIO_ODR_ODR5  = (1 << GPIO_ODR_ODR5_POS),  // Port output data (y = 0..15)
    GPIO_ODR_ODR4  = (1 << GPIO_ODR_ODR4_POS),  // Port output data (y = 0..15)
    GPIO_ODR_ODR3  = (1 << GPIO_ODR_ODR3_POS),  // Port output data (y = 0..15)
    GPIO_ODR_ODR2  = (1 << GPIO_ODR_ODR2_POS),  // Port output data (y = 0..15)
    GPIO_ODR_ODR1  = (1 << GPIO_ODR_ODR1_POS),  // Port output data (y = 0..15)
    GPIO_ODR_ODR0  = (1 << GPIO_ODR_ODR0_POS),  // Port output data (y = 0..15)
} gpio_odr_e;

typedef enum {
    GPIO_ODR_ODR15_MASK = 0b1, // 1 bit(s)
    GPIO_ODR_ODR14_MASK = 0b1, // 1 bit(s)
    GPIO_ODR_ODR13_MASK = 0b1, // 1 bit(s)
    GPIO_ODR_ODR12_MASK = 0b1, // 1 bit(s)
    GPIO_ODR_ODR11_MASK = 0b1, // 1 bit(s)
    GPIO_ODR_ODR10_MASK = 0b1, // 1 bit(s)
    GPIO_ODR_ODR9_MASK  = 0b1, // 1 bit(s)
    GPIO_ODR_ODR8_MASK  = 0b1, // 1 bit(s)
    GPIO_ODR_ODR7_MASK  = 0b1, // 1 bit(s)
    GPIO_ODR_ODR6_MASK  = 0b1, // 1 bit(s)
    GPIO_ODR_ODR5_MASK  = 0b1, // 1 bit(s)
    GPIO_ODR_ODR4_MASK  = 0b1, // 1 bit(s)
    GPIO_ODR_ODR3_MASK  = 0b1, // 1 bit(s)
    GPIO_ODR_ODR2_MASK  = 0b1, // 1 bit(s)
    GPIO_ODR_ODR1_MASK  = 0b1, // 1 bit(s)
    GPIO_ODR_ODR0_MASK  = 0b1, // 1 bit(s)
} gpio_odr_mask_e;

typedef enum {
    GPIO_BSRR_BR15_POS = 31, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR14_POS = 30, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR13_POS = 29, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR12_POS = 28, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR11_POS = 27, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR10_POS = 26, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR9_POS  = 25, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR8_POS  = 24, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR7_POS  = 23, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR6_POS  = 22, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR5_POS  = 21, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR4_POS  = 20, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR3_POS  = 19, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR2_POS  = 18, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR1_POS  = 17, // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR0_POS  = 16, // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS15_POS = 15, // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS14_POS = 14, // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS13_POS = 13, // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS12_POS = 12, // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS11_POS = 11, // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS10_POS = 10, // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS9_POS  = 9,  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS8_POS  = 8,  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS7_POS  = 7,  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS6_POS  = 6,  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS5_POS  = 5,  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS4_POS  = 4,  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS3_POS  = 3,  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS2_POS  = 2,  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS1_POS  = 1,  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS0_POS  = 0,  // Port x set bit y (y= 0..15)
} gpio_bsrr_pos_e;

typedef enum {
    GPIO_BSRR_BR15 = (1 << GPIO_BSRR_BR15_POS), // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR14 = (1 << GPIO_BSRR_BR14_POS), // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR13 = (1 << GPIO_BSRR_BR13_POS), // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR12 = (1 << GPIO_BSRR_BR12_POS), // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR11 = (1 << GPIO_BSRR_BR11_POS), // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR10 = (1 << GPIO_BSRR_BR10_POS), // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR9  = (1 << GPIO_BSRR_BR9_POS),  // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR8  = (1 << GPIO_BSRR_BR8_POS),  // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR7  = (1 << GPIO_BSRR_BR7_POS),  // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR6  = (1 << GPIO_BSRR_BR6_POS),  // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR5  = (1 << GPIO_BSRR_BR5_POS),  // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR4  = (1 << GPIO_BSRR_BR4_POS),  // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR3  = (1 << GPIO_BSRR_BR3_POS),  // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR2  = (1 << GPIO_BSRR_BR2_POS),  // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR1  = (1 << GPIO_BSRR_BR1_POS),  // Port x reset bit y (y = 0..15)
    GPIO_BSRR_BR0  = (1 << GPIO_BSRR_BR0_POS),  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS15 = (1 << GPIO_BSRR_BS15_POS), // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS14 = (1 << GPIO_BSRR_BS14_POS), // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS13 = (1 << GPIO_BSRR_BS13_POS), // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS12 = (1 << GPIO_BSRR_BS12_POS), // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS11 = (1 << GPIO_BSRR_BS11_POS), // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS10 = (1 << GPIO_BSRR_BS10_POS), // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS9  = (1 << GPIO_BSRR_BS9_POS),  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS8  = (1 << GPIO_BSRR_BS8_POS),  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS7  = (1 << GPIO_BSRR_BS7_POS),  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS6  = (1 << GPIO_BSRR_BS6_POS),  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS5  = (1 << GPIO_BSRR_BS5_POS),  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS4  = (1 << GPIO_BSRR_BS4_POS),  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS3  = (1 << GPIO_BSRR_BS3_POS),  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS2  = (1 << GPIO_BSRR_BS2_POS),  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS1  = (1 << GPIO_BSRR_BS1_POS),  // Port x set bit y (y= 0..15)
    GPIO_BSRR_BS0  = (1 << GPIO_BSRR_BS0_POS),  // Port x set bit y (y= 0..15)
} gpio_bsrr_e;

typedef enum {
    GPIO_BSRR_BR15_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BR14_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BR13_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BR12_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BR11_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BR10_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BR9_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BR8_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BR7_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BR6_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BR5_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BR4_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BR3_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BR2_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BR1_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BR0_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BS15_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BS14_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BS13_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BS12_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BS11_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BS10_MASK = 0b1, // 1 bit(s)
    GPIO_BSRR_BS9_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BS8_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BS7_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BS6_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BS5_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BS4_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BS3_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BS2_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BS1_MASK  = 0b1, // 1 bit(s)
    GPIO_BSRR_BS0_MASK  = 0b1, // 1 bit(s)
} gpio_bsrr_mask_e;

typedef enum {
    GPIO_LCKR_LCKK_POS  = 16, // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK15_POS = 15, // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK14_POS = 14, // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK13_POS = 13, // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK12_POS = 12, // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK11_POS = 11, // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK10_POS = 10, // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK9_POS  = 9,  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK8_POS  = 8,  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK7_POS  = 7,  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK6_POS  = 6,  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK5_POS  = 5,  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK4_POS  = 4,  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK3_POS  = 3,  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK2_POS  = 2,  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK1_POS  = 1,  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK0_POS  = 0,  // Port x lock bit y (y= 0..15)
} gpio_lckr_pos_e;

typedef enum {
    GPIO_LCKR_LCKK  = (1 << GPIO_LCKR_LCKK_POS),  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK15 = (1 << GPIO_LCKR_LCK15_POS), // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK14 = (1 << GPIO_LCKR_LCK14_POS), // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK13 = (1 << GPIO_LCKR_LCK13_POS), // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK12 = (1 << GPIO_LCKR_LCK12_POS), // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK11 = (1 << GPIO_LCKR_LCK11_POS), // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK10 = (1 << GPIO_LCKR_LCK10_POS), // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK9  = (1 << GPIO_LCKR_LCK9_POS),  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK8  = (1 << GPIO_LCKR_LCK8_POS),  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK7  = (1 << GPIO_LCKR_LCK7_POS),  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK6  = (1 << GPIO_LCKR_LCK6_POS),  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK5  = (1 << GPIO_LCKR_LCK5_POS),  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK4  = (1 << GPIO_LCKR_LCK4_POS),  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK3  = (1 << GPIO_LCKR_LCK3_POS),  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK2  = (1 << GPIO_LCKR_LCK2_POS),  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK1  = (1 << GPIO_LCKR_LCK1_POS),  // Port x lock bit y (y= 0..15)
    GPIO_LCKR_LCK0  = (1 << GPIO_LCKR_LCK0_POS),  // Port x lock bit y (y= 0..15)
} gpio_lckr_e;

typedef enum {
    GPIO_LCKR_LCKK_MASK  = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK15_MASK = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK14_MASK = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK13_MASK = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK12_MASK = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK11_MASK = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK10_MASK = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK9_MASK  = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK8_MASK  = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK7_MASK  = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK6_MASK  = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK5_MASK  = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK4_MASK  = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK3_MASK  = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK2_MASK  = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK1_MASK  = 0b1, // 1 bit(s)
    GPIO_LCKR_LCK0_MASK  = 0b1, // 1 bit(s)
} gpio_lckr_mask_e;

typedef enum {
    GPIO_AFRL_AFRL7_POS = 28, // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL6_POS = 24, // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL5_POS = 20, // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL4_POS = 16, // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL3_POS = 12, // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL2_POS = 8,  // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL1_POS = 4,  // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL0_POS = 0,  // Alternate function selection for port x bit y (y = 0..7)
} gpio_afrl_pos_e;

typedef enum {
    GPIO_AFRL_AFRL7 = (1 << GPIO_AFRL_AFRL7_POS), // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL6 = (1 << GPIO_AFRL_AFRL6_POS), // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL5 = (1 << GPIO_AFRL_AFRL5_POS), // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL4 = (1 << GPIO_AFRL_AFRL4_POS), // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL3 = (1 << GPIO_AFRL_AFRL3_POS), // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL2 = (1 << GPIO_AFRL_AFRL2_POS), // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL1 = (1 << GPIO_AFRL_AFRL1_POS), // Alternate function selection for port x bit y (y = 0..7)
    GPIO_AFRL_AFRL0 = (1 << GPIO_AFRL_AFRL0_POS), // Alternate function selection for port x bit y (y = 0..7)
} gpio_afrl_e;

typedef enum {
    GPIO_AFRL_AFRL7_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRL_AFRL6_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRL_AFRL5_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRL_AFRL4_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRL_AFRL3_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRL_AFRL2_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRL_AFRL1_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRL_AFRL0_MASK = 0b1111, // 4 bit(s)
} gpio_afrl_mask_e;

typedef enum {
    GPIO_AFRH_AFRH15_POS = 28, // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH14_POS = 24, // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH13_POS = 20, // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH12_POS = 16, // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH11_POS = 12, // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH10_POS = 8,  // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH9_POS  = 4,  // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH8_POS  = 0,  // Alternate function selection for port x bit y (y = 8..15)
} gpio_afrh_pos_e;

typedef enum {
    GPIO_AFRH_AFRH15 = (1 << GPIO_AFRH_AFRH15_POS), // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH14 = (1 << GPIO_AFRH_AFRH14_POS), // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH13 = (1 << GPIO_AFRH_AFRH13_POS), // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH12 = (1 << GPIO_AFRH_AFRH12_POS), // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH11 = (1 << GPIO_AFRH_AFRH11_POS), // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH10 = (1 << GPIO_AFRH_AFRH10_POS), // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH9  = (1 << GPIO_AFRH_AFRH9_POS),  // Alternate function selection for port x bit y (y = 8..15)
    GPIO_AFRH_AFRH8  = (1 << GPIO_AFRH_AFRH8_POS),  // Alternate function selection for port x bit y (y = 8..15)
} gpio_afrh_e;

typedef enum {
    GPIO_AFRH_AFRH15_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRH_AFRH14_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRH_AFRH13_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRH_AFRH12_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRH_AFRH11_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRH_AFRH10_MASK = 0b1111, // 4 bit(s)
    GPIO_AFRH_AFRH9_MASK  = 0b1111, // 4 bit(s)
    GPIO_AFRH_AFRH8_MASK  = 0b1111, // 4 bit(s)
} gpio_afrh_mask_e;

//======================================================================================//
//                  Structure Definitions
//======================================================================================//

// Register definition
typedef __vo struct {
    uint32_t MODER;   // GPIO port mode register                    Offset: 0x0
    uint32_t OTYPER;  // GPIO port output type register             Offset: 0x4
    uint32_t OSPEEDR; // GPIO port output speed register            Offset: 0x8
    uint32_t PUPDR;   // GPIO port pull-up/pull-down register       Offset: 0xC
    uint32_t IDR;     // GPIO port input data register              Offset: 0x10
    uint32_t ODR;     // GPIO port output data register             Offset: 0x14
    uint32_t BSRR;    // GPIO port bit set/reset register           Offset: 0x18
    uint32_t LCKR;    // GPIO port configuration lock register      Offset: 0x1C
    uint32_t AFR[2];  // GPIO alternate function register           Offset: 0x20
} gpio_reg_def;

// Configuration definition (used to initialize and configure gpio port settings)
typedef struct {
    pin_number_e pin_no;                    // Default: PIN_NO_0
    gpio_modes_e mode;                      // Default: GPIO_MODE_INPUT
    gpio_output_type_e output_type;         // Default: GPIO_OPTYPE_PUSH_PULL
    gpio_output_speed_e output_speed;       // Default: GPIO_OSPEED_LOW
    gpio_pullup_pulldown_e pullup_pulldown; // Default: GPIO_PUPD_NO
    gpio_alt_fn_e alt_fn_no;                // Default: GPIO_ALT_FN_0
    gpio_it_trigger_e it_trigger;           // Default: GPIO_IT_NA
} gpio_config;

// Handle definition (used to configure and handle the gpio features)
typedef struct {
    gpio_reg_def *p_gpiox;
    gpio_config gpio_conf;
} gpio_handle;

//======================================================================================//
//                  Peripheral Structure Macros
//======================================================================================//

#define GPIOA ((gpio_reg_def *)GPIOA_BASE_ADDR)
#define GPIOB ((gpio_reg_def *)GPIOB_BASE_ADDR)
#define GPIOC ((gpio_reg_def *)GPIOC_BASE_ADDR)
#define GPIOD ((gpio_reg_def *)GPIOD_BASE_ADDR)
#define GPIOE ((gpio_reg_def *)GPIOE_BASE_ADDR)
#define GPIOF ((gpio_reg_def *)GPIOF_BASE_ADDR)
#define GPIOG ((gpio_reg_def *)GPIOG_BASE_ADDR)
#define GPIOH ((gpio_reg_def *)GPIOH_BASE_ADDR)

//======================================================================================//
//                  Function API prototypes
//======================================================================================//

void gpio_init(gpio_handle *const p_gpio_handle);
void gpio_reset(gpio_reg_def const *const p_gpiox);

void gpio_write(gpio_reg_def *p_gpiox, pin_number_e pin_no, pin_logic_level_e pin_level);
pin_logic_level_e gpio_read(gpio_reg_def const *p_gpiox, pin_number_e pin_no);
void gpio_toggle(gpio_reg_def *p_gpiox, pin_number_e pin_no);

void gpio_it_config(gpio_handle const *const p_gpio_handle, togglable_e toggle);
void gpio_it_handler(exti_lines_e exti_line);

#endif
