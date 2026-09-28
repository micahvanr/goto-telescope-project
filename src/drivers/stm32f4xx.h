#ifndef STM32F4XX_H
#define STM32F4XX_H

#include "common.h"

//============================================================//
//          Address Definitions
//============================================================//

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

//} General Constants
//=========================================//{
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

//} Interrupt Constants
//=========================================//{

// Only adding IRQ numbers when they are needed
typedef enum {
    WWDG_IRQ_NO_0                = 0,
    PVDPVD_IRQ_NO_1              = 1,
    TAMP_STAMP_IRQ_NO_2          = 2,
    RTC_WKUP_IRQ_NO_3            = 3,
    FLASH_IRQ_NO_4               = 4,
    RCC_IRQ_NO_5                 = 5,
    EXTI0_IRQ_NO_6               = 6,
    EXTI1_IRQ_NO_7               = 7,
    EXTI2_IRQ_NO_8               = 8,
    EXTI3_IRQ_NO_9               = 9,
    EXTI4_IRQ_NO_10              = 10,
    DMA1_STREAM0_IRQ_NO_11       = 11,
    DMA1_STREAM1_IRQ_NO_12       = 12,
    DMA1_STREAM2_IRQ_NO_13       = 13,
    DMA1_STREAM3_IRQ_NO_14       = 14,
    DMA1_STREAM4_IRQ_NO_15       = 15,
    DMA1_STREAM5_IRQ_NO_16       = 16,
    DMA1_STREAM6_IRQ_NO_17       = 17,
    ADC_IRQ_NO_18                = 18,
    CAN1_TX_IRQ_NO_19            = 19,
    CAN1_RX0_IRQ_NO_20           = 20,
    CAN1_RX1_IRQ_NO_21           = 21,
    CAN1_SCE_IRQ_NO_22           = 22,
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
    SPI1_IRQ_NO_35               = 35,
    SPI2_IRQ_NO_36               = 36,
    USART1_IRQ_NO_37             = 37,
    USART2_IRQ_NO_38             = 38,
    USART3_IRQ_NO_39             = 39,
    EXTI15_10_IRQ_NO_40          = 40,
    RTC_ALARM_IRQ_NO_41          = 41,
    OTG_FS_WKUP_IRQ_NO_42        = 42,
    TIM8_BRK_TIM12_IRQ_NO_43     = 43,
    TIM8_UP_TIM13_IRQ_NO_44      = 44,
    TIM8_TRG_COM_TIM14_IRQ_NO_45 = 45,
    TIM8_CC_IRQ_NO_46            = 46,
    DMA1_STREAM7_IRQ_NO_47       = 47,
    FSMC_IRQ_NO_48               = 48,
    SDIO_IRQ_NO_49               = 49,
    TIM5_IRQ_NO_50               = 50,
    SPI3_IRQ_NO_51               = 51,
    UART4_IRQ_NO_52              = 52,
    UART5_IRQ_NO_53              = 53,
    TIM6_DAC_IRQ_NO_54           = 54,
    TIM7_IRQ_NO_55               = 55,
    DMA2_STREAM0_IRQ_NO_56       = 56,
    DMA2_STREAM1_IRQ_NO_57       = 57,
    DMA2_STREAM2_IRQ_NO_58       = 58,
    DMA2_STREAM3_IRQ_NO_59       = 59,
    DMA2_STREAM4_IRQ_NO_60       = 60,
    ETH_IRQ_NO_61                = 61,
    ETH_WKUP_IRQ_NO_62           = 62,
    CAN2_TX_IRQ_NO_63            = 63,
    CAN2_RX0_IRQ_NO_64           = 64,
    CAN2_RX1_IRQ_NO_65           = 65,
    CAN2_SCE_IRQ_NO_66           = 66,
    OTG_FS_IRQ_NO_67             = 67,
    DMA2_STREAM5_IRQ_NO_68       = 68,
    DMA2_STREAM6_IRQ_NO_69       = 69,
    DMA2_STREAM7_IRQ_NO_70       = 70,
    USART6_IRQ_NO_71             = 71,
    I2C3_EV_IRQ_NO_72            = 72,
    I2C3_ER_IRQ_NO_73            = 73,
    OTG_HS_EP1_OUT_IRQ_NO_74     = 74,
    OTG_HS_EP1_IN_IRQ_NO_75      = 75,
    OTG_HS_WKUP_IRQ_NO_76        = 76,
    OTG_HS_IRQ_NO_77             = 77,
    DCMI_IRQ_NO_78               = 78,
    CRYP_IRQ_NO_79               = 79,
    HASH_RNG_IRQ_NO_80           = 80,
    FPU_IRQ_NO_81                = 81,
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

//} Hardware Constants
//=========================================//{

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

//======================================================================================//
//                  Peripheral Structure Definitions
//======================================================================================//

//======================================================================================//
//                  General MCU API Function Prototypes
//======================================================================================//

// TODO: Change to nvic_...
void irq_config(irq_number_e irq_num, togglable_e toggle);
void irq_priority(irq_number_e irq_num, irq_priority_e irq_pri);

#endif
