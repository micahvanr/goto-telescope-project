#include "gpio.h"

/*****************************************************************
                        Helper Function Prototypes
*****************************************************************/
static inline uint8_t map_gpio_ports_to_num(gpio_reg_def const *const p_gpiox);
static inline uint8_t map_exti_to_irq_num(exti_lines_e line_num);
static inline bool_e gpio_verify_pin_initialized(gpio_reg_def const *const p_gpio, pin_number_e pin_no);
static inline gpio_modes_e gpio_get_pin_mode(gpio_reg_def const *const p_gpiox, pin_number_e pin_no);
static inline bool_e gpio_verify_pin_output(gpio_reg_def const *const p_gpiox, pin_number_e pin_no);
static inline bool_e gpio_verify_pin_input(gpio_reg_def const *const p_gpiox, pin_number_e pin_no);

static inline void gpio_clock_enable(gpio_reg_def const *const p_gpiox);

static inline void gpio_clock_disable(gpio_reg_def const *const p_gpiox);

/*****************************************************************
                        Global variables
*****************************************************************/
uint16_t g_gpio_pin_init[8] = {0};

/****************************************************************************************************
                                    Peripheral Function API Implementation
****************************************************************************************************/

/***************************************************************************
Function: gpio_init
Overview: Takes a given GPIO handle structure and configures the drivers appropriately 
Parameters:
    p_gpio_handle - Structure with the corresponding settings to configure the GPIO port
Return: 
    None
Note: None
***************************************************************************/
void gpio_init(gpio_handle *const p_gpio_handle)
{
    pin_number_e pin_no = p_gpio_handle->gpio_conf.pin_no;

    // TODO initialize gpio pin init structure
    g_gpio_pin_init[map_gpio_ports_to_num(p_gpio_handle->p_gpiox)] |= (1 << pin_no);

    // Enable RCC clock for peripheral port
    gpio_clock_enable(p_gpio_handle->p_gpiox);
    // Set gpio mode
    p_gpio_handle->p_gpiox->MODER |= (p_gpio_handle->gpio_conf.mode << (2 * pin_no));

    // Set output type
    p_gpio_handle->p_gpiox->OTYPER |= (p_gpio_handle->gpio_conf.output_type << pin_no);

    // Set output speed
    p_gpio_handle->p_gpiox->OSPEEDR |= (p_gpio_handle->gpio_conf.output_speed << (2 * pin_no));

    // Set pull up or pull down resistors
    p_gpio_handle->p_gpiox->PUPDR |= (p_gpio_handle->gpio_conf.pullup_pulldown << (2 * pin_no));

    // Set alternate function mode
    if (p_gpio_handle->gpio_conf.alt_fn_no != GPIO_ALT_FN_NA) {
        uint8_t afr_reg = pin_no % 8;
        p_gpio_handle->p_gpiox->AFR[afr_reg] |= (p_gpio_handle->gpio_conf.alt_fn_no << (4 * pin_no));
    }

    // Set interrupt mode
    if (p_gpio_handle->gpio_conf.it_trigger != GPIO_IT_NA) {
        gpio_it_config(p_gpio_handle, ENABLE);
    }
}

/***************************************************************************
Function: gpio_reset
Overview: Takes a given GPIO port and resets it using the RCC
Parameters:
    p_gpiox - Port register strucutre that needs to be reset
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
    p_gpiox - Port register strucutre that needs to be reset
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
        RCC->APB2ENR |= (1 << RCC_APB2ENR_SYSCFG_BIT);

        // Enable the corresponding EXTI line in the EXTI control register of SYSCFG
        SYSCFG->EXTICR[exti_cr_reg_num] |= (map_gpio_ports_to_num(p_gpio_handle->p_gpiox) << (4 * (exti_line_num % 4)));

        // Enable the interrupt
        EXTI->IMR |= (1 << exti_line_num);

        switch (p_gpio_handle->gpio_conf.it_trigger) {
        case GPIO_IT_RT:
            EXTI->RTSR |= (1 << exti_line_num);
            break;
        case GPIO_IT_FT:
            EXTI->FTSR |= (1 << exti_line_num);
            break;
        case GPIO_IT_RFT:
            EXTI->RTSR |= (1 << exti_line_num);
            EXTI->FTSR |= (1 << exti_line_num);
            break;
        case GPIO_IT_NA:
            break;
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
Function: gpio_handler
Overview: Function to be called inside application EXTI interrupt so it can clear the corresponding EXTI line
Parameters:
    exti_line - EXTI line that needs to be cleared
Return: 
    None
Note: None
***************************************************************************/
void gpio_handler(exti_lines_e exti_line)
{
    // Clears the pending bit for the EXTI line in the EXTI register
    EXTI->PR |= (1 << exti_line);
}

/***************************************************************************
Function: gpio_write
Overview: Sets a pin on a given port to high or low
Parameters:
    p_gpiox     - Register structure for the GPIO port
    pin_no      - Pin number for to be changed
    pin_level   - HIGH (1) or LOW (0)
Return: 
    None
Note: None
***************************************************************************/
void gpio_write(gpio_reg_def *p_gpiox, pin_number_e pin_no, pin_logic_level_e pin_level)
{
    gpio_verify_pin_initialized(p_gpiox, pin_no);
    switch (pin_level) {
    case HIGH:
        p_gpiox->ODR |= (1 << pin_no);
        break;
    case LOW:
        p_gpiox->ODR &= ~(1 << pin_no);
        break;
    }
}

/***************************************************************************
Function: gpio_read
Overview: Reads a pin on a given port and returns high or low
Parameters:
    p_gpiox - Register structure for the GPIO port
    pin_no  - Pin number to be read
Return: 
    pin_logic_level_e - HIGH (1) or LOW (0) of the given port and pin
Note: None
***************************************************************************/
pin_logic_level_e gpio_read(gpio_reg_def const *p_gpiox, pin_number_e pin_no)
{
    gpio_verify_pin_initialized(p_gpiox, pin_no);
    return (pin_logic_level_e)(p_gpiox->IDR & (1 << pin_no));
}

/***************************************************************************
Function: gpio_toggle
Overview: Toggles a pin on a given port 
Parameters:
    p_gpiox - Register structure for the GPIO port
    pin_no - Pin number to be toggled
Return: 
    None
Note: None
***************************************************************************/
void gpio_toggle(gpio_reg_def *p_gpiox, pin_number_e pin_no)
{
    ASSERT(gpio_verify_pin_initialized(p_gpiox, pin_no));
    p_gpiox->ODR ^= (1 << pin_no);
}

/****************************************************************************************************
                                Helper Function Implementation
****************************************************************************************************/

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
    return (line_num == EXTI_LINE_NO_0)                                     ? IRQ_NO_6_EXTI0
         : (line_num == EXTI_LINE_NO_1)                                     ? IRQ_NO_7_EXTI1
         : (line_num == EXTI_LINE_NO_2)                                     ? IRQ_NO_8_EXTI2
         : (line_num == EXTI_LINE_NO_3)                                     ? IRQ_NO_9_EXTI3
         : (line_num == EXTI_LINE_NO_4)                                     ? IRQ_NO_9_EXTI3
         : ((line_num >= EXTI_LINE_NO_5) && (line_num <= EXTI_LINE_NO_9))   ? IRQ_NO_23_EXTI9_5
         : ((line_num >= EXTI_LINE_NO_10) && (line_num <= EXTI_LINE_NO_15)) ? IRQ_NO_40_EXTI15_10
                                                                            : 0;
}

static inline bool_e gpio_verify_pin_initialized(gpio_reg_def const *const p_gpio, pin_number_e pin_no)
{
    // Get port initialization pins from list
    uint16_t port_init = g_gpio_pin_init[map_gpio_ports_to_num(p_gpio)];
    // Mask the port to get the bit for the pin number
    uint8_t pin_initialized = ((port_init & (1 << pin_no)) >> pin_no);

    return ((pin_initialized == GPIO_INITIALIZED) ? TRUE : FALSE);
}

// Potentially unused functions: gpio_get_pin_mode, gpio_verify_pin_output, gpio_verify_pin_input
static inline gpio_modes_e gpio_get_pin_mode(gpio_reg_def const *const p_gpiox, pin_number_e pin_no)
{
    uint32_t pin_mode_num = (2 * pin_no);
    gpio_modes_e pin_mode = ((p_gpiox->MODER & (0b11 << pin_mode_num)) >> pin_mode_num);
    switch (pin_mode) {
    case GPIO_MODE_INPUT:
        return GPIO_MODE_INPUT;
    case GPIO_MODE_OUTPUT:
        return GPIO_MODE_OUTPUT;
    case GPIO_MODE_ALT_FN:
        return GPIO_MODE_ALT_FN;
    case GPIO_MODE_ANALOG:
        return GPIO_MODE_ANALOG;
    }
    return GPIO_MODE_INPUT;
}

static inline bool_e gpio_verify_pin_output(gpio_reg_def const *const p_gpiox, pin_number_e pin_no)
{
    return (gpio_get_pin_mode(p_gpiox, pin_no) == GPIO_MODE_OUTPUT) ? TRUE : FALSE;
}

static inline bool_e gpio_verify_pin_input(gpio_reg_def const *const p_gpiox, pin_number_e pin_no)
{
    return (gpio_get_pin_mode(p_gpiox, pin_no) == GPIO_MODE_INPUT) ? TRUE : FALSE;
}

static inline void gpio_clock_enable(gpio_reg_def const *const p_gpiox)
{
    RCC->AHB1ENR |= (1 << map_gpio_ports_to_num(p_gpiox));
}

static inline void gpio_clock_disable(gpio_reg_def const *const p_gpiox)
{
    RCC->AHB1ENR &= ~(1 << map_gpio_ports_to_num(p_gpiox));
}
