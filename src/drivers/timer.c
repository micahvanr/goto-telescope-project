#include "timer.h"
#include "assert_handler.h"
#include "common.h"
#include "rcc.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

//========================================================//
//          Helper Function Prototypes
//========================================================//

static void timer_set(tim_reg_def *p_timx, uint32_t time, timer_unit_of_time_e unit);
static inline bus_types get_timer_bus(tim_reg_def const *const p_timx);
static inline void timer_clock_enable(tim_reg_def const *const p_timx);
static inline void timer_clock_disable(tim_reg_def const *const p_timx);
static void set_prescaler_count(tim_reg_def *p_timx, uint32_t time, timer_unit_of_time_e unit);
static inline bool overflow_check(uint32_t check_num, uint32_t factor);

//========================================================//
//          Global Variables
//========================================================//

//======================================================================================//
//                  Peripheral Function API Implementation
//======================================================================================//

// Features for General purpose timers:
// - Configure channels
// - Configure timers

void timer_init(void)
{
}

/***************************************************************************
Function: timer_delay_x 
Overview: Creates a delay for a given amount of time
Parameters: 
    seconds/ms/us: Time for delay
Return: 
    None
Note: None
***************************************************************************/
void timer_delay_sec(uint32_t seconds)
{
    timer_set(DELAY_TIMER, seconds, TIMER_UNIT_S);
    while (timer_read_status(DELAY_TIMER) != TIMER_UPDATE_FOUND);
    timer_reset_status(DELAY_TIMER);
}
void timer_delay_ms(uint32_t ms)
{
    timer_set(DELAY_TIMER, ms, TIMER_UNIT_MS);
    while (timer_read_status(DELAY_TIMER) != TIMER_UPDATE_FOUND);
    timer_reset_status(DELAY_TIMER);
}
void timer_delay_us(uint32_t us)
{
    timer_set(DELAY_TIMER, us, TIMER_UNIT_US);
    while (timer_read_status(DELAY_TIMER) != TIMER_UPDATE_FOUND);
    timer_reset_status(DELAY_TIMER);
}

/***************************************************************************
Function: timer_set_x
Overview: Sets the given TIMER peripheral to count to the given time
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
    seconds/ms/us: Time to set
Return: 
    None
Note: None
***************************************************************************/
void timer_set_sec(tim_reg_def *const p_timx, uint32_t seconds)
{
    timer_set(p_timx, seconds, TIMER_UNIT_S);
}

void timer_set_ms(tim_reg_def *const p_timx, uint32_t ms)
{
    timer_set(p_timx, ms, TIMER_UNIT_MS);
}

void timer_set_us(tim_reg_def *const p_timx, uint32_t us)
{
    // Cannot handle timing of 10 us
    if (us < 25) {
        ASSERT(false);
    }
    timer_set(p_timx, us, TIMER_UNIT_US);
}

/***************************************************************************
Function: timer_read_status
Overview: Reads the status update flag
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    timer_status_e: Status of timer:
    TIMER_UPDATE_NA    (0)
    TIMER_UPDATE_FOUND (1)
Note: None
***************************************************************************/
timer_status_e timer_read_status(tim_reg_def *const p_timx)
{
    return (timer_status_e)(p_timx->SR & TIM_SR_UIF);
}

/***************************************************************************
Function: timer_reset_status
Overview: Resets the status update flag
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    None
Note: None
***************************************************************************/
void timer_reset_status(tim_reg_def *const p_timx)
{
    p_timx->SR &= ~TIM_SR_UIF;
}

//======================================================================================//
//                  Helper Function Implementation
//======================================================================================//

static void timer_set(tim_reg_def *p_timx, uint32_t time, timer_unit_of_time_e unit)
{
    // Enable clock
    timer_clock_enable(p_timx);

    // Do prescaler and counter calculations
    set_prescaler_count(p_timx, time, unit);

    // Set ARR preload
    // (if preload is off, the count value will count to this new value immediately)
    // (if preload is on, the count value will count to the new value after finishing what its counting to now)
    p_timx->CR1 |= TIM_CR1_ARPE;

    // Disables one pulse mode
    p_timx->CR1 &= TIM_CR1_OPM;

    // Enable counter
    p_timx->CR1 |= TIM_CR1_CEN;
}

// TODO: Implement similar function in other peripherals?
static inline bus_types get_timer_bus(tim_reg_def const *const p_timx)
{
    // If TIMER peripheral on APB2
    if ((p_timx == TIM1) || (p_timx == TIM8) || (p_timx == TIM9) || (p_timx == TIM10) || (p_timx == TIM11)) {
        return APB2_BUS;

        // If TIMER peripheral on APB1
    } else {
        return APB1_BUS;
    }
}

static inline void timer_clock_enable(tim_reg_def const *const p_timx)
{

    switch (get_timer_bus(p_timx)) {

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

static inline void timer_clock_disable(tim_reg_def const *const p_timx)
{
    switch (get_timer_bus(p_timx)) {

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

static void set_prescaler_count(tim_reg_def *p_timx, uint32_t time, timer_unit_of_time_e unit)
{
    uint32_t clk_freq; // Represents clock frequency of what the clock would be according to new prescaler
    uint16_t prescaler = 1;
    uint16_t count     = 1;
    uint32_t unit_in_seconds;

    uint8_t const FACTOR_5 = 5;
    uint8_t const FACTOR_3 = 3;
    uint8_t const FACTOR_2 = 2;

    switch (unit) {
    case TIMER_UNIT_S:  unit_in_seconds = 1; break;
    case TIMER_UNIT_MS: unit_in_seconds = 1000; break;
    case TIMER_UNIT_US: unit_in_seconds = 1000000; break;
    }

    clk_freq = rcc_get_timer_clock_freq_hz(get_timer_bus(p_timx));

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
