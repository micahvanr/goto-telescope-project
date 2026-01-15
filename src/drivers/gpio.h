#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx.h"

/****************************************************************************************************
                                    Address Definitions
****************************************************************************************************/

#define GPIOA_BASE_ADDR (0x40020000ul)
#define GPIOB_BASE_ADDR (0x40020400ul)
#define GPIOC_BASE_ADDR (0x40020800ul)
#define GPIOD_BASE_ADDR (0x40020C00ul)
#define GPIOE_BASE_ADDR (0x40021000ul)
#define GPIOF_BASE_ADDR (0x40021400ul)
#define GPIOG_BASE_ADDR (0x40021800ul)
#define GPIOH_BASE_ADDR (0x40021C00ul)

/****************************************************************************************************
                                    Macros and Other Enums
****************************************************************************************************/

/*****                   GPIO Handle Structure Possible Values                    *****/

typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALT_FN,
    GPIO_MODE_ANALOG
} gpio_modes_e;

typedef enum {
    GPIO_IT_RT,
    GPIO_IT_FT,
    GPIO_IT_RFT,
    GPIO_IT_NA
} gpio_it_trigger_e;

typedef enum {
    GPIO_ALT_FN_0,
    GPIO_ALT_FN_1,
    GPIO_ALT_FN_2,
    GPIO_ALT_FN_3,
    GPIO_ALT_FN_4,
    GPIO_ALT_FN_5,
    GPIO_ALT_FN_6,
    GPIO_ALT_FN_7,
    GPIO_ALT_FN_8,
    GPIO_ALT_FN_9,
    GPIO_ALT_FN_10,
    GPIO_ALT_FN_11,
    GPIO_ALT_FN_12,
    GPIO_ALT_FN_13,
    GPIO_ALT_FN_14,
    GPIO_ALT_FN_15,
    GPIO_ALT_FN_NA
} gpio_alt_fn_e;

typedef enum {
    GPIO_OPTYPE_PUSH_PULL  = 0,
    GPIO_OPTYPE_OPEN_DRAIN = 1
} gpio_output_type_e;

typedef enum {
    GPIO_OSPEED_LOW       = 0,
    GPIO_OSPEED_MEDIUM    = 1,
    GPIO_OSPEED_FAST      = 2,
    GPIO_OSPEED_VERY_FAST = 3
} gpio_output_speed_e;

typedef enum {
    GPIO_NO_PUPD   = 0,
    GPIO_PULL_UP   = 1,
    GPIO_PULL_DOWN = 2
} gpio_pullup_pulldown_e;

typedef enum {
    GPIO_UNINITIALIZED = 0,
    GPIO_INITIALIZED   = 1
} gpio_initialization_e;

typedef enum {
    EXTI_LINE_NO_0,
    EXTI_LINE_NO_1,
    EXTI_LINE_NO_2,
    EXTI_LINE_NO_3,
    EXTI_LINE_NO_4,
    EXTI_LINE_NO_5,
    EXTI_LINE_NO_6,
    EXTI_LINE_NO_7,
    EXTI_LINE_NO_8,
    EXTI_LINE_NO_9,
    EXTI_LINE_NO_10,
    EXTI_LINE_NO_11,
    EXTI_LINE_NO_12,
    EXTI_LINE_NO_13,
    EXTI_LINE_NO_14,
    EXTI_LINE_NO_15,
    EXTI_LINE_NO_16,
    EXTI_LINE_NO_17,
    EXTI_LINE_NO_18,
    EXTI_LINE_NO_19,
    EXTI_LINE_NO_20,
    EXTI_LINE_NO_21,
    EXTI_LINE_NO_22,
} exti_lines_e;

/****************************************************************************************************
                                    Structure Definitions
****************************************************************************************************/

// Register definition
typedef volatile struct {
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
    gpio_initialization_e initialized;
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
void gpio_handler(exti_lines_e exti_line);

#endif