#ifndef STM32F4XX_H
#define STM32F4XX_H

#include "common.h"

// TODO: Think about changing comment sections with macros
//====================================================================//
//  Address Definitions
//====================================================================//

/****************************************************************************************************
                                        Address Definitions
****************************************************************************************************/

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

#ifdef MACRO
#define EXTI_BASE_ADDR   (0x40013C00ul)
#define SYSCFG_BASE_ADDR (0x40013800ul)

#define NVIC_ISER_BASE_ADDR (0xE000E100ul)
#define NVIC_ICER_BASE_ADDR (0xE000E180ul)
#define NVIC_ISPR_BASE_ADDR (0xE000E200ul)
#define NVIC_ICPR_BASE_ADDR (0xE000E280ul)
#define NVIC_IABR_BASE_ADDR (0xE000E300ul)
#define NVIC_IPR_BASE_ADDR  (0xE000E400ul)

#define APB1_BASE_ADDR (0x40000000ul)
#define APB2_BASE_ADDR (0x40010000ul)
#define AHB1_BASE_ADDR (0x40020000ul)
#define AHB2_BASE_ADDR (0x50000000ul)
#endif

/****************************************************************************************************
                                                Macros and Other Enums
****************************************************************************************************/

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
    EXTI0_IRQ_NO_6      = 6,
    EXTI1_IRQ_NO_7      = 7,
    EXTI2_IRQ_NO_8      = 8,
    EXTI3_IRQ_NO_9      = 9,
    EXTI4_IRQ_NO_10     = 10,
    EXTI9_5_IRQ_NO_23   = 23,
    I2C1_EV_IRQ_NO_31   = 31,
    I2C1_ER_IRQ_NO_32   = 32,
    I2C2_EV_IRQ_NO_33   = 33,
    I2C2_ER_IRQ_NO_34   = 34,
    USART1_IRQ_NO_37    = 37,
    USART2_IRQ_NO_38    = 38,
    USART3_IRQ_NO_39    = 39,
    EXTI15_10_IRQ_NO_40 = 40,
    UART4_IRQ_NO_52     = 52,
    UART5_IRQ_NO_53     = 53,
    USART6_IRQ_NO_71    = 71,
    I2C3_EV_IRQ_NO_72   = 72,
    I2C3_ER_IRQ_NO_73   = 73,
} irq_number_e;

// Lower number means higher priority
typedef enum {
    IRQ_PRIORITY_0,
    IRQ_PRIORITY_1,
    IRQ_PRIORITY_2,
    IRQ_PRIORITY_3,
    IRQ_PRIORITY_4,
    IRQ_PRIORITY_5,
    IRQ_PRIORITY_6,
    IRQ_PRIORITY_7,
    IRQ_PRIORITY_8,
    IRQ_PRIORITY_9,
    IRQ_PRIORITY_10,
    IRQ_PRIORITY_11,
    IRQ_PRIORITY_12,
    IRQ_PRIORITY_13,
    IRQ_PRIORITY_14,
    IRQ_PRIORITY_15,
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

/****************************************************************************************************
                                        Register Structure Definitions
****************************************************************************************************/

// clang-format off
typedef __vo struct {
	uint32_t IMR;				// Interrupt mask register											Offset: 0x00
	uint32_t EMR;				// Event mask register												Offset: 0x04
	uint32_t RTSR;				// Rising trigger selection register								Offset: 0x08
	uint32_t FTSR;				// Falling trigger selection register								Offset: 0x0C
	uint32_t SWIER;				// Software interrupt event register								Offset: 0x10
	uint32_t PR;				// Pending register													Offset: 0x14
} exti_reg_def;

typedef __vo struct {
	uint32_t MEMRMP;			// SYSCFG memory remap register										Offset: 0x00
	uint32_t PMC;				// SYSCFG peripheral mode configuration register 					Offset: 0x04
	uint32_t EXTICR[4];			// SYSCFG external interrupt configuration register 1-4				Offset: 0x08
	uint32_t RESERVED0;			// Reserved 0x18
	uint32_t RESERVED1;			// Reserved 0x1C
	uint32_t CMPCR;				// Compensation cell control register								Offset: 0x20
} syscfg_reg_def;

// clang-format on

/****************************************************************************************************
                                    Peripheral Structure Definitions
****************************************************************************************************/

#define EXTI   ((exti_reg_def *)EXTI_BASE_ADDR)
#define SYSCFG ((syscfg_reg_def *)SYSCFG_BASE_ADDR)

/****************************************************************************************************
                                    General MCU API Function Prototypes
****************************************************************************************************/

void irq_config(irq_number_e irq_num, togglable_e toggle);
void irq_priority(irq_number_e irq_num, irq_priority_e irq_pri);

#endif
