#ifndef STM32F4XX_H
#define STM32F4XX_H

#include "common.h"

//============================================================//
//          Address Definitions
//============================================================//

typedef enum {
    EXTI_BASE_ADDR   = (0x40013C00ul),
    SYSCFG_BASE_ADDR = (0x40013800ul),
} misc_base_addr_e;

typedef enum {
    NVIC_ISER_BASE_ADDR = (0xE000E100ul),
    NVIC_ICER_BASE_ADDR = (0xE000E180ul),
    NVIC_ISPR_BASE_ADDR = (0xE000E200ul),
    NVIC_ICPR_BASE_ADDR = (0xE000E280ul),
    NVIC_IABR_BASE_ADDR = (0xE000E300ul),
    NVIC_IPR_BASE_ADDR  = (0xE000E400ul),
} nvic_base_addr_e;

typedef enum {
    APB1_BASE_ADDR = (0x40000000ul),
    APB2_BASE_ADDR = (0x40010000ul),
    AHB1_BASE_ADDR = (0x40020000ul),
    AHB2_BASE_ADDR = (0x50000000ul),
} bus_base_addr_e;

//======================================================================================//
//                  Macros and Other Enums
//======================================================================================//

// Enums
// General
typedef enum {
    PIN_NO_0  = 0,
    PIN_NO_1  = 1,
    PIN_NO_2  = 2,
    PIN_NO_3  = 3,
    PIN_NO_4  = 4,
    PIN_NO_5  = 5,
    PIN_NO_6  = 6,
    PIN_NO_7  = 7,
    PIN_NO_8  = 8,
    PIN_NO_9  = 9,
    PIN_NO_10 = 10,
    PIN_NO_11 = 11,
    PIN_NO_12 = 12,
    PIN_NO_13 = 13,
    PIN_NO_14 = 14,
    PIN_NO_15 = 15,
} pin_number_e;

typedef enum {
    HIGH = 1,
    LOW  = 0
} pin_logic_level_e;

typedef enum {
    ENABLE  = 1,
    DISABLE = 0,
} togglable_e;

// Only adding IRQ numbers when they are needed
typedef enum {
    EXTI0_IRQ_NO_6               = 6,
    EXTI1_IRQ_NO_7               = 7,
    EXTI2_IRQ_NO_8               = 8,
    EXTI3_IRQ_NO_9               = 9,
    EXTI4_IRQ_NO_10              = 10,
    EXTI9_5_IRQ_NO_23            = 23,
    TIM1_BRK_TIM9_IRQ_NO_24      = 24,
    TIM1_UP_TIM10_IRQ_NO_25      = 25,
    TIM1_TRG_COM_TIM11_IRQ_NO_26 = 26,
    TIM1_CC_IRQ_NO_27            = 27,
    TIM2_IRQ_NO_28               = 28,
    TIM3_IRQ_NO_29               = 29,
    TIM4_IRQ_NO_30               = 30,
    I2C1_EV_IRQ_NO_31            = 31,
    I2C1_ER_IRQ_NO_32            = 32,
    I2C2_EV_IRQ_NO_33            = 33,
    I2C2_ER_IRQ_NO_34            = 34,
    USART1_IRQ_NO_37             = 37,
    USART2_IRQ_NO_38             = 38,
    USART3_IRQ_NO_39             = 39,
    EXTI15_10_IRQ_NO_40          = 40,
    TIM8_BRK_TIM12_IRQ_NO_43     = 43,
    TIM8_UP_TIM13_IRQ_NO_44      = 44,
    TIM8_TRG_COM_TIM14_IRQ_NO_45 = 45,
    TIM8_CC_IRQ_NO_46            = 46,
    UART4_IRQ_NO_52              = 52,
    UART5_IRQ_NO_53              = 53,
    TIM7_IRQ_NO_55               = 55,
    USART6_IRQ_NO_71             = 71,
    I2C3_EV_IRQ_NO_72            = 72,
} irq_number_e;

// Lower number means higher priority
typedef enum {
    IRQ_PRIORITY_0                   = 0,
    IRQ_PRIORITY_1                   = 1,
    IRQ_PRIORITY_2                   = 2,
    IRQ_PRIORITY_3                   = 3,
    IRQ_PRIORITY_4                   = 4,
    IRQ_PRIORITY_5                   = 5,
    IRQ_PRIORITY_6                   = 6,
    IRQ_PRIORITY_7                   = 7,
    IRQ_PRIORITY_8                   = 8,
    IRQ_PRIORITY_9                   = 9,
    IRQ_PRIORITY_10                  = 10,
    IRQ_PRIORITY_11                  = 11,
    IRQ_PRIORITY_12                  = 12,
    IRQ_PRIORITY_13                  = 13,
    IRQ_PRIORITY_14                  = 14,
    IRQ_PRIORITY_15                  = 15,
    IRQ_NUM_PRIORITY_BITS_IMPLMENTED = 4,
} irq_priority_e;

typedef enum {
    LED_GREEN_PIN  = 12,
    LED_ORANGE_PIN = 13,
    LED_RED_PIN    = 14,
    LED_BLUE_PIN   = 15,
} hardware_pin_assignment_e;

// Ports mapped to integers A->0, B->1, C->2... etc.
typedef enum {
    LED_GREEN_PORT  = 3u, // Port D
    LED_ORANGE_PORT = 3u,
    LED_RED_PORT    = 3u,
    LED_BLUE_PORT   = 3u,
} hardware_port_assignment_e;

//======================================================================================//
//                  Register Structure Definitions
//======================================================================================//

typedef __vo struct {
    uint32_t IMR;   // Interrupt mask register                             Offset: 0x0
    uint32_t EMR;   // Event mask register                                 Offset: 0x4
    uint32_t RTSR;  // Rising Trigger selection register                   Offset: 0x8
    uint32_t FTSR;  // Falling Trigger selection register                  Offset: 0xC
    uint32_t SWIER; // Software interrupt event register                   Offset: 0x10
    uint32_t PR;    // Pending register                                    Offset: 0x14
} exti_reg_def;

typedef __vo struct {
    uint32_t MEMRM;      // memory remap register                            Offset: 0x0
    uint32_t PMC;        // peripheral mode configuration register           Offset: 0x4
    uint32_t EXTICR[4];  // external interrupt configuration register 1-4    Offset: 0x8
    uint32_t reserved_1; // Reserved 0x18
    uint32_t reserved_2; // Reserved 0x1C
    uint32_t CMPCR;      // Compensation cell control register               Offset: 0x20
} syscfg_reg_def;

//======================================================================================//
//                  Peripheral Structure Definitions
//======================================================================================//

#define EXTI   ((exti_reg_def *)EXTI_BASE_ADDR)
#define SYSCFG ((syscfg_reg_def *)SYSCFG_BASE_ADDR)

//======================================================================================//
//                  General MCU API Function Prototypes
//======================================================================================//

void irq_config(irq_number_e irq_num, togglable_e toggle);
void irq_priority(irq_number_e irq_num, irq_priority_e irq_pri);

#endif
