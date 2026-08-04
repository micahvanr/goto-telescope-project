#include "tim.h"
#include "assert_handler.h"
#include "common.h"
#include "rcc.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

//========================================================//
//          Helper Function Prototypes
//========================================================//

static void tim_set_core(tim_handler *p_tim_handler);
static inline bus_types get_tim_bus(tim_reg_def const *const p_timx);
static inline void tim_clock_enable(tim_reg_def const *const p_timx);
static inline void tim_clock_disable(tim_reg_def const *const p_timx);
static void set_prescaler_count(tim_reg_def *p_timx, uint32_t time, tim_unit_of_time_e unit);
static inline bool overflow_check(uint32_t check_num, uint32_t factor);

//========================================================//
//          Global Variables
//========================================================//

//======================================================================================//
//                  Peripheral Function API Implementation
//======================================================================================//

// Features for General purpose timers:
// Configure timers:
// - Configure count mode (upcounting / downcounting)
// - Configure one pulse mode
// - Do prescaler and counter calculations
// - Set ARR preload
//   - (if preload is off, the count value will count to this new value immediately)
//   - (if preload is on, the count value will count to the new value after finishing what its counting to now)
// - Clock selection (internal clock / external / internal trigger input)
//
// Configure channels (input capture / output compare):
// - Input capture:
//   - Set input filter (if needed)
//   - Set edge detection (rising / falling)
// - Output compare:
//   - Output mode (frozen / equal active / equal inactive / force active / force inactive / PWM 1 & 2)
//   - Preload
//   - Polarity
//   - Enable
//
// - Additional procedures:
// Enable clock in RCC
// Enable counter
//
// Possible Function APIs
// -

/***************************************************************************
Function: tim_reset 
Overview: Resets the given timer
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    None
Note: None
***************************************************************************/
void tim_reset(tim_reg_def const *const p_timx)
{
    tim_clock_disable(p_timx);
}
/***************************************************************************
Function: tim_delay 
Overview: Creates a delay for a given amount of time
Parameters: 
    time: Time for delay
    unit: Unit of time
        TIM_UNIT_S  
        TIM_UNIT_MS 
        TIM_UNIT_US 
Return: 
    None
Note: None
***************************************************************************/
void tim_delay(uint32_t time, tim_unit_of_time_e unit)
{
    static uint8_t initialized = false;
    tim_handler delay_handler  = {0};

    if (initialized == false) {
        delay_handler.p_timx                  = DELAY_TIMER;
        delay_handler.tim_conf.preload        = TIM_ARR_PRELOAD_EN;
        delay_handler.tim_conf.one_pulse_mode = TIM_ONE_PULSE_MODE_DI;
        delay_handler.tim_conf.clock_sel      = TIM_CLK_SEL_INTERNAL;
    }

    tim_set_auto(&delay_handler, time, unit);
    while (tim_read_status(delay_handler.p_timx) != TIM_UPDATE_FOUND);
    tim_reset_status(DELAY_TIMER);
}

/***************************************************************************
Function: tim_set_manual
Overview: Sets the given TIMER peripheral to count to the given time
Parameters: 
    p_tim_handle: Structure with the corresponding settings to set the TIMER peripheral
    auto_reload: Value that gets reloaded and counted to
    prescaler: Prescaler that divides the clock to the timer
Return: 
    None
Note: None
***************************************************************************/
void tim_set_manual(tim_handler *p_tim_handler, uint16_t auto_reload, uint16_t prescaler)
{
    // Enable clock
    tim_clock_enable(p_tim_handler->p_timx);

    // Do prescaler and counter calculations
    p_tim_handler->p_timx->ARR = auto_reload;
    p_tim_handler->p_timx->PSC = prescaler;

    tim_set_core(p_tim_handler);
}

/***************************************************************************
Function: tim_set_auto
Overview: Sets the given TIMER peripheral to count to the given time
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
    seconds/ms/us: Time to set
    unit: Unit of time
        TIM_UNIT_S  
        TIM_UNIT_MS 
        TIM_UNIT_US 
Return: 
    None
Note: None
***************************************************************************/
void tim_set_auto(tim_handler *p_tim_handler, uint32_t time, tim_unit_of_time_e unit)
{
    // Enable clock
    tim_clock_enable(p_tim_handler->p_timx);

    // Cannot handle timing of 25 us
    if ((time < 25) && (unit == TIM_UNIT_US)) {
        ASSERT(false);
    }

    // Do prescaler and counter calculations
    set_prescaler_count(p_tim_handler->p_timx, time, unit);

    tim_set_core(p_tim_handler);
}

/***************************************************************************
Function: tim_read_status
Overview: Reads the status update flag
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    tim_status_e: Status of timer:
    TIM_UPDATE_NA    (0)
    TIM_UPDATE_FOUND (1)
Note: None
***************************************************************************/
tim_status_e tim_read_status(tim_reg_def const *const p_timx)
{
    return (tim_status_e)(p_timx->SR & TIM_SR_UIF);
}

/***************************************************************************
Function: tim_reset_status
Overview: Resets the status update flag
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    None
Note: None
***************************************************************************/
void tim_reset_status(tim_reg_def *const p_timx)
{
    p_timx->SR &= ~TIM_SR_UIF;
}

//======================================================================================//
//                  Helper Function Implementation
//======================================================================================//

static void tim_set_core(tim_handler *p_tim_handler)
{
    // Set ARR preload
    // (if preload is off, the count value will count to this new value immediately)
    // (if preload is on, the count value will count to the new value after finishing what its counting to now)
    p_tim_handler->p_timx->CR1 |= p_tim_handler->tim_conf.preload << TIM_CR1_ARPE_POS;

    // One pulse mode
    p_tim_handler->p_timx->CR1 |= p_tim_handler->tim_conf.one_pulse_mode << TIM_CR1_OPM_POS;

    // TODO: Add clock selection

    // Enable counter
    p_tim_handler->p_timx->CR1 |= TIM_CR1_CEN;
}
// TODO: Implement similar function in other peripherals?
static inline bus_types get_tim_bus(tim_reg_def const *const p_timx)
{
    // If TIMER peripheral on APB2
    if ((p_timx == TIM1) || (p_timx == TIM8) || (p_timx == TIM9) || (p_timx == TIM10) || (p_timx == TIM11)) {
        return APB2_BUS;

        // If TIMER peripheral on APB1
    } else {
        return APB1_BUS;
    }
}

static inline void tim_clock_enable(tim_reg_def const *const p_timx)
{

    switch (get_tim_bus(p_timx)) {

    case APB1_BUS:
        RCC->APB1ENR |= (p_timx == TIM2)  ? RCC_APB1ENR_TIM2EN
                      : (p_timx == TIM3)  ? RCC_APB1ENR_TIM3EN
                      : (p_timx == TIM4)  ? RCC_APB1ENR_TIM4EN
                      : (p_timx == TIM5)  ? RCC_APB1ENR_TIM5EN
                      : (p_timx == TIM6)  ? RCC_APB1ENR_TIM6EN
                      : (p_timx == TIM7)  ? RCC_APB1ENR_TIM7EN
                      : (p_timx == TIM12) ? RCC_APB1ENR_TIM12EN
                      : (p_timx == TIM13) ? RCC_APB1ENR_TIM13EN
                      : (p_timx == TIM14) ? RCC_APB1ENR_TIM14EN
                                          : 0;
        break;

    case APB2_BUS:
        RCC->APB2ENR |= (p_timx == TIM1)  ? RCC_APB2ENR_TIM1EN
                      : (p_timx == TIM8)  ? RCC_APB2ENR_TIM8EN
                      : (p_timx == TIM9)  ? RCC_APB2ENR_TIM9EN
                      : (p_timx == TIM10) ? RCC_APB2ENR_TIM10EN
                      : (p_timx == TIM11) ? RCC_APB2ENR_TIM11EN
                                          : 0;
        break;

    default: ASSERT(false);
    }
}

static inline void tim_clock_disable(tim_reg_def const *const p_timx)
{
    switch (get_tim_bus(p_timx)) {

    case APB1_BUS:
        RCC->APB1RSTR |= (p_timx == TIM2)  ? RCC_APB1RSTR_TIM2RST
                       : (p_timx == TIM3)  ? RCC_APB1RSTR_TIM3RST
                       : (p_timx == TIM4)  ? RCC_APB1RSTR_TIM4RST
                       : (p_timx == TIM5)  ? RCC_APB1RSTR_TIM5RST
                       : (p_timx == TIM6)  ? RCC_APB1RSTR_TIM6RST
                       : (p_timx == TIM7)  ? RCC_APB1RSTR_TIM7RST
                       : (p_timx == TIM12) ? RCC_APB1RSTR_TIM12RST
                       : (p_timx == TIM13) ? RCC_APB1RSTR_TIM13RST
                       : (p_timx == TIM14) ? RCC_APB1RSTR_TIM14RST
                                           : 0;
        break;

    case APB2_BUS:
        RCC->APB2RSTR |= (p_timx == TIM1)  ? RCC_APB2RSTR_TIM1RST
                       : (p_timx == TIM8)  ? RCC_APB2RSTR_TIM8RST
                       : (p_timx == TIM9)  ? RCC_APB2RSTR_TIM9RST
                       : (p_timx == TIM10) ? RCC_APB2RSTR_TIM10RST
                       : (p_timx == TIM11) ? RCC_APB2RSTR_TIM11RST
                                           : 0;
        break;

    default: ASSERT(false);
    }
}

// Checks multiplication against an unsigned 16 bit variable
// Returns true if there is an overflow
static inline bool overflow_check(uint32_t check_num, uint32_t factor)
{
    if (check_num < (UINT16_MAX / factor)) {
        return false;
    } else {
        return true;
    }
}

static void set_prescaler_count(tim_reg_def *p_timx, uint32_t time, tim_unit_of_time_e unit)
{
    uint32_t clk_freq; // Represents clock frequency of what the clock would be according to new prescaler
    uint16_t prescaler = 1;
    uint16_t count     = 1;
    uint32_t unit_in_seconds;

    uint8_t const FACTOR_5 = 5;
    uint8_t const FACTOR_3 = 3;
    uint8_t const FACTOR_2 = 2;

    switch (unit) {
    case TIM_UNIT_S:  unit_in_seconds = 1; break;
    case TIM_UNIT_MS: unit_in_seconds = 1000; break;
    case TIM_UNIT_US: unit_in_seconds = 1000000; break;
    }

    clk_freq = rcc_get_timer_clock_freq_hz(get_tim_bus(p_timx));

    // Finding the highest prescaler value:
    // Increase the prescaler value until one of the following is true
    // - Dividing clock by new prescaler value results in decimal number
    // - The prescaler value is too large for 16 bits
    // - The new count value is 0
    // This series of operations will result in a high prescaler value while maintaining accuracy

    while (((clk_freq % FACTOR_5) == 0) && (((clk_freq / FACTOR_5) % unit_in_seconds) == 0)
           && !overflow_check(prescaler, FACTOR_5)) {
        clk_freq /= FACTOR_5;
        prescaler *= FACTOR_5;

        count = clk_freq / unit_in_seconds;
        count *= time;
    }

    while (((clk_freq % FACTOR_3) == 0) && (((clk_freq / FACTOR_3) % unit_in_seconds) == 0)
           && !overflow_check(prescaler, FACTOR_3)) {
        clk_freq /= FACTOR_3;
        prescaler *= FACTOR_3;

        count = clk_freq / unit_in_seconds;
        count *= time;
    }

    while (((clk_freq % FACTOR_2) == 0) && (((clk_freq / FACTOR_2) % unit_in_seconds) == 0)
           && !overflow_check(prescaler, FACTOR_2) && (((clk_freq / (FACTOR_2 * unit_in_seconds)) * time) > 1)) {
        clk_freq /= FACTOR_2;
        prescaler *= FACTOR_2;

        count = clk_freq / unit_in_seconds;
        count *= time;
    }

    count -= 1;
    prescaler -= 1;

    p_timx->PSC = prescaler;
    p_timx->ARR = count;
}
