#ifndef EXTI_H
#define EXTI_H

#include "common.h"
#include "stm32f4xx.h"

//======================================================================================//}
//                  Address Definitions
//======================================================================================//{

typedef enum {
    EXTI_BASE_ADDR = ((APB2_BASE_ADDR) + (0x3C00U)),
} exti_base_addr_e;

//======================================================================================//}
//                  Peripheral Constants
//======================================================================================//{

//} API Function Argument/Return Options
//=========================================//{

typedef enum : uint32_t {
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

// Applications:
// - EXTI PR
typedef enum : uint32_t {
    EXTI_PEND_STATUS_NOT_TRIGGERED = 0b0,
    EXTI_PEND_STATUS_TRIGGERED     = 0b1,
} exti_pend_status_e;

//======================================================================================//}
//                  Register Constants
//======================================================================================//{

// clang-format off
typedef enum : uint32_t {
    EXTI_IMR_MR0_POS  = 0U,    // Interrupt Mask on line 0
    EXTI_IMR_MR1_POS  = 1U,    // Interrupt Mask on line 1
    EXTI_IMR_MR2_POS  = 2U,    // Interrupt Mask on line 2
    EXTI_IMR_MR3_POS  = 3U,    // Interrupt Mask on line 3
    EXTI_IMR_MR4_POS  = 4U,    // Interrupt Mask on line 4
    EXTI_IMR_MR5_POS  = 5U,    // Interrupt Mask on line 5
    EXTI_IMR_MR6_POS  = 6U,    // Interrupt Mask on line 6
    EXTI_IMR_MR7_POS  = 7U,    // Interrupt Mask on line 7
    EXTI_IMR_MR8_POS  = 8U,    // Interrupt Mask on line 8
    EXTI_IMR_MR9_POS  = 9U,    // Interrupt Mask on line 9
    EXTI_IMR_MR10_POS = 10U,   // Interrupt Mask on line 10
    EXTI_IMR_MR11_POS = 11U,   // Interrupt Mask on line 11
    EXTI_IMR_MR12_POS = 12U,   // Interrupt Mask on line 12
    EXTI_IMR_MR13_POS = 13U,   // Interrupt Mask on line 13
    EXTI_IMR_MR14_POS = 14U,   // Interrupt Mask on line 14
    EXTI_IMR_MR15_POS = 15U,   // Interrupt Mask on line 15
    EXTI_IMR_MR16_POS = 16U,   // Interrupt Mask on line 16
    EXTI_IMR_MR17_POS = 17U,   // Interrupt Mask on line 17
    EXTI_IMR_MR18_POS = 18U,   // Interrupt Mask on line 18
    EXTI_IMR_MR19_POS = 19U,   // Interrupt Mask on line 19
    EXTI_IMR_MR20_POS = 20U,   // Interrupt Mask on line 20
    EXTI_IMR_MR21_POS = 21U,   // Interrupt Mask on line 21
    EXTI_IMR_MR22_POS = 22U,   // Interrupt Mask on line 22
} exti_imr_pos_e;

typedef enum : uint32_t {
    EXTI_IMR_MR0  = (1U << EXTI_IMR_MR0_POS),    // Interrupt Mask on line 0
    EXTI_IMR_MR1  = (1U << EXTI_IMR_MR1_POS),    // Interrupt Mask on line 1
    EXTI_IMR_MR2  = (1U << EXTI_IMR_MR2_POS),    // Interrupt Mask on line 2
    EXTI_IMR_MR3  = (1U << EXTI_IMR_MR3_POS),    // Interrupt Mask on line 3
    EXTI_IMR_MR4  = (1U << EXTI_IMR_MR4_POS),    // Interrupt Mask on line 4
    EXTI_IMR_MR5  = (1U << EXTI_IMR_MR5_POS),    // Interrupt Mask on line 5
    EXTI_IMR_MR6  = (1U << EXTI_IMR_MR6_POS),    // Interrupt Mask on line 6
    EXTI_IMR_MR7  = (1U << EXTI_IMR_MR7_POS),    // Interrupt Mask on line 7
    EXTI_IMR_MR8  = (1U << EXTI_IMR_MR8_POS),    // Interrupt Mask on line 8
    EXTI_IMR_MR9  = (1U << EXTI_IMR_MR9_POS),    // Interrupt Mask on line 9
    EXTI_IMR_MR10 = (1U << EXTI_IMR_MR10_POS),   // Interrupt Mask on line 10
    EXTI_IMR_MR11 = (1U << EXTI_IMR_MR11_POS),   // Interrupt Mask on line 11
    EXTI_IMR_MR12 = (1U << EXTI_IMR_MR12_POS),   // Interrupt Mask on line 12
    EXTI_IMR_MR13 = (1U << EXTI_IMR_MR13_POS),   // Interrupt Mask on line 13
    EXTI_IMR_MR14 = (1U << EXTI_IMR_MR14_POS),   // Interrupt Mask on line 14
    EXTI_IMR_MR15 = (1U << EXTI_IMR_MR15_POS),   // Interrupt Mask on line 15
    EXTI_IMR_MR16 = (1U << EXTI_IMR_MR16_POS),   // Interrupt Mask on line 16
    EXTI_IMR_MR17 = (1U << EXTI_IMR_MR17_POS),   // Interrupt Mask on line 17
    EXTI_IMR_MR18 = (1U << EXTI_IMR_MR18_POS),   // Interrupt Mask on line 18
    EXTI_IMR_MR19 = (1U << EXTI_IMR_MR19_POS),   // Interrupt Mask on line 19
    EXTI_IMR_MR20 = (1U << EXTI_IMR_MR20_POS),   // Interrupt Mask on line 20
    EXTI_IMR_MR21 = (1U << EXTI_IMR_MR21_POS),   // Interrupt Mask on line 21
    EXTI_IMR_MR22 = (1U << EXTI_IMR_MR22_POS),   // Interrupt Mask on line 22
} exti_imr_e;

typedef enum : uint32_t {
    EXTI_IMR_MR0_MASK  = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR1_MASK  = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR2_MASK  = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR3_MASK  = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR4_MASK  = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR5_MASK  = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR6_MASK  = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR7_MASK  = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR8_MASK  = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR9_MASK  = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR10_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR11_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR12_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR13_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR14_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR15_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR16_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR17_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR18_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR19_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR20_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR21_MASK = 0b1U,   // 1 bit(s)
    EXTI_IMR_MR22_MASK = 0b1U,   // 1 bit(s)
} exti_imr_mask_e;

typedef enum : uint32_t {
    EXTI_EMR_MR0_POS  = 0U,    // Event Mask on line 0
    EXTI_EMR_MR1_POS  = 1U,    // Event Mask on line 1
    EXTI_EMR_MR2_POS  = 2U,    // Event Mask on line 2
    EXTI_EMR_MR3_POS  = 3U,    // Event Mask on line 3
    EXTI_EMR_MR4_POS  = 4U,    // Event Mask on line 4
    EXTI_EMR_MR5_POS  = 5U,    // Event Mask on line 5
    EXTI_EMR_MR6_POS  = 6U,    // Event Mask on line 6
    EXTI_EMR_MR7_POS  = 7U,    // Event Mask on line 7
    EXTI_EMR_MR8_POS  = 8U,    // Event Mask on line 8
    EXTI_EMR_MR9_POS  = 9U,    // Event Mask on line 9
    EXTI_EMR_MR10_POS = 10U,   // Event Mask on line 10
    EXTI_EMR_MR11_POS = 11U,   // Event Mask on line 11
    EXTI_EMR_MR12_POS = 12U,   // Event Mask on line 12
    EXTI_EMR_MR13_POS = 13U,   // Event Mask on line 13
    EXTI_EMR_MR14_POS = 14U,   // Event Mask on line 14
    EXTI_EMR_MR15_POS = 15U,   // Event Mask on line 15
    EXTI_EMR_MR16_POS = 16U,   // Event Mask on line 16
    EXTI_EMR_MR17_POS = 17U,   // Event Mask on line 17
    EXTI_EMR_MR18_POS = 18U,   // Event Mask on line 18
    EXTI_EMR_MR19_POS = 19U,   // Event Mask on line 19
    EXTI_EMR_MR20_POS = 20U,   // Event Mask on line 20
    EXTI_EMR_MR21_POS = 21U,   // Event Mask on line 21
    EXTI_EMR_MR22_POS = 22U,   // Event Mask on line 22
} exti_emr_pos_e;

typedef enum : uint32_t {
    EXTI_EMR_MR0  = (1U << EXTI_EMR_MR0_POS),    // Event Mask on line 0
    EXTI_EMR_MR1  = (1U << EXTI_EMR_MR1_POS),    // Event Mask on line 1
    EXTI_EMR_MR2  = (1U << EXTI_EMR_MR2_POS),    // Event Mask on line 2
    EXTI_EMR_MR3  = (1U << EXTI_EMR_MR3_POS),    // Event Mask on line 3
    EXTI_EMR_MR4  = (1U << EXTI_EMR_MR4_POS),    // Event Mask on line 4
    EXTI_EMR_MR5  = (1U << EXTI_EMR_MR5_POS),    // Event Mask on line 5
    EXTI_EMR_MR6  = (1U << EXTI_EMR_MR6_POS),    // Event Mask on line 6
    EXTI_EMR_MR7  = (1U << EXTI_EMR_MR7_POS),    // Event Mask on line 7
    EXTI_EMR_MR8  = (1U << EXTI_EMR_MR8_POS),    // Event Mask on line 8
    EXTI_EMR_MR9  = (1U << EXTI_EMR_MR9_POS),    // Event Mask on line 9
    EXTI_EMR_MR10 = (1U << EXTI_EMR_MR10_POS),   // Event Mask on line 10
    EXTI_EMR_MR11 = (1U << EXTI_EMR_MR11_POS),   // Event Mask on line 11
    EXTI_EMR_MR12 = (1U << EXTI_EMR_MR12_POS),   // Event Mask on line 12
    EXTI_EMR_MR13 = (1U << EXTI_EMR_MR13_POS),   // Event Mask on line 13
    EXTI_EMR_MR14 = (1U << EXTI_EMR_MR14_POS),   // Event Mask on line 14
    EXTI_EMR_MR15 = (1U << EXTI_EMR_MR15_POS),   // Event Mask on line 15
    EXTI_EMR_MR16 = (1U << EXTI_EMR_MR16_POS),   // Event Mask on line 16
    EXTI_EMR_MR17 = (1U << EXTI_EMR_MR17_POS),   // Event Mask on line 17
    EXTI_EMR_MR18 = (1U << EXTI_EMR_MR18_POS),   // Event Mask on line 18
    EXTI_EMR_MR19 = (1U << EXTI_EMR_MR19_POS),   // Event Mask on line 19
    EXTI_EMR_MR20 = (1U << EXTI_EMR_MR20_POS),   // Event Mask on line 20
    EXTI_EMR_MR21 = (1U << EXTI_EMR_MR21_POS),   // Event Mask on line 21
    EXTI_EMR_MR22 = (1U << EXTI_EMR_MR22_POS),   // Event Mask on line 22
} exti_emr_e;

typedef enum : uint32_t {
    EXTI_EMR_MR0_MASK  = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR1_MASK  = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR2_MASK  = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR3_MASK  = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR4_MASK  = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR5_MASK  = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR6_MASK  = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR7_MASK  = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR8_MASK  = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR9_MASK  = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR10_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR11_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR12_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR13_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR14_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR15_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR16_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR17_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR18_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR19_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR20_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR21_MASK = 0b1U,   // 1 bit(s)
    EXTI_EMR_MR22_MASK = 0b1U,   // 1 bit(s)
} exti_emr_mask_e;

typedef enum : uint32_t {
    EXTI_RTSR_TR0_POS  = 0U,    // Rising trigger event configuration of line 0
    EXTI_RTSR_TR1_POS  = 1U,    // Rising trigger event configuration of line 1
    EXTI_RTSR_TR2_POS  = 2U,    // Rising trigger event configuration of line 2
    EXTI_RTSR_TR3_POS  = 3U,    // Rising trigger event configuration of line 3
    EXTI_RTSR_TR4_POS  = 4U,    // Rising trigger event configuration of line 4
    EXTI_RTSR_TR5_POS  = 5U,    // Rising trigger event configuration of line 5
    EXTI_RTSR_TR6_POS  = 6U,    // Rising trigger event configuration of line 6
    EXTI_RTSR_TR7_POS  = 7U,    // Rising trigger event configuration of line 7
    EXTI_RTSR_TR8_POS  = 8U,    // Rising trigger event configuration of line 8
    EXTI_RTSR_TR9_POS  = 9U,    // Rising trigger event configuration of line 9
    EXTI_RTSR_TR10_POS = 10U,   // Rising trigger event configuration of line 10
    EXTI_RTSR_TR11_POS = 11U,   // Rising trigger event configuration of line 11
    EXTI_RTSR_TR12_POS = 12U,   // Rising trigger event configuration of line 12
    EXTI_RTSR_TR13_POS = 13U,   // Rising trigger event configuration of line 13
    EXTI_RTSR_TR14_POS = 14U,   // Rising trigger event configuration of line 14
    EXTI_RTSR_TR15_POS = 15U,   // Rising trigger event configuration of line 15
    EXTI_RTSR_TR16_POS = 16U,   // Rising trigger event configuration of line 16
    EXTI_RTSR_TR17_POS = 17U,   // Rising trigger event configuration of line 17
    EXTI_RTSR_TR18_POS = 18U,   // Rising trigger event configuration of line 18
    EXTI_RTSR_TR19_POS = 19U,   // Rising trigger event configuration of line 19
    EXTI_RTSR_TR20_POS = 20U,   // Rising trigger event configuration of line 20
    EXTI_RTSR_TR21_POS = 21U,   // Rising trigger event configuration of line 21
    EXTI_RTSR_TR22_POS = 22U,   // Rising trigger event configuration of line 22
} exti_rtsr_pos_e;

typedef enum : uint32_t {
    EXTI_RTSR_TR0  = (1U << EXTI_RTSR_TR0_POS),    // Rising trigger event configuration of line 0
    EXTI_RTSR_TR1  = (1U << EXTI_RTSR_TR1_POS),    // Rising trigger event configuration of line 1
    EXTI_RTSR_TR2  = (1U << EXTI_RTSR_TR2_POS),    // Rising trigger event configuration of line 2
    EXTI_RTSR_TR3  = (1U << EXTI_RTSR_TR3_POS),    // Rising trigger event configuration of line 3
    EXTI_RTSR_TR4  = (1U << EXTI_RTSR_TR4_POS),    // Rising trigger event configuration of line 4
    EXTI_RTSR_TR5  = (1U << EXTI_RTSR_TR5_POS),    // Rising trigger event configuration of line 5
    EXTI_RTSR_TR6  = (1U << EXTI_RTSR_TR6_POS),    // Rising trigger event configuration of line 6
    EXTI_RTSR_TR7  = (1U << EXTI_RTSR_TR7_POS),    // Rising trigger event configuration of line 7
    EXTI_RTSR_TR8  = (1U << EXTI_RTSR_TR8_POS),    // Rising trigger event configuration of line 8
    EXTI_RTSR_TR9  = (1U << EXTI_RTSR_TR9_POS),    // Rising trigger event configuration of line 9
    EXTI_RTSR_TR10 = (1U << EXTI_RTSR_TR10_POS),   // Rising trigger event configuration of line 10
    EXTI_RTSR_TR11 = (1U << EXTI_RTSR_TR11_POS),   // Rising trigger event configuration of line 11
    EXTI_RTSR_TR12 = (1U << EXTI_RTSR_TR12_POS),   // Rising trigger event configuration of line 12
    EXTI_RTSR_TR13 = (1U << EXTI_RTSR_TR13_POS),   // Rising trigger event configuration of line 13
    EXTI_RTSR_TR14 = (1U << EXTI_RTSR_TR14_POS),   // Rising trigger event configuration of line 14
    EXTI_RTSR_TR15 = (1U << EXTI_RTSR_TR15_POS),   // Rising trigger event configuration of line 15
    EXTI_RTSR_TR16 = (1U << EXTI_RTSR_TR16_POS),   // Rising trigger event configuration of line 16
    EXTI_RTSR_TR17 = (1U << EXTI_RTSR_TR17_POS),   // Rising trigger event configuration of line 17
    EXTI_RTSR_TR18 = (1U << EXTI_RTSR_TR18_POS),   // Rising trigger event configuration of line 18
    EXTI_RTSR_TR19 = (1U << EXTI_RTSR_TR19_POS),   // Rising trigger event configuration of line 19
    EXTI_RTSR_TR20 = (1U << EXTI_RTSR_TR20_POS),   // Rising trigger event configuration of line 20
    EXTI_RTSR_TR21 = (1U << EXTI_RTSR_TR21_POS),   // Rising trigger event configuration of line 21
    EXTI_RTSR_TR22 = (1U << EXTI_RTSR_TR22_POS),   // Rising trigger event configuration of line 22
} exti_rtsr_e;

typedef enum : uint32_t {
    EXTI_RTSR_TR0_MASK  = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR1_MASK  = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR2_MASK  = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR3_MASK  = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR4_MASK  = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR5_MASK  = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR6_MASK  = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR7_MASK  = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR8_MASK  = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR9_MASK  = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR10_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR11_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR12_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR13_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR14_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR15_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR16_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR17_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR18_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR19_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR20_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR21_MASK = 0b1U,   // 1 bit(s)
    EXTI_RTSR_TR22_MASK = 0b1U,   // 1 bit(s)
} exti_rtsr_mask_e;

typedef enum : uint32_t {
    EXTI_FTSR_TR0_POS  = 0U,    // Falling trigger event configuration of line 0
    EXTI_FTSR_TR1_POS  = 1U,    // Falling trigger event configuration of line 1
    EXTI_FTSR_TR2_POS  = 2U,    // Falling trigger event configuration of line 2
    EXTI_FTSR_TR3_POS  = 3U,    // Falling trigger event configuration of line 3
    EXTI_FTSR_TR4_POS  = 4U,    // Falling trigger event configuration of line 4
    EXTI_FTSR_TR5_POS  = 5U,    // Falling trigger event configuration of line 5
    EXTI_FTSR_TR6_POS  = 6U,    // Falling trigger event configuration of line 6
    EXTI_FTSR_TR7_POS  = 7U,    // Falling trigger event configuration of line 7
    EXTI_FTSR_TR8_POS  = 8U,    // Falling trigger event configuration of line 8
    EXTI_FTSR_TR9_POS  = 9U,    // Falling trigger event configuration of line 9
    EXTI_FTSR_TR10_POS = 10U,   // Falling trigger event configuration of line 10
    EXTI_FTSR_TR11_POS = 11U,   // Falling trigger event configuration of line 11
    EXTI_FTSR_TR12_POS = 12U,   // Falling trigger event configuration of line 12
    EXTI_FTSR_TR13_POS = 13U,   // Falling trigger event configuration of line 13
    EXTI_FTSR_TR14_POS = 14U,   // Falling trigger event configuration of line 14
    EXTI_FTSR_TR15_POS = 15U,   // Falling trigger event configuration of line 15
    EXTI_FTSR_TR16_POS = 16U,   // Falling trigger event configuration of line 16
    EXTI_FTSR_TR17_POS = 17U,   // Falling trigger event configuration of line 17
    EXTI_FTSR_TR18_POS = 18U,   // Falling trigger event configuration of line 18
    EXTI_FTSR_TR19_POS = 19U,   // Falling trigger event configuration of line 19
    EXTI_FTSR_TR20_POS = 20U,   // Falling trigger event configuration of line 20
    EXTI_FTSR_TR21_POS = 21U,   // Falling trigger event configuration of line 21
    EXTI_FTSR_TR22_POS = 22U,   // Falling trigger event configuration of line 22
} exti_ftsr_pos_e;

typedef enum : uint32_t {
    EXTI_FTSR_TR0  = (1U << EXTI_FTSR_TR0_POS),    // Falling trigger event configuration of line 0
    EXTI_FTSR_TR1  = (1U << EXTI_FTSR_TR1_POS),    // Falling trigger event configuration of line 1
    EXTI_FTSR_TR2  = (1U << EXTI_FTSR_TR2_POS),    // Falling trigger event configuration of line 2
    EXTI_FTSR_TR3  = (1U << EXTI_FTSR_TR3_POS),    // Falling trigger event configuration of line 3
    EXTI_FTSR_TR4  = (1U << EXTI_FTSR_TR4_POS),    // Falling trigger event configuration of line 4
    EXTI_FTSR_TR5  = (1U << EXTI_FTSR_TR5_POS),    // Falling trigger event configuration of line 5
    EXTI_FTSR_TR6  = (1U << EXTI_FTSR_TR6_POS),    // Falling trigger event configuration of line 6
    EXTI_FTSR_TR7  = (1U << EXTI_FTSR_TR7_POS),    // Falling trigger event configuration of line 7
    EXTI_FTSR_TR8  = (1U << EXTI_FTSR_TR8_POS),    // Falling trigger event configuration of line 8
    EXTI_FTSR_TR9  = (1U << EXTI_FTSR_TR9_POS),    // Falling trigger event configuration of line 9
    EXTI_FTSR_TR10 = (1U << EXTI_FTSR_TR10_POS),   // Falling trigger event configuration of line 10
    EXTI_FTSR_TR11 = (1U << EXTI_FTSR_TR11_POS),   // Falling trigger event configuration of line 11
    EXTI_FTSR_TR12 = (1U << EXTI_FTSR_TR12_POS),   // Falling trigger event configuration of line 12
    EXTI_FTSR_TR13 = (1U << EXTI_FTSR_TR13_POS),   // Falling trigger event configuration of line 13
    EXTI_FTSR_TR14 = (1U << EXTI_FTSR_TR14_POS),   // Falling trigger event configuration of line 14
    EXTI_FTSR_TR15 = (1U << EXTI_FTSR_TR15_POS),   // Falling trigger event configuration of line 15
    EXTI_FTSR_TR16 = (1U << EXTI_FTSR_TR16_POS),   // Falling trigger event configuration of line 16
    EXTI_FTSR_TR17 = (1U << EXTI_FTSR_TR17_POS),   // Falling trigger event configuration of line 17
    EXTI_FTSR_TR18 = (1U << EXTI_FTSR_TR18_POS),   // Falling trigger event configuration of line 18
    EXTI_FTSR_TR19 = (1U << EXTI_FTSR_TR19_POS),   // Falling trigger event configuration of line 19
    EXTI_FTSR_TR20 = (1U << EXTI_FTSR_TR20_POS),   // Falling trigger event configuration of line 20
    EXTI_FTSR_TR21 = (1U << EXTI_FTSR_TR21_POS),   // Falling trigger event configuration of line 21
    EXTI_FTSR_TR22 = (1U << EXTI_FTSR_TR22_POS),   // Falling trigger event configuration of line 22
} exti_ftsr_e;

typedef enum : uint32_t {
    EXTI_FTSR_TR0_MASK  = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR1_MASK  = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR2_MASK  = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR3_MASK  = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR4_MASK  = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR5_MASK  = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR6_MASK  = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR7_MASK  = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR8_MASK  = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR9_MASK  = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR10_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR11_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR12_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR13_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR14_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR15_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR16_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR17_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR18_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR19_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR20_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR21_MASK = 0b1U,   // 1 bit(s)
    EXTI_FTSR_TR22_MASK = 0b1U,   // 1 bit(s)
} exti_ftsr_mask_e;

typedef enum : uint32_t {
    EXTI_SWIER_SWIER0_POS  = 0U,    // Software Interrupt on line 0
    EXTI_SWIER_SWIER1_POS  = 1U,    // Software Interrupt on line 1
    EXTI_SWIER_SWIER2_POS  = 2U,    // Software Interrupt on line 2
    EXTI_SWIER_SWIER3_POS  = 3U,    // Software Interrupt on line 3
    EXTI_SWIER_SWIER4_POS  = 4U,    // Software Interrupt on line 4
    EXTI_SWIER_SWIER5_POS  = 5U,    // Software Interrupt on line 5
    EXTI_SWIER_SWIER6_POS  = 6U,    // Software Interrupt on line 6
    EXTI_SWIER_SWIER7_POS  = 7U,    // Software Interrupt on line 7
    EXTI_SWIER_SWIER8_POS  = 8U,    // Software Interrupt on line 8
    EXTI_SWIER_SWIER9_POS  = 9U,    // Software Interrupt on line 9
    EXTI_SWIER_SWIER10_POS = 10U,   // Software Interrupt on line 10
    EXTI_SWIER_SWIER11_POS = 11U,   // Software Interrupt on line 11
    EXTI_SWIER_SWIER12_POS = 12U,   // Software Interrupt on line 12
    EXTI_SWIER_SWIER13_POS = 13U,   // Software Interrupt on line 13
    EXTI_SWIER_SWIER14_POS = 14U,   // Software Interrupt on line 14
    EXTI_SWIER_SWIER15_POS = 15U,   // Software Interrupt on line 15
    EXTI_SWIER_SWIER16_POS = 16U,   // Software Interrupt on line 16
    EXTI_SWIER_SWIER17_POS = 17U,   // Software Interrupt on line 17
    EXTI_SWIER_SWIER18_POS = 18U,   // Software Interrupt on line 18
    EXTI_SWIER_SWIER19_POS = 19U,   // Software Interrupt on line 19
    EXTI_SWIER_SWIER20_POS = 20U,   // Software Interrupt on line 20
    EXTI_SWIER_SWIER21_POS = 21U,   // Software Interrupt on line 21
    EXTI_SWIER_SWIER22_POS = 22U,   // Software Interrupt on line 22
} exti_swier_pos_e;

typedef enum : uint32_t {
    EXTI_SWIER_SWIER0  = (1U << EXTI_SWIER_SWIER0_POS),    // Software Interrupt on line 0
    EXTI_SWIER_SWIER1  = (1U << EXTI_SWIER_SWIER1_POS),    // Software Interrupt on line 1
    EXTI_SWIER_SWIER2  = (1U << EXTI_SWIER_SWIER2_POS),    // Software Interrupt on line 2
    EXTI_SWIER_SWIER3  = (1U << EXTI_SWIER_SWIER3_POS),    // Software Interrupt on line 3
    EXTI_SWIER_SWIER4  = (1U << EXTI_SWIER_SWIER4_POS),    // Software Interrupt on line 4
    EXTI_SWIER_SWIER5  = (1U << EXTI_SWIER_SWIER5_POS),    // Software Interrupt on line 5
    EXTI_SWIER_SWIER6  = (1U << EXTI_SWIER_SWIER6_POS),    // Software Interrupt on line 6
    EXTI_SWIER_SWIER7  = (1U << EXTI_SWIER_SWIER7_POS),    // Software Interrupt on line 7
    EXTI_SWIER_SWIER8  = (1U << EXTI_SWIER_SWIER8_POS),    // Software Interrupt on line 8
    EXTI_SWIER_SWIER9  = (1U << EXTI_SWIER_SWIER9_POS),    // Software Interrupt on line 9
    EXTI_SWIER_SWIER10 = (1U << EXTI_SWIER_SWIER10_POS),   // Software Interrupt on line 10
    EXTI_SWIER_SWIER11 = (1U << EXTI_SWIER_SWIER11_POS),   // Software Interrupt on line 11
    EXTI_SWIER_SWIER12 = (1U << EXTI_SWIER_SWIER12_POS),   // Software Interrupt on line 12
    EXTI_SWIER_SWIER13 = (1U << EXTI_SWIER_SWIER13_POS),   // Software Interrupt on line 13
    EXTI_SWIER_SWIER14 = (1U << EXTI_SWIER_SWIER14_POS),   // Software Interrupt on line 14
    EXTI_SWIER_SWIER15 = (1U << EXTI_SWIER_SWIER15_POS),   // Software Interrupt on line 15
    EXTI_SWIER_SWIER16 = (1U << EXTI_SWIER_SWIER16_POS),   // Software Interrupt on line 16
    EXTI_SWIER_SWIER17 = (1U << EXTI_SWIER_SWIER17_POS),   // Software Interrupt on line 17
    EXTI_SWIER_SWIER18 = (1U << EXTI_SWIER_SWIER18_POS),   // Software Interrupt on line 18
    EXTI_SWIER_SWIER19 = (1U << EXTI_SWIER_SWIER19_POS),   // Software Interrupt on line 19
    EXTI_SWIER_SWIER20 = (1U << EXTI_SWIER_SWIER20_POS),   // Software Interrupt on line 20
    EXTI_SWIER_SWIER21 = (1U << EXTI_SWIER_SWIER21_POS),   // Software Interrupt on line 21
    EXTI_SWIER_SWIER22 = (1U << EXTI_SWIER_SWIER22_POS),   // Software Interrupt on line 22
} exti_swier_e;

typedef enum : uint32_t {
    EXTI_SWIER_SWIER0_MASK  = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER1_MASK  = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER2_MASK  = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER3_MASK  = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER4_MASK  = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER5_MASK  = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER6_MASK  = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER7_MASK  = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER8_MASK  = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER9_MASK  = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER10_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER11_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER12_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER13_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER14_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER15_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER16_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER17_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER18_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER19_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER20_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER21_MASK = 0b1U,   // 1 bit(s)
    EXTI_SWIER_SWIER22_MASK = 0b1U,   // 1 bit(s)
} exti_swier_mask_e;

typedef enum : uint32_t {
    EXTI_PR_PR0_POS  = 0U,    // Pending bit 0
    EXTI_PR_PR1_POS  = 1U,    // Pending bit 1
    EXTI_PR_PR2_POS  = 2U,    // Pending bit 2
    EXTI_PR_PR3_POS  = 3U,    // Pending bit 3
    EXTI_PR_PR4_POS  = 4U,    // Pending bit 4
    EXTI_PR_PR5_POS  = 5U,    // Pending bit 5
    EXTI_PR_PR6_POS  = 6U,    // Pending bit 6
    EXTI_PR_PR7_POS  = 7U,    // Pending bit 7
    EXTI_PR_PR8_POS  = 8U,    // Pending bit 8
    EXTI_PR_PR9_POS  = 9U,    // Pending bit 9
    EXTI_PR_PR10_POS = 10U,   // Pending bit 10
    EXTI_PR_PR11_POS = 11U,   // Pending bit 11
    EXTI_PR_PR12_POS = 12U,   // Pending bit 12
    EXTI_PR_PR13_POS = 13U,   // Pending bit 13
    EXTI_PR_PR14_POS = 14U,   // Pending bit 14
    EXTI_PR_PR15_POS = 15U,   // Pending bit 15
    EXTI_PR_PR16_POS = 16U,   // Pending bit 16
    EXTI_PR_PR17_POS = 17U,   // Pending bit 17
    EXTI_PR_PR18_POS = 18U,   // Pending bit 18
    EXTI_PR_PR19_POS = 19U,   // Pending bit 19
    EXTI_PR_PR20_POS = 20U,   // Pending bit 20
    EXTI_PR_PR21_POS = 21U,   // Pending bit 21
    EXTI_PR_PR22_POS = 22U,   // Pending bit 22
} exti_pr_pos_e;

typedef enum : uint32_t {
    EXTI_PR_PR0  = (1U << EXTI_PR_PR0_POS),    // Pending bit 0
    EXTI_PR_PR1  = (1U << EXTI_PR_PR1_POS),    // Pending bit 1
    EXTI_PR_PR2  = (1U << EXTI_PR_PR2_POS),    // Pending bit 2
    EXTI_PR_PR3  = (1U << EXTI_PR_PR3_POS),    // Pending bit 3
    EXTI_PR_PR4  = (1U << EXTI_PR_PR4_POS),    // Pending bit 4
    EXTI_PR_PR5  = (1U << EXTI_PR_PR5_POS),    // Pending bit 5
    EXTI_PR_PR6  = (1U << EXTI_PR_PR6_POS),    // Pending bit 6
    EXTI_PR_PR7  = (1U << EXTI_PR_PR7_POS),    // Pending bit 7
    EXTI_PR_PR8  = (1U << EXTI_PR_PR8_POS),    // Pending bit 8
    EXTI_PR_PR9  = (1U << EXTI_PR_PR9_POS),    // Pending bit 9
    EXTI_PR_PR10 = (1U << EXTI_PR_PR10_POS),   // Pending bit 10
    EXTI_PR_PR11 = (1U << EXTI_PR_PR11_POS),   // Pending bit 11
    EXTI_PR_PR12 = (1U << EXTI_PR_PR12_POS),   // Pending bit 12
    EXTI_PR_PR13 = (1U << EXTI_PR_PR13_POS),   // Pending bit 13
    EXTI_PR_PR14 = (1U << EXTI_PR_PR14_POS),   // Pending bit 14
    EXTI_PR_PR15 = (1U << EXTI_PR_PR15_POS),   // Pending bit 15
    EXTI_PR_PR16 = (1U << EXTI_PR_PR16_POS),   // Pending bit 16
    EXTI_PR_PR17 = (1U << EXTI_PR_PR17_POS),   // Pending bit 17
    EXTI_PR_PR18 = (1U << EXTI_PR_PR18_POS),   // Pending bit 18
    EXTI_PR_PR19 = (1U << EXTI_PR_PR19_POS),   // Pending bit 19
    EXTI_PR_PR20 = (1U << EXTI_PR_PR20_POS),   // Pending bit 20
    EXTI_PR_PR21 = (1U << EXTI_PR_PR21_POS),   // Pending bit 21
    EXTI_PR_PR22 = (1U << EXTI_PR_PR22_POS),   // Pending bit 22
} exti_pr_e;

typedef enum : uint32_t {
    EXTI_PR_PR0_MASK  = 0b1U,   // 1 bit(s)
    EXTI_PR_PR1_MASK  = 0b1U,   // 1 bit(s)
    EXTI_PR_PR2_MASK  = 0b1U,   // 1 bit(s)
    EXTI_PR_PR3_MASK  = 0b1U,   // 1 bit(s)
    EXTI_PR_PR4_MASK  = 0b1U,   // 1 bit(s)
    EXTI_PR_PR5_MASK  = 0b1U,   // 1 bit(s)
    EXTI_PR_PR6_MASK  = 0b1U,   // 1 bit(s)
    EXTI_PR_PR7_MASK  = 0b1U,   // 1 bit(s)
    EXTI_PR_PR8_MASK  = 0b1U,   // 1 bit(s)
    EXTI_PR_PR9_MASK  = 0b1U,   // 1 bit(s)
    EXTI_PR_PR10_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR11_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR12_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR13_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR14_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR15_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR16_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR17_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR18_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR19_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR20_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR21_MASK = 0b1U,   // 1 bit(s)
    EXTI_PR_PR22_MASK = 0b1U,   // 1 bit(s)
} exti_pr_mask_e;

// clang-format on

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

typedef __vo struct {
    uint32_t IMR;   // Interrupt mask register                             Offset: 0x0
    uint32_t EMR;   // Event mask register                                 Offset: 0x4
    uint32_t RTSR;  // Rising Trigger selection register                   Offset: 0x8
    uint32_t FTSR;  // Falling Trigger selection register                  Offset: 0xC
    uint32_t SWIER; // Software interrupt event register                   Offset: 0x10
    uint32_t PR;    // Pending register                                    Offset: 0x14
} exti_reg_def;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define EXTI ((exti_reg_def *)EXTI_BASE_ADDR)

//======================================================================================//}
//                  Function API Prototypes
//======================================================================================//{

void exti_it_config(exti_lines_e line_no, togglable_e toggle);
void exti_event_config(exti_lines_e line_no, togglable_e toggle);
void exti_rising_edge_config(exti_lines_e line_no, togglable_e toggle);
void exti_falling_edge_config(exti_lines_e line_no, togglable_e toggle);
void exti_software_it_event(exti_lines_e line_no);
exti_pend_status_e exti_get_pending(exti_lines_e line_no);
void exti_clear_pending(exti_lines_e line_no);

#endif
