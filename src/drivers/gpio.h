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

//  GPIO Handle Structure Possible Values
//=========================================//

typedef enum {
    GPIO_MODE_INPUT  = 0b00,
    GPIO_MODE_OUTPUT = 0b10,
    GPIO_MODE_ALT_FN = 0b01,
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
    GPIO_UNINITIALIZED = 0,
    GPIO_INITIALIZED   = 1,
} gpio_initialization_e;

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
