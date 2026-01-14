#ifndef STM32F4XX_H
#define STM32F4XX_H

#include <stdint.h>

/****************************************************************************************************
                                        Address Definitions
****************************************************************************************************/

#define RCC_BASE_ADDR    (0x40023800ul)
#define EXTI_BASE_ADDR   (0x40013C00ul)
#define SYSCFG_BASE_ADDR (0x40013800ul)

#define NVIC_ISER_BASE_ADDR (0xE000E100ul)
#define NVIC_ICER_BASE_ADDR (0xE000E180ul)
#define NVIC_ISPR_BASE_ADDR (0xE000E200ul)
#define NVIC_ICPR_BASE_ADDR (0xE000E280ul)
#define NVIC_IABR_BASE_ADDR (0xE000E300ul)
#define NVIC_IPR_BASE_ADDR  (0xE000E400ul)

/****************************************************************************************************
                                                Macros and Other Enums
****************************************************************************************************/

// Enums
typedef enum {
    PIN_NO_0 = 0,
    PIN_NO_1,
    PIN_NO_2,
    PIN_NO_3,
    PIN_NO_4,
    PIN_NO_5,
    PIN_NO_6,
    PIN_NO_7,
    PIN_NO_8,
    PIN_NO_9,
    PIN_NO_10,
    PIN_NO_11,
    PIN_NO_12,
    PIN_NO_13,
    PIN_NO_14,
    PIN_NO_15
} pin_number_e;

typedef enum {
    HIGH = 1,
    LOW  = 0
} pin_logic_level_e;

typedef enum {
    FALSE = 0,
    TRUE  = 1
} bool_e;

typedef enum {
    ENABLE  = 1,
    DISABLE = 0
} togglable_e;

// Only adding IRQ numbers when they are needed
typedef enum {
    IRQ_NO_6_EXTI0      = 6,
    IRQ_NO_7_EXTI1      = 7,
    IRQ_NO_8_EXTI2      = 8,
    IRQ_NO_9_EXTI3      = 9,
    IRQ_NO_10_EXTI4     = 10,
    IRQ_NO_23_EXTI9_5   = 23,
    IRQ_NO_40_EXTI15_10 = 40,
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
    RCC_APB2ENR_SYSCFG_BIT = 14
} rcc_apb2enr_bits_e;

// Macros

// Ports mapped to integers A->0, B->1, C->2... etc.
#define LED_GREEN_PIN   12u
#define LED_GREEN_PORT  3u
#define LED_ORANGE_PIN  13u
#define LED_ORANGE_PORT 3u
#define LED_RED_PIN     14u
#define LED_RED_PORT    3u
#define LED_BLUE_PIN    15u
#define LED_BLUE_PORT   3u

#define UNUSED(x) (void)(x)

/****************************************************************************************************
                                        Register Structure Definitions
****************************************************************************************************/

// clang-format off

// RCC Register Definition Structure
typedef volatile struct {
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

typedef volatile struct {
	uint32_t IMR;				// Interrupt mask register											Offset: 0x00
	uint32_t EMR;				// Event mask register												Offset: 0x04
	uint32_t RTSR;				// Rising trigger selection register								Offset: 0x08
	uint32_t FTSR;				// Falling trigger selection register								Offset: 0x0C
	uint32_t SWIER;				// Software interrupt event register								Offset: 0x10
	uint32_t PR;				// Pending register													Offset: 0x14
} exti_reg_def;

typedef volatile struct {
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

#define RCC    ((rcc_reg_def *)RCC_BASE_ADDR)
#define EXTI   ((exti_reg_def *)EXTI_BASE_ADDR)
#define SYSCFG ((syscfg_reg_def *)SYSCFG_BASE_ADDR)

/****************************************************************************************************
                                    General MCU API Function Prototypes
****************************************************************************************************/

void irq_config(irq_number_e irq_num, togglable_e toggle);
void irq_priority(irq_number_e irq_num, irq_priority_e irq_pri);

#include "../common/assert_handler.h"

#endif