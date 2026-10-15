//======================================================================================//{
//  File: stm32f4xx.h
//  Overview: Provide constants for the STM32F4xx MCu.
//  Features:
//  Notes: None
//======================================================================================//}

#ifndef STM32F4XX_H
#define STM32F4XX_H

//======================================================================================//}
//                  Public Constants
//======================================================================================//{

//} Address Definitions
//=========================================//{

typedef enum {
    APB1_BASE_ADDR = (0x40000000ul),
    APB2_BASE_ADDR = (0x40010000ul),
    AHB1_BASE_ADDR = (0x40020000ul),
    AHB2_BASE_ADDR = (0x50000000ul),
} bus_base_addr_e;

//} General Constants
//=========================================//{

typedef enum {
    PIN_NUM_0  = 0,
    PIN_NUM_1  = 1,
    PIN_NUM_2  = 2,
    PIN_NUM_3  = 3,
    PIN_NUM_4  = 4,
    PIN_NUM_5  = 5,
    PIN_NUM_6  = 6,
    PIN_NUM_7  = 7,
    PIN_NUM_8  = 8,
    PIN_NUM_9  = 9,
    PIN_NUM_10 = 10,
    PIN_NUM_11 = 11,
    PIN_NUM_12 = 12,
    PIN_NUM_13 = 13,
    PIN_NUM_14 = 14,
    PIN_NUM_15 = 15,
} pin_number_e;

typedef enum {
    HIGH = 1,
    LOW  = 0
} pin_logic_level_e;

typedef enum {
    ENABLE  = 1,
    DISABLE = 0,
} togglable_e;

typedef enum {
    AHB1_BUS = 0,
    AHB2_BUS = 1,
    APB1_BUS = 3,
    APB2_BUS = 4,
} bus_types;

//} Interrupt Constants
//=========================================//{

// Only adding IRQ numbers when they are needed.
typedef enum {
    WWDG_IRQ_NUM_0                = 0,
    PVDPVD_IRQ_NUM_1              = 1,
    TAMP_STAMP_IRQ_NUM_2          = 2,
    RTC_WKUP_IRQ_NUM_3            = 3,
    FLASH_IRQ_NUM_4               = 4,
    RCC_IRQ_NUM_5                 = 5,
    EXTI0_IRQ_NUM_6               = 6,
    EXTI1_IRQ_NUM_7               = 7,
    EXTI2_IRQ_NUM_8               = 8,
    EXTI3_IRQ_NUM_9               = 9,
    EXTI4_IRQ_NUM_10              = 10,
    DMA1_STREAM0_IRQ_NUM_11       = 11,
    DMA1_STREAM1_IRQ_NUM_12       = 12,
    DMA1_STREAM2_IRQ_NUM_13       = 13,
    DMA1_STREAM3_IRQ_NUM_14       = 14,
    DMA1_STREAM4_IRQ_NUM_15       = 15,
    DMA1_STREAM5_IRQ_NUM_16       = 16,
    DMA1_STREAM6_IRQ_NUM_17       = 17,
    ADC_IRQ_NUM_18                = 18,
    CAN1_TX_IRQ_NUM_19            = 19,
    CAN1_RX0_IRQ_NUM_20           = 20,
    CAN1_RX1_IRQ_NUM_21           = 21,
    CAN1_SCE_IRQ_NUM_22           = 22,
    EXTI9_5_IRQ_NUM_23            = 23,
    TIM1_BRK_TIM9_IRQ_NUM_24      = 24,
    TIM1_UP_TIM10_IRQ_NUM_25      = 25,
    TIM1_TRG_COM_TIM11_IRQ_NUM_26 = 26,
    TIM1_CC_IRQ_NUM_27            = 27,
    TIM2_IRQ_NUM_28               = 28,
    TIM3_IRQ_NUM_29               = 29,
    TIM4_IRQ_NUM_30               = 30,
    I2C1_EV_IRQ_NUM_31            = 31,
    I2C1_ER_IRQ_NUM_32            = 32,
    I2C2_EV_IRQ_NUM_33            = 33,
    I2C2_ER_IRQ_NUM_34            = 34,
    SPI1_IRQ_NUM_35               = 35,
    SPI2_IRQ_NUM_36               = 36,
    USART1_IRQ_NUM_37             = 37,
    USART2_IRQ_NUM_38             = 38,
    USART3_IRQ_NUM_39             = 39,
    EXTI15_10_IRQ_NUM_40          = 40,
    RTC_ALARM_IRQ_NUM_41          = 41,
    OTG_FS_WKUP_IRQ_NUM_42        = 42,
    TIM8_BRK_TIM12_IRQ_NUM_43     = 43,
    TIM8_UP_TIM13_IRQ_NUM_44      = 44,
    TIM8_TRG_COM_TIM14_IRQ_NUM_45 = 45,
    TIM8_CC_IRQ_NUM_46            = 46,
    DMA1_STREAM7_IRQ_NUM_47       = 47,
    FSMC_IRQ_NUM_48               = 48,
    SDIO_IRQ_NUM_49               = 49,
    TIM5_IRQ_NUM_50               = 50,
    SPI3_IRQ_NUM_51               = 51,
    UART4_IRQ_NUM_52              = 52,
    UART5_IRQ_NUM_53              = 53,
    TIM6_DAC_IRQ_NUM_54           = 54,
    TIM7_IRQ_NUM_55               = 55,
    DMA2_STREAM0_IRQ_NUM_56       = 56,
    DMA2_STREAM1_IRQ_NUM_57       = 57,
    DMA2_STREAM2_IRQ_NUM_58       = 58,
    DMA2_STREAM3_IRQ_NUM_59       = 59,
    DMA2_STREAM4_IRQ_NUM_60       = 60,
    ETH_IRQ_NUM_61                = 61,
    ETH_WKUP_IRQ_NUM_62           = 62,
    CAN2_TX_IRQ_NUM_63            = 63,
    CAN2_RX0_IRQ_NUM_64           = 64,
    CAN2_RX1_IRQ_NUM_65           = 65,
    CAN2_SCE_IRQ_NUM_66           = 66,
    OTG_FS_IRQ_NUM_67             = 67,
    DMA2_STREAM5_IRQ_NUM_68       = 68,
    DMA2_STREAM6_IRQ_NUM_69       = 69,
    DMA2_STREAM7_IRQ_NUM_70       = 70,
    USART6_IRQ_NUM_71             = 71,
    I2C3_EV_IRQ_NUM_72            = 72,
    I2C3_ER_IRQ_NUM_73            = 73,
    OTG_HS_EP1_OUT_IRQ_NUM_74     = 74,
    OTG_HS_EP1_IN_IRQ_NUM_75      = 75,
    OTG_HS_WKUP_IRQ_NUM_76        = 76,
    OTG_HS_IRQ_NUM_77             = 77,
    DCMI_IRQ_NUM_78               = 78,
    CRYP_IRQ_NUM_79               = 79,
    HASH_RNG_IRQ_NUM_80           = 80,
    FPU_IRQ_NUM_81                = 81,
} irq_number_e;

//} Hardware Constants
//=========================================//{

typedef enum {
    LED_GREEN_PIN   = PIN_NUM_12,
    LED_ORANGE_PIN  = PIN_NUM_13,
    LED_RED_PIN     = PIN_NUM_14,
    LED_BLUE_PIN    = PIN_NUM_15,
    USER_BUTTON_PIN = PIN_NUM_0,
} hardware_pin_assignment_e;

// Use constants because enums cannot have anything but a number as its value.
#define LED_GREEN_PORT  GPIOD
#define LED_ORANGE_PORT GPIOD
#define LED_RED_PORT    GPIOD
#define LED_BLUE_PORT   GPIOD

#define USER_BUTTON_PORT GPIOA

// Ports mapped to integers A->0, B->1, C->2... etc.
typedef enum {
    LED_GREEN_PORT_NUM  = 3U, // Port D
    LED_ORANGE_PORT_NUM = 3U,
    LED_RED_PORT_NUM    = 3U,
    LED_BLUE_PORT_NUM   = 3U,
} hardware_port_assignment_e;

#endif // STM32F4XX_H

// *End of file*
