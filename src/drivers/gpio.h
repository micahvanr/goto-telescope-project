#ifndef GPIO_H
#define GPIO_H

#include "rcc.h"
#include "stm32f4xx.h"

/****************************************************************************************************
                                    Address Definitions
****************************************************************************************************/

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

#ifdef MACRO
#define GPIOA_BASE_ADDR ((AHB1_BASE_ADDR) + (0x0000u))
#define GPIOB_BASE_ADDR ((AHB1_BASE_ADDR) + (0x0400u))
#define GPIOC_BASE_ADDR ((AHB1_BASE_ADDR) + (0x0800u))
#define GPIOD_BASE_ADDR ((AHB1_BASE_ADDR) + (0x0C00u))
#define GPIOE_BASE_ADDR ((AHB1_BASE_ADDR) + (0x1000u))
#define GPIOF_BASE_ADDR ((AHB1_BASE_ADDR) + (0x1400u))
#define GPIOG_BASE_ADDR ((AHB1_BASE_ADDR) + (0x1800u))
#define GPIOH_BASE_ADDR ((AHB1_BASE_ADDR) + (0x1C00u))
#endif
/****************************************************************************************************
                                    Macros and Other Enums
****************************************************************************************************/

/*****                   GPIO Handle Structure Possible Values                    *****/

typedef enum {
    GPIO_MODE_INPUT  = 0, // Default
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_ALT_FN = 2,
    GPIO_MODE_ANALOG = 3,
} gpio_modes_e;

typedef enum {
    GPIO_IT_NA  = 0, // Default
    GPIO_IT_RT  = 1,
    GPIO_IT_FT  = 2,
    GPIO_IT_RFT = 3,
} gpio_it_trigger_e;

typedef enum {
    GPIO_ALT_FN_0  = 0, // Default
    GPIO_ALT_FN_1  = 1,
    GPIO_ALT_FN_2  = 2,
    GPIO_ALT_FN_3  = 3,
    GPIO_ALT_FN_4  = 4,
    GPIO_ALT_FN_5  = 5,
    GPIO_ALT_FN_6  = 6,
    GPIO_ALT_FN_7  = 7,
    GPIO_ALT_FN_8  = 8,
    GPIO_ALT_FN_9  = 9,
    GPIO_ALT_FN_10 = 10,
    GPIO_ALT_FN_11 = 11,
    GPIO_ALT_FN_12 = 12,
    GPIO_ALT_FN_13 = 13,
    GPIO_ALT_FN_14 = 14,
    GPIO_ALT_FN_15 = 15,
    GPIO_ALT_FN_NA,
} gpio_alt_fn_e;

typedef enum {
    GPIO_OPTYPE_PUSH_PULL  = 0, // Default
    GPIO_OPTYPE_OPEN_DRAIN = 1,
} gpio_output_type_e;

typedef enum {
    GPIO_OSPEED_LOW       = 0, // Default
    GPIO_OSPEED_MEDIUM    = 1,
    GPIO_OSPEED_FAST      = 2,
    GPIO_OSPEED_VERY_FAST = 3,
} gpio_output_speed_e;

typedef enum {
    GPIO_NO_PUPD   = 0, // Default
    GPIO_PULL_UP   = 1,
    GPIO_PULL_DOWN = 2,
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

/****************************************************************************************************
                                    Structure Definitions
****************************************************************************************************/

// Register definition
typedef __vo struct {
    uint32_t MODER;   // GPIO port mode register                  Offset: 0x00
    uint32_t OTYPER;  // GPIO port output type register           Offset: 0x04
    uint32_t OSPEEDR; // GPIO port output speed register          Offset: 0x08
    uint32_t PUPDR;   // GPIO port pull-up/pull-down register     Offset: 0x0C
    uint32_t IDR;     // GPIO port input data register            Offset: 0x10
    uint32_t ODR;     // GPIO port output data register           Offset: 0x14
    uint32_t BSRR;    // GPIO port bit set/reset register         Offset: 0x18
    uint32_t LCKR;    // GPIO port configuration lock register    Offset: 0x1C
    uint32_t AFR[2];  // GPIO alternate function register         Offset: 0x20
} gpio_reg_def;

// Configuration definition (used to initialize and configure gpio port settings)
typedef struct {
    pin_number_e pin_no;
    gpio_modes_e mode;
    gpio_output_type_e output_type;
    gpio_output_speed_e output_speed;
    gpio_pullup_pulldown_e pullup_pulldown;
    gpio_alt_fn_e alt_fn_no;
    gpio_it_trigger_e it_trigger;
} gpio_config;

// Handle definition (used to configure and handle the gpio features)
typedef struct {
    gpio_reg_def *p_gpiox;
    gpio_config gpio_conf;
} gpio_handle;

/*
Initialization structure. Each bit represents a pin and when it is zero it means the pin has not been initialized.
Otherwise a one means it has been initialized. This is used to ensure initialization of a pin before using it.
*/
typedef struct {
    uint16_t gpio_a_pin_init;
    uint16_t gpio_b_pin_init;
    uint16_t gpio_c_pin_init;
    uint16_t gpio_d_pin_init;
    uint16_t gpio_e_pin_init;
    uint16_t gpio_f_pin_init;
    uint16_t gpio_g_pin_init;
    uint16_t gpio_h_pin_init;
} gpio_pin_init;

/****************************************************************************************************
                                    Peripheral Structure Definitions
****************************************************************************************************/

#define GPIOA ((gpio_reg_def *)GPIOA_BASE_ADDR)
#define GPIOB ((gpio_reg_def *)GPIOB_BASE_ADDR)
#define GPIOC ((gpio_reg_def *)GPIOC_BASE_ADDR)
#define GPIOD ((gpio_reg_def *)GPIOD_BASE_ADDR)
#define GPIOE ((gpio_reg_def *)GPIOE_BASE_ADDR)
#define GPIOF ((gpio_reg_def *)GPIOF_BASE_ADDR)
#define GPIOG ((gpio_reg_def *)GPIOG_BASE_ADDR)
#define GPIOH ((gpio_reg_def *)GPIOH_BASE_ADDR)

/****************************************************************************************************
                                    Peripheral Function APIs
****************************************************************************************************/

void gpio_init(gpio_handle *const p_gpio_handle);
void gpio_reset(gpio_reg_def const *const p_gpiox);

void gpio_write(gpio_reg_def *p_gpiox, pin_number_e pin_no, pin_logic_level_e pin_level);
pin_logic_level_e gpio_read(gpio_reg_def const *p_gpiox, pin_number_e pin_no);
void gpio_toggle(gpio_reg_def *p_gpiox, pin_number_e pin_no);

void gpio_it_config(gpio_handle const *const p_gpio_handle, togglable_e toggle);
void gpio_it_handler(exti_lines_e exti_line);

#endif