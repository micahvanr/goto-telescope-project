#include "gpio.h"
#include "assert_handler.h"
#include "rcc.h"

/*****************************************************************
                        Helper Function Prototypes
*****************************************************************/

// Assert helper functions
static void gpio_init_asserts(gpio_handle const *const p_gpio_handle);

// General helper functions
static inline uint8_t map_gpio_ports_to_num(gpio_reg_def const *const p_gpiox);
static inline uint8_t map_exti_to_irq_num(exti_lines_e line_num);
static inline bool verify_pin_initialized(gpio_reg_def const *const p_gpio, pin_number_e pin_no);

static inline void gpio_clock_enable(gpio_reg_def const *const p_gpiox);
static inline void gpio_clock_disable(gpio_reg_def const *const p_gpiox);

/*****************************************************************
                        Global variables
*****************************************************************/

// This variable is used to check/set a pins initialzation status.
// It is an array with a length of 8 and each element is 16 bits wide.
// Each element in the array represents a port and each bit inside the variable represents a pin.
// If a bit in a specific element is 0, that means it has not been initialized. If it is a 1, it has been.
uint16_t g_gpio_pin_init[8] = {0};

/****************************************************************************************************
                                    Peripheral Function API Implementation
****************************************************************************************************/

/***************************************************************************
Function: gpio_init
Overview: Takes a given GPIO handle structure and configures the drivers appropriately 
Parameters:
    p_gpio_handle: Structure with the corresponding settings to configure the GPIO pin
Return: 
    None
Note: 
    The structure must be initialized with 0 before use to get the default value. 
        Otherwise be sure to set every member element to a value
***************************************************************************/
void gpio_init(gpio_handle *const p_gpio_handle)
{
    pin_number_e pin_no = p_gpio_handle->gpio_conf.pin_no;

    // Ensure handle structure values are valid
    gpio_init_asserts(p_gpio_handle);

    // Initialize gpio pin init structure
    g_gpio_pin_init[map_gpio_ports_to_num(p_gpio_handle->p_gpiox)] |= (1 << pin_no);

    // Enable RCC clock for peripheral port
    gpio_clock_enable(p_gpio_handle->p_gpiox);

    // Set alternate function mode
    // NOTE: Done before rest of config otherwise it may trigger undesired pin bouncing
    if (p_gpio_handle->gpio_conf.alt_fn_no != GPIO_ALT_FN_NA) {
        uint8_t afr_reg;
        afr_reg = pin_no / 8;
        p_gpio_handle->p_gpiox->AFR[afr_reg] |= (p_gpio_handle->gpio_conf.alt_fn_no << (4 * (pin_no % 8)));
    }

    // Set gpio mode
    p_gpio_handle->p_gpiox->MODER |= (p_gpio_handle->gpio_conf.mode << (2 * pin_no));

    // Set output type
    p_gpio_handle->p_gpiox->OTYPER |= (p_gpio_handle->gpio_conf.output_type << pin_no);

    // Set output speed
    p_gpio_handle->p_gpiox->OSPEEDR |= (p_gpio_handle->gpio_conf.output_speed << (2 * pin_no));

    // Set pull up or pull down resistors
    p_gpio_handle->p_gpiox->PUPDR |= (p_gpio_handle->gpio_conf.pullup_pulldown << (2 * pin_no));


    // Set interrupt mode
    if (p_gpio_handle->gpio_conf.it_trigger != GPIO_IT_NA) {
        gpio_it_config(p_gpio_handle, ENABLE);
    }
}

/***************************************************************************
Function: gpio_reset
Overview: Takes a given GPIO port and resets it using the RCC
Parameters:
    p_gpiox: Port register strucutre that needs to be reset
Return: 
    None
Note: None
***************************************************************************/
void gpio_reset(gpio_reg_def const *const p_gpiox)
{
    RCC->AHB1ENR |= (1 << map_gpio_ports_to_num(p_gpiox));
}

/***************************************************************************
Function: gpio_it_config
Overview: Takes a given GPIO handle structure with the pin number within it and enables or disables the interrupt for it
Parameters:
    p_gpio_handle: Structure with the corresponding settings to configure the GPIO port
    toggle: 
        ENABLE (1)
        DISABLE (0)
Return: 
    None
Note: None
***************************************************************************/
void gpio_it_config(gpio_handle const *const p_gpio_handle, togglable_e toggle)
{
    uint8_t pin_no             = p_gpio_handle->gpio_conf.pin_no;
    uint8_t exti_cr_reg_num    = pin_no / 4;
    exti_lines_e exti_line_num = (exti_lines_e)pin_no;

    switch (toggle) {
    case ENABLE:
        // Enable the SYSCFG clock in RCC
        RCC->APB2ENR |= RCC_APB2ENR_SYSCFG;

        // Enable the corresponding EXTI line in the EXTI control register of SYSCFG
        SYSCFG->EXTICR[exti_cr_reg_num] |= (map_gpio_ports_to_num(p_gpio_handle->p_gpiox) << (4 * (exti_line_num % 4)));

        // Enable the interrupt
        EXTI->IMR |= (1 << exti_line_num);

        switch (p_gpio_handle->gpio_conf.it_trigger) {
        case GPIO_IT_RT: EXTI->RTSR |= (1 << exti_line_num); break;
        case GPIO_IT_FT: EXTI->FTSR |= (1 << exti_line_num); break;
        case GPIO_IT_RFT:
            EXTI->RTSR |= (1 << exti_line_num);
            EXTI->FTSR |= (1 << exti_line_num);
            break;
        case GPIO_IT_NA: break;
        }
        irq_config(map_exti_to_irq_num(exti_line_num), ENABLE);
        break;

    case DISABLE:
        // Disable the corresponding EXTI line in the EXTI control register of SYSCFG
        SYSCFG->EXTICR[exti_cr_reg_num] &=
            ~(map_gpio_ports_to_num(p_gpio_handle->p_gpiox) << (4 * (exti_line_num % 4)));
        // Disable the rising edge and falling edge interrupts
        EXTI->RTSR &= ~(1 << exti_line_num);
        EXTI->FTSR &= ~(1 << exti_line_num);
        // Disable the interrupt in the processor
        irq_config(map_exti_to_irq_num(exti_line_num), DISABLE);
        break;
    }
}

/***************************************************************************
Function: gpio_it_handler
Overview: Function to be called inside application EXTI interrupt so it can clear the corresponding EXTI line
Parameters:
    exti_line: EXTI line that needs to be cleared
        EXTI_LINE_NO_x (0-22)
Return: 
    None
Note: None
***************************************************************************/
void gpio_it_handler(exti_lines_e exti_line)
{
    // Clears the pending bit for the EXTI line in the EXTI register
    EXTI->PR |= (1 << exti_line);
}

/***************************************************************************
Function: gpio_write
Overview: Sets a pin on a given port to high or low
Parameters:
    p_gpiox: Register structure for the GPIO port
    pin_no: Pin number for to be changed
        PIN_NO_x (0-15)
    pin_level: Pin logic level
        HIGH (1) 
        LOW (0)
Return: 
    None
Note: None
***************************************************************************/
void gpio_write(gpio_reg_def *p_gpiox, pin_number_e pin_no, pin_logic_level_e pin_level)
{
    ASSERT(verify_pin_initialized(p_gpiox, pin_no));
    switch (pin_level) {
    case HIGH: p_gpiox->ODR |= (1 << pin_no); break;
    case LOW:  p_gpiox->ODR &= ~(1 << pin_no); break;
    }
}

/***************************************************************************
Function: gpio_read
Overview: Reads a pin on a given port and returns high or low
Parameters:
    p_gpiox: Register structure for the GPIO port
        GPIOx (A-H)
    pin_no:
        PIN_NO_x (0-15)
Return: 
    pin_logic_level_e: Level of the port/pin read from
        HIGH (1) 
        LOW (0)
Note: None
***************************************************************************/
pin_logic_level_e gpio_read(gpio_reg_def const *p_gpiox, pin_number_e pin_no)
{
    ASSERT(verify_pin_initialized(p_gpiox, pin_no));
    return (pin_logic_level_e)(p_gpiox->IDR & (1 << pin_no));
}

/***************************************************************************
Function: gpio_toggle
Overview: Toggles a pin on a given port 
Parameters:
    p_gpiox: Register structure for the GPIO port
        GPIOx (A-H)
    pin_no:
        PIN_NO_x (0-15)
Return: 
    None
Note: None
***************************************************************************/
void gpio_toggle(gpio_reg_def *p_gpiox, pin_number_e pin_no)
{
    ASSERT(verify_pin_initialized(p_gpiox, pin_no));
    p_gpiox->ODR ^= (1 << pin_no);
}

/****************************************************************************************************
                                Helper Function Implementation
****************************************************************************************************/

static void gpio_init_asserts(gpio_handle const *const p_gpio_handle)
{
    uint8_t found_setting = false;

    // Peripheral check
    found_setting = (p_gpio_handle->p_gpiox == GPIOA) ? true
                  : (p_gpio_handle->p_gpiox == GPIOB) ? true
                  : (p_gpio_handle->p_gpiox == GPIOC) ? true
                  : (p_gpio_handle->p_gpiox == GPIOD) ? true
                  : (p_gpio_handle->p_gpiox == GPIOE) ? true
                  : (p_gpio_handle->p_gpiox == GPIOF) ? true
                  : (p_gpio_handle->p_gpiox == GPIOG) ? true
                  : (p_gpio_handle->p_gpiox == GPIOH) ? true
                                                      : false;

    // Alternate function check
    switch (p_gpio_handle->gpio_conf.alt_fn_no) {
    case GPIO_ALT_FN_0:  found_setting = true; break;
    case GPIO_ALT_FN_1:  found_setting = true; break;
    case GPIO_ALT_FN_2:  found_setting = true; break;
    case GPIO_ALT_FN_3:  found_setting = true; break;
    case GPIO_ALT_FN_4:  found_setting = true; break;
    case GPIO_ALT_FN_5:  found_setting = true; break;
    case GPIO_ALT_FN_6:  found_setting = true; break;
    case GPIO_ALT_FN_7:  found_setting = true; break;
    case GPIO_ALT_FN_8:  found_setting = true; break;
    case GPIO_ALT_FN_9:  found_setting = true; break;
    case GPIO_ALT_FN_10: found_setting = true; break;
    case GPIO_ALT_FN_11: found_setting = true; break;
    case GPIO_ALT_FN_12: found_setting = true; break;
    case GPIO_ALT_FN_13: found_setting = true; break;
    case GPIO_ALT_FN_14: found_setting = true; break;
    case GPIO_ALT_FN_15: found_setting = true; break;
    case GPIO_ALT_FN_NA: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Interrupt trigger check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.it_trigger) {
    case GPIO_IT_NA:  found_setting = true; break;
    case GPIO_IT_RT:  found_setting = true; break;
    case GPIO_IT_FT:  found_setting = true; break;
    case GPIO_IT_RFT: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Mode check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.mode) {
    case GPIO_MODE_INPUT:  found_setting = true; break;
    case GPIO_MODE_OUTPUT: found_setting = true; break;
    case GPIO_MODE_ALT_FN: found_setting = true; break;
    case GPIO_MODE_ANALOG: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Output speed check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.output_speed) {
    case GPIO_OSPEED_LOW:       found_setting = true; break;
    case GPIO_OSPEED_MEDIUM:    found_setting = true; break;
    case GPIO_OSPEED_FAST:      found_setting = true; break;
    case GPIO_OSPEED_VERY_FAST: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Output type check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.output_type) {
    case GPIO_OPTYPE_PUSH_PULL:  found_setting = true; break;
    case GPIO_OPTYPE_OPEN_DRAIN: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Pin number check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.pin_no) {
    case PIN_NO_0:  found_setting = true; break;
    case PIN_NO_1:  found_setting = true; break;
    case PIN_NO_2:  found_setting = true; break;
    case PIN_NO_3:  found_setting = true; break;
    case PIN_NO_4:  found_setting = true; break;
    case PIN_NO_5:  found_setting = true; break;
    case PIN_NO_6:  found_setting = true; break;
    case PIN_NO_7:  found_setting = true; break;
    case PIN_NO_8:  found_setting = true; break;
    case PIN_NO_9:  found_setting = true; break;
    case PIN_NO_10: found_setting = true; break;
    case PIN_NO_11: found_setting = true; break;
    case PIN_NO_12: found_setting = true; break;
    case PIN_NO_13: found_setting = true; break;
    case PIN_NO_14: found_setting = true; break;
    case PIN_NO_15: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Pullup/pulldown type check
    found_setting = false;
    switch (p_gpio_handle->gpio_conf.pullup_pulldown) {
    case GPIO_PUPD_NO:   found_setting = true; break;
    case GPIO_PULL_UP:   found_setting = true; break;
    case GPIO_PULL_DOWN: found_setting = true; break;
    }
    ASSERT(found_setting);
}

// Map GPIO ports to codes for setting GPIO interrupts and other functions
static inline uint8_t map_gpio_ports_to_num(gpio_reg_def const *const p_gpiox)
{
    return (p_gpiox == GPIOA) ? 0
         : (p_gpiox == GPIOB) ? 1
         : (p_gpiox == GPIOC) ? 2
         : (p_gpiox == GPIOD) ? 3
         : (p_gpiox == GPIOE) ? 4
         : (p_gpiox == GPIOF) ? 5
         : (p_gpiox == GPIOG) ? 6
         : (p_gpiox == GPIOH) ? 7
                              : 0;
}

// Map GPIO ports to codes for setting GPIO interrupts and other functions
static inline uint8_t map_exti_to_irq_num(exti_lines_e line_num)
{
    return (line_num == EXTI_LINE_NO_0)                                     ? EXTI0_IRQ_NO_6
         : (line_num == EXTI_LINE_NO_1)                                     ? EXTI1_IRQ_NO_7
         : (line_num == EXTI_LINE_NO_2)                                     ? EXTI2_IRQ_NO_8
         : (line_num == EXTI_LINE_NO_3)                                     ? EXTI3_IRQ_NO_9
         : (line_num == EXTI_LINE_NO_4)                                     ? EXTI3_IRQ_NO_9
         : ((line_num >= EXTI_LINE_NO_5) && (line_num <= EXTI_LINE_NO_9))   ? EXTI9_5_IRQ_NO_23
         : ((line_num >= EXTI_LINE_NO_10) && (line_num <= EXTI_LINE_NO_15)) ? EXTI15_10_IRQ_NO_40
                                                                            : 0;
}

static inline bool verify_pin_initialized(gpio_reg_def const *const p_gpio, pin_number_e pin_no)
{
    // Get port initialization pins from list
    uint16_t port_init = g_gpio_pin_init[map_gpio_ports_to_num(p_gpio)];
    // Mask the port to get the bit for the pin number
    uint8_t pin_initialized = ((port_init & (1 << pin_no)) >> pin_no);

    return ((pin_initialized == GPIO_INITIALIZED) ? true : false);
}

static inline void gpio_clock_enable(gpio_reg_def const *const p_gpiox)
{
    RCC->AHB1ENR |= (1 << map_gpio_ports_to_num(p_gpiox));
}

static inline void gpio_clock_disable(gpio_reg_def const *const p_gpiox)
{
    RCC->AHB1ENR &= ~(1 << map_gpio_ports_to_num(p_gpiox));
}
