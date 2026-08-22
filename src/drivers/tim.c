#include "tim.h"
#include "assert_handler.h"
#include "common.h"
#include "rcc.h"
#include "stm32f4xx.h"
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

//========================================================//
//          Helper Function Prototypes
//========================================================//

static inline void tim_clock_enable(tim_reg_def const *const p_timx);
static inline void tim_clock_disable(tim_reg_def const *const p_timx);
static void set_prescaler_count(tim_reg_def *p_timx, uint32_t period, tim_unit_of_time_e unit);
static inline bool overflow_check(tim_reg_def const *p_timx, uint32_t check_num, uint32_t factor);
static inline uint32_t get_ccr_pwm(tim_reg_def const *p_timx, uint32_t duty_cycle);
static tim_type_e get_tim_type(tim_reg_def *p_timx);

static void tim_base_init_asserts(tim_handler const *p_tim_handler);
static void tim_ic_init_asserts(tim_reg_def const *p_timx, tim_ic_config ic_config);
static void tim_oc_init_asserts(tim_reg_def const *p_timx, tim_oc_config oc_config);
static void tim_peri_assert(tim_reg_def const *p_timx);
static inline uint8_t map_tim_peri_to_num(tim_reg_def const *p_timx);
static inline void set_tim_base_init_status(tim_reg_def const *p_timx);
static inline tim_init_check_e get_tim_base_init_status(tim_reg_def const *p_timx);
static inline void set_tim_channel_init_status(tim_reg_def const *p_timx, tim_channel_sel_e channel);
static inline tim_init_check_e get_tim_channel_init_status(tim_reg_def const *p_timx, tim_channel_sel_e channel);

//========================================================//
//          Global Variables
//========================================================//
uint16_t g_tim_peri_init                       = 0;
uint8_t g_tim_channel_init[TIM_MAX_NUM_TIMERS] = {0};

//======================================================================================//
//                  Peripheral Function API Implementation
//======================================================================================//

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
Note: Care should be taken when using us unit as the delay can be too short and 
the software may not be able to keep up
***************************************************************************/
void tim_delay(uint32_t period, tim_unit_of_time_e unit)
{
    static uint8_t initialized = false;
    tim_handler delay_handler  = {0};

    if (initialized == false) {
        delay_handler.p_timx                  = DELAY_TIMER;
        delay_handler.tim_conf.preload        = TIM_ARR_PRELOAD_EN;
        delay_handler.tim_conf.one_pulse_mode = TIM_ONE_PULSE_MODE_DI;
        delay_handler.tim_conf.clock_sel      = TIM_CLK_SEL_INTERNAL;

        delay_handler.timing_conf.period = period;
        delay_handler.timing_conf.unit   = unit;
    }

    tim_init(&delay_handler);
    tim_start(delay_handler.p_timx);

    while (tim_read_base_status(delay_handler.p_timx) != TIM_UPDATE_FOUND);
    tim_reset_base_status(DELAY_TIMER);
}

/***************************************************************************
Function: tim_init
Overview: Initializes the TIMER peripheral inside the handle with the settings in the configuration structure
Parameters:
    p_tim_handle: Structure with the corresponding settings to configure the TIMER peripheral
Return: 
    None
Note: Check handler's substructures for default values
***************************************************************************/
void tim_init(tim_handler *p_tim_handler)
{
    tim_type_e timer_type;

    tim_base_init_asserts(p_tim_handler);

    set_tim_base_init_status(p_tim_handler->p_timx);

    timer_type = get_tim_type(p_tim_handler->p_timx);

    tim_clock_enable(p_tim_handler->p_timx);

    // One pulse mode
    p_tim_handler->p_timx->CR1 |= p_tim_handler->tim_conf.one_pulse_mode << TIM_CR1_OPM_POS;

    if ((timer_type == TIM_TYPE_ADVANCED) || (timer_type == TIM_TYPE_GENERAL_2_5)) {
        // Set direction
        p_tim_handler->p_timx->CR1 |= p_tim_handler->tim_conf.direction << TIM_CR1_DIR_POS;
        // Set align mode
        p_tim_handler->p_timx->CR1 |= p_tim_handler->tim_conf.align_mode << TIM_CR1_CMS_POS;
    }

    if ((timer_type == TIM_TYPE_ADVANCED) || (timer_type == TIM_TYPE_GENERAL_2_5)
        || (timer_type == TIM_TYPE_GENERAL_9_14)) {
        p_tim_handler->p_timx->SMCR |= p_tim_handler->tim_conf.clock_sel << TIM_SMCR_SMS_POS;
        if (p_tim_handler->tim_conf.clock_sel == TIM_CLK_SEL_EXTERNAL) {
            p_tim_handler->p_timx->SMCR |= p_tim_handler->tim_conf.trigger_sel << TIM_SMCR_TS_POS;
        }
    }

    // Set prescaler and auto reload value
    // Use manual ARR and PSC
    if ((p_tim_handler->timing_conf.auto_reload != 0) && (p_tim_handler->timing_conf.prescaler != 0)) {
        p_tim_handler->p_timx->PSC = p_tim_handler->timing_conf.prescaler - 1;
        p_tim_handler->p_timx->ARR = p_tim_handler->timing_conf.auto_reload - 1;
    // Calculate ARR and PSC from given period and unit
    } else if ((p_tim_handler->timing_conf.period != 0) && (p_tim_handler->timing_conf.unit != 0)) {
        set_prescaler_count(p_tim_handler->p_timx, p_tim_handler->timing_conf.period, p_tim_handler->timing_conf.unit);
    // Calculate ARR and PSC from given period and unit but convert from frequency first
    } else if (p_tim_handler->timing_conf.frequency_hz != 0) {
        uint32_t period;
        tim_unit_of_time_e unit;
        period = 1000000 / p_tim_handler->timing_conf.frequency_hz;
        unit = TIM_UNIT_US;
        set_prescaler_count(p_tim_handler->p_timx, period, unit);
    }

    // Set ARR preload
    // (if preload is off, the count value will count to this new value immediately)
    // (if preload is on, the count value will count to the new value after finishing what its counting to now)
    // This value is set after the prescaler and counter are set and only if a value has been set
    // otherwise a garbage value will be set in the PSC register
    if ((p_tim_handler->p_timx->PSC != 0) && (p_tim_handler->p_timx->ARR != 0)) {
        p_tim_handler->p_timx->CR1 |= p_tim_handler->tim_conf.preload << TIM_CR1_ARPE_POS;
    }
}

// Check each setting of the handle and ensure it is one of the available enum values
static void tim_base_init_asserts(tim_handler const *p_tim_handler)
{
    uint8_t found_setting = false;

    tim_peri_assert(p_tim_handler->p_timx);

    found_setting = false;
    switch (p_tim_handler->tim_conf.preload) {
    case TIM_ARR_PRELOAD_DI: found_setting = true; break;
    case TIM_ARR_PRELOAD_EN: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handler->tim_conf.one_pulse_mode) {
    case TIM_ONE_PULSE_MODE_DI: found_setting = true; break;
    case TIM_ONE_PULSE_MODE_EN: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handler->tim_conf.clock_sel) {
    case TIM_CLK_SEL_INTERNAL:         found_setting = true; break;
    case TIM_CLK_SEL_INTERNAL_TRIGGER: found_setting = true; break;
    case TIM_CLK_SEL_EXTERNAL:         found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handler->tim_conf.trigger_sel) {
    case TIM_TRIGGER_SEL_ITR0:    found_setting = true; break;
    case TIM_TRIGGER_SEL_ITR1:    found_setting = true; break;
    case TIM_TRIGGER_SEL_ITR2:    found_setting = true; break;
    case TIM_TRIGGER_SEL_ITR3:    found_setting = true; break;
    case TIM_TRIGGER_SEL_TI1F_ED: found_setting = true; break;
    case TIM_TRIGGER_SEL_TI1FP1:  found_setting = true; break;
    case TIM_TRIGGER_SEL_TI1FP2:  found_setting = true; break;
    case TIM_TRIGGER_SEL_ETRF:    found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handler->tim_conf.direction) {
    case TIM_CNT_DIR_UP:   found_setting = true; break;
    case TIM_CNT_DIR_DOWN: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handler->tim_conf.align_mode) {
    case TIM_ALIGN_EDGE:              found_setting = true; break;
    case TIM_ALIGN_CENTER_IT_DOWN:    found_setting = true; break;
    case TIM_ALIGN_CENTER_IT_UP:      found_setting = true; break;
    case TIM_ALIGN_CENTER_IT_DOWN_UP: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (p_tim_handler->timing_conf.unit) {
    case TIM_UNIT_S:  found_setting = true; break;
    case TIM_UNIT_MS: found_setting = true; break;
    case TIM_UNIT_US: found_setting = true; break;
    }
    ASSERT(found_setting);
}

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
Function: tim_start
Overview: Starts the counter on the given timer
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    None
Note: None
***************************************************************************/
void tim_start(tim_reg_def *p_timx)
{
    if (get_tim_base_init_status(p_timx) == TIM_NOT_INITIALIZED) {
        ASSERT(false);
    }
    p_timx->CR1 |= TIM_CR1_CEN;
}

/***************************************************************************
Function: tim_start_it
Overview: Starts the counter on the given timer with interrupts enabled
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    None
Note: None
***************************************************************************/
void tim_start_it(tim_reg_def *p_timx)
{
    if (get_tim_base_init_status(p_timx) == TIM_NOT_INITIALIZED) {
        ASSERT(false);
    }
    p_timx->DIER |= TIM_DIER_UIE;
    tim_it_config(p_timx, TIM_CHANNEL_NA, ENABLE);
    p_timx->CR1 |= TIM_CR1_CEN;
}

/***************************************************************************
Function: tim_channel_start
Overview: Enables the counter on the given timer
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    None
Note: None
***************************************************************************/
void tim_channel_start(tim_reg_def *p_timx, tim_channel_sel_e channel)
{
    uint8_t const ENABLE_REG_SHIFT = (channel * 4);

    if (get_tim_channel_init_status(p_timx, channel) == TIM_NOT_INITIALIZED) {
        ASSERT(false);
    }

    p_timx->CCER |= 1 << ENABLE_REG_SHIFT;
}

/***************************************************************************
Function: tim_channel_start
Overview: Enables the counter on the given timer
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    None
Note: None
***************************************************************************/
void tim_channel_start_it(tim_reg_def *p_timx, tim_channel_sel_e channel)
{
    uint8_t const ENABLE_REG_SHIFT = (channel * 4);

    if (get_tim_channel_init_status(p_timx, channel) == TIM_NOT_INITIALIZED) {
        ASSERT(false);
    }

    tim_it_config(p_timx, channel, ENABLE);
    p_timx->DIER |= TIM_DIER_CC1IE << channel;
    p_timx->CCER |= 1 << ENABLE_REG_SHIFT;
}

/***************************************************************************
Function: tim_it_config
Overview: Configures the given TIMER base peripheral and/or channel
Parameters: 
    p_timx: TIMER peripheral structure
        TIMx (1-14)
    channel: Channel to be configured
        TIM_CHANNEL_SEL_x (1-4)
        TIM_CHANNEL_NA
    toggle: 
        ENABLE [1]
        DISABLE [0]
Return: 
    None
Note: None
***************************************************************************/
void tim_it_config(tim_reg_def const *p_timx, tim_channel_sel_e channel, togglable_e toggle)
{
    if ((channel != TIM_CHANNEL_NA) && (p_timx == TIM1)) {
        irq_config(TIM1_CC_IRQ_NO_27, toggle);
        return;
    } else if ((channel != TIM_CHANNEL_NA) && (p_timx == TIM8)) {
        irq_config(TIM8_CC_IRQ_NO_46, toggle);
        return;
    }

    if (p_timx == TIM9) {
        irq_config(TIM1_BRK_TIM9_IRQ_NO_24, toggle);
    } else if ((p_timx == TIM1) || (p_timx == TIM10)) {
        irq_config(TIM1_UP_TIM10_IRQ_NO_25, toggle);
    } else if (p_timx == TIM11) {
        irq_config(TIM1_TRG_COM_TIM11_IRQ_NO_26, toggle);
    } else if (p_timx == TIM2) {
        irq_config(TIM2_IRQ_NO_28, toggle);
    } else if (p_timx == TIM3) {
        irq_config(TIM3_IRQ_NO_29, toggle);
    } else if (p_timx == TIM4) {
        irq_config(TIM4_IRQ_NO_30, toggle);
    } else if (p_timx == TIM12) {
        irq_config(TIM8_BRK_TIM12_IRQ_NO_43, toggle);
    } else if ((p_timx == TIM8) || (p_timx == TIM13)) {
        irq_config(TIM8_UP_TIM13_IRQ_NO_44, toggle);
    } else if (p_timx == TIM14) {
        irq_config(TIM8_TRG_COM_TIM14_IRQ_NO_45, toggle);
    } else if (p_timx == TIM7) {
        irq_config(TIM7_IRQ_NO_55, toggle);
    }
}

/***************************************************************************
Function: tim_ic_init 
Overview: Initializes the TIMER input capture with the settings in the configuration structure
Parameters: 
    p_timx: TIMER peripheral structure
        TIMx (1-14)
    ic_config: Input capture configuration settings
Return: 
    None
Note: None
***************************************************************************/
void tim_ic_init(tim_reg_def *p_timx, tim_ic_config ic_config)
{
    // Either % the channel num and start from 1 or dont and start channel num from 0
    uint8_t const MODE_REG         = ic_config.channel_num / 2;
    uint8_t const MODE_REG_SHIFT   = (ic_config.channel_num % 2) * 8;
    uint8_t const ENABLE_REG_SHIFT = (ic_config.channel_num * 4);

    tim_ic_init_asserts(p_timx, ic_config);

    set_tim_channel_init_status(p_timx, ic_config.channel_num);

    // Select active input
    p_timx->CCMR[MODE_REG] |= ic_config.input_map << MODE_REG_SHIFT;

    // Program filter
    p_timx->CCMR[MODE_REG] |= ic_config.filter << MODE_REG_SHIFT;

    // Select edge

    p_timx->CCER |= (ic_config.edge_detection << ENABLE_REG_SHIFT) << TIM_CCER_CC1P_POS;

    // Program input prescaler
    p_timx->CCMR[MODE_REG] |= (ic_config.prescaler << MODE_REG_SHIFT) << TIM_CCMR1_INPUT_ICPCS_POS;

    // Enable capture
    p_timx->CCER |= (1 << ENABLE_REG_SHIFT) << TIM_CCER_CC1E_POS;
}

// Check each setting of the config and ensure it is one of the available enum values
static void tim_ic_init_asserts(tim_reg_def const *p_timx, tim_ic_config ic_config)
{
    uint8_t found_setting;

    tim_peri_assert(p_timx);

    found_setting = false;
    switch (ic_config.channel_num) {
    case TIM_CHANNEL_SEL_1: found_setting = true; break;
    case TIM_CHANNEL_SEL_2: found_setting = true; break;
    case TIM_CHANNEL_SEL_3: found_setting = true; break;
    case TIM_CHANNEL_SEL_4: found_setting = true; break;
    case TIM_CHANNEL_NA:    found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (ic_config.dts_mult) {
    case TIM_DTS_FREQ_CLK_INT:        found_setting = true; break;
    case TIM_DTS_FREQ_CLK_INT_MULT_2: found_setting = true; break;
    case TIM_DTS_FREQ_CLK_INT_MULT_4: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (ic_config.input_map) {
    case TIM_INPUT_MAP_MAIN:         found_setting = true; break;
    case TIM_INPUT_MAP_TI_ALTERNATE: found_setting = true; break;
    case TIM_INPUT_MAP_TRC:          found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (ic_config.prescaler) {
    case TIM_IC_PRESCALER_NA:   found_setting = true; break;
    case TIM_IC_PRESCALER_2_EV: found_setting = true; break;
    case TIM_IC_PRESCALER_4_EV: found_setting = true; break;
    case TIM_IC_PRESCALER_8_EV: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (ic_config.filter) {
    case TIM_IC_FILTER_NA:            found_setting = true; break;
    case TIM_IC_FILTER_FCLKINT_N2:    found_setting = true; break;
    case TIM_IC_FILTER_FCLKINT_N4:    found_setting = true; break;
    case TIM_IC_FILTER_FCLKINT_N8:    found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV2_N6:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV2_N8:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV4_N6:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV4_N8:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV8_N6:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV8_N8:  found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV16_N5: found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV16_N6: found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV16_N8: found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV32_N5: found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV32_N6: found_setting = true; break;
    case TIM_IC_FILTER_FDTS_DIV32_N8: found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (ic_config.edge_detection) {
    case TIM_IC_EDGE_DETECTION_RISING:         found_setting = true; break;
    case TIM_IC_EDGE_DETECTION_FALLING:        found_setting = true; break;
    case TIM_IC_EDGE_DETECTION_RISING_FALLING: found_setting = true; break;
    }
    ASSERT(found_setting);
}

/***************************************************************************
Function: tim_oc_init 
Overview: Initializes the TIMER output compare with the settings in the configuration structure
Parameters: 
    p_timx: TIMER peripheral structure
        TIMx (1-14)
    oc_config: Output compare configuration settings
Return: 
    None
Note: None
***************************************************************************/
void tim_oc_init(tim_reg_def *p_timx, tim_oc_config oc_config)
{
    // Either % the channel num and start from 1 or dont and start channel num from 0
    uint8_t const MODE_REG         = oc_config.channel_num / 2;
    uint8_t const MODE_REG_SHIFT   = (oc_config.channel_num % 2) * 8;
    uint8_t const ENABLE_REG_SHIFT = (oc_config.channel_num * 4);

    tim_oc_init_asserts(p_timx, oc_config);

    set_tim_channel_init_status(p_timx, oc_config.channel_num);

    // Ensure channel in output mode
    p_timx->CCMR[MODE_REG] &= ~(TIM_CCMR1_OUTPUT_CC1S_MASK << MODE_REG_SHIFT);

    // Select oc mode
    p_timx->CCMR[MODE_REG] |= (oc_config.output_mode << MODE_REG_SHIFT) << TIM_CCMR1_OUTPUT_OC1M_POS;

    // Select output polarity
    p_timx->CCER |= (oc_config.polarity << ENABLE_REG_SHIFT) << TIM_CCER_CC1P_POS;

    // Select preload
    p_timx->CCMR[MODE_REG] |= (oc_config.arr_preload << MODE_REG_SHIFT) << TIM_CCMR1_OUTPUT_OC1PE_POS;

    // Set the CCR value manually
    if (oc_config.ccr_value != 0) {
        p_timx->CCR[oc_config.channel_num] = oc_config.ccr_value;
    }
    // Set CCR value based on PWM duty cycle
    else if (oc_config.duty_cycle != 0) {
        p_timx->CCR[oc_config.channel_num] = get_ccr_pwm(p_timx, oc_config.duty_cycle);
    }

    // Enable output
    p_timx->BDTR |= TIM_BDTR_MOE;
}

// Check each setting of the config and ensure it is one of the available enum values
static void tim_oc_init_asserts(tim_reg_def const *p_timx, tim_oc_config oc_config)
{
    uint8_t found_setting;
    tim_peri_assert(p_timx);

    found_setting = false;
    switch (oc_config.channel_num) {
    case TIM_CHANNEL_SEL_1: found_setting = true; break;
    case TIM_CHANNEL_SEL_2: found_setting = true; break;
    case TIM_CHANNEL_SEL_3: found_setting = true; break;
    case TIM_CHANNEL_SEL_4: found_setting = true; break;
    case TIM_CHANNEL_NA:    found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (oc_config.output_mode) {
    case TIM_OC_OPM_FROZEN:         found_setting = true; break;
    case TIM_OC_OPM_EQ_ACTIVE:      found_setting = true; break;
    case TIM_OC_OPM_EQ_INACTIVE:    found_setting = true; break;
    case TIM_OC_OPM_TOGGLE:         found_setting = true; break;
    case TIM_OC_OPM_FORCE_INACTIVE: found_setting = true; break;
    case TIM_OC_OPM_FORCE_ACTIVE:   found_setting = true; break;
    case TIM_OC_OPM_PWM_1:          found_setting = true; break;
    case TIM_OC_OPM_PWM_2:          found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (oc_config.polarity) {
    case TIM_OC_POL_ACTIVE_HIGH: found_setting = true; break;
    case TIM_OC_POL_ACTIVE_LOW:  found_setting = true; break;
    }
    ASSERT(found_setting);

    found_setting = false;
    switch (oc_config.arr_preload) {
    case TIM_ARR_PRELOAD_DI: found_setting = true; break;
    case TIM_ARR_PRELOAD_EN: found_setting = true; break;
    }
    ASSERT(found_setting);
}

/***************************************************************************
Function: tim_read_base_status
Overview: Reads the status update flag
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    tim_status_e: Status of timer:
    TIM_UPDATE_NA    [0]
    TIM_UPDATE_FOUND [1]
Note: None
***************************************************************************/
tim_status_e tim_read_base_status(tim_reg_def const *const p_timx)
{
    return (tim_status_e)(p_timx->SR & TIM_SR_UIF);
}

/***************************************************************************
Function: tim_reset_base_status
Overview: Resets the status update flag for the timer
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    None
Note: None
***************************************************************************/
void tim_reset_base_status(tim_reg_def *const p_timx)
{
    p_timx->SR &= ~TIM_SR_UIF;
}

/***************************************************************************
Function: tim_read_channel_status
Overview: Reads the status update flag
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
Return: 
    tim_status_e: Status of timer:
    TIM_UPDATE_NA    [0]
    TIM_UPDATE_FOUND [1]
Note: None
***************************************************************************/
tim_status_e tim_read_channel_status(tim_reg_def const *p_timx, tim_channel_sel_e channel)
{
    return (tim_status_e)((p_timx->SR & (TIM_SR_CC1IF << channel)) >> TIM_SR_CC1IF_POS);
}

/***************************************************************************
Function: tim_reset_base_status
Overview: Resets the status update flag for the given channel
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
    channel: Channel to be configured
        TIM_CHANNEL_SEL_x (1-4)
        TIM_CHANNEL_NA
Return: 
    None
Note: None
***************************************************************************/
void tim_reset_channel_status(tim_reg_def *const p_timx, tim_channel_sel_e channel)
{
    p_timx->SR &= ~(TIM_SR_CC1IF << channel);
}

/***************************************************************************
Function: tim_get_channel_ccr
Overview: Gets the given TIMER's channel CCR value
Parameters: 
    p_timx: TIMER peripheral
        TIMx (1-14)
    channel: Channel to be configured
        TIM_CHANNEL_SEL_x (1-4)
        TIM_CHANNEL_NA
Return: 
    uint32_t: CCR value of channel
Note: None
***************************************************************************/
uint32_t tim_get_channel_ccr(tim_reg_def *const p_timx, tim_channel_sel_e channel)
{
    return p_timx->CCR[channel];
}

//======================================================================================//
//                  Helper Function Implementation
//======================================================================================//

// TODO: Implement similar function in other peripherals?
bus_types get_tim_bus(tim_reg_def const *const p_timx)
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

static void set_prescaler_count(tim_reg_def *p_timx, uint32_t period, tim_unit_of_time_e unit)
{
    uint32_t clk_freq; // Represents clock frequency of what the clock would be according to new prescaler
    uint32_t prescaler = 1;
    uint32_t count     = 1;
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
    // - The prescaler value is too large for 16 bits (32 bits for TIM2/TIM5)
    // - The new count value is 0
    // This series of operations will result in a high prescaler value while maintaining accuracy

    while (((clk_freq % FACTOR_5) == 0) && (((clk_freq / FACTOR_5) % unit_in_seconds) == 0)
           && !overflow_check(p_timx, prescaler, FACTOR_5)) {
        clk_freq /= FACTOR_5;
        prescaler *= FACTOR_5;

        count = clk_freq / unit_in_seconds;
        count *= period;
    }

    while (((clk_freq % FACTOR_3) == 0) && (((clk_freq / FACTOR_3) % unit_in_seconds) == 0)
           && !overflow_check(p_timx, prescaler, FACTOR_3)) {
        clk_freq /= FACTOR_3;
        prescaler *= FACTOR_3;

        count = clk_freq / unit_in_seconds;
        count *= period;
    }

    while (((clk_freq % FACTOR_2) == 0) && (((clk_freq / FACTOR_2) % unit_in_seconds) == 0)
           && !overflow_check(p_timx, prescaler, FACTOR_2)
           && (((clk_freq / (FACTOR_2 * unit_in_seconds)) * period) > 1)) {
        clk_freq /= FACTOR_2;
        prescaler *= FACTOR_2;

        count = clk_freq / unit_in_seconds;
        count *= period;
    }

    count -= 1;
    prescaler -= 1;

    p_timx->PSC = prescaler;
    p_timx->ARR = count;
}

// Checks multiplication against an unsigned 16 bit variable
// Returns true if there is an overflow
static inline bool overflow_check(tim_reg_def const *p_timx, uint32_t check_num, uint32_t factor)
{
    uint32_t max;
    if ((p_timx == TIM2) || (p_timx == TIM5)) {
        max = UINT32_MAX;
    } else {
        max = UINT16_MAX;
    }
    if (check_num < (max / factor)) {
        return false;
    } else {
        return true;
    }
}

static inline uint32_t get_ccr_pwm(tim_reg_def const *p_timx, uint32_t duty_cycle)
{
    uint32_t arr_value = p_timx->ARR;
    uint32_t ccr;
    if ((arr_value + 1) < 100) {
        ASSERT(false);
    }
    ccr = ((arr_value + 1) / 100) * duty_cycle;
    return ccr;
}

static tim_type_e get_tim_type(tim_reg_def *p_timx)
{
    if ((p_timx == TIM6) || (p_timx == TIM7)) {
        return TIM_TYPE_BASIC;
    } else if ((p_timx == TIM2) || (p_timx == TIM3) || (p_timx == TIM4) || (p_timx == TIM5)) {
        return TIM_TYPE_GENERAL_2_5;
    } else if ((p_timx == TIM9) || (p_timx == TIM10) || (p_timx == TIM11) || (p_timx == TIM12) || (p_timx == TIM13)
               || (p_timx == TIM14)) {
        return TIM_TYPE_GENERAL_9_14;
    } else if ((p_timx == TIM1) || (p_timx == TIM8)) {
        return TIM_TYPE_ADVANCED;
    }
    UNUSED(p_timx);
    return 0;
}

static void tim_peri_assert(tim_reg_def const *p_timx)
{
    uint8_t found_setting = false;

    // Peripheral check
    found_setting = (p_timx == TIM1)  ? true
                  : (p_timx == TIM2)  ? true
                  : (p_timx == TIM3)  ? true
                  : (p_timx == TIM4)  ? true
                  : (p_timx == TIM5)  ? true
                  : (p_timx == TIM6)  ? true
                  : (p_timx == TIM7)  ? true
                  : (p_timx == TIM8)  ? true
                  : (p_timx == TIM9)  ? true
                  : (p_timx == TIM10) ? true
                  : (p_timx == TIM11) ? true
                  : (p_timx == TIM12) ? true
                  : (p_timx == TIM13) ? true
                  : (p_timx == TIM14) ? true
                                      : false;
    ASSERT(found_setting);
}

static inline uint8_t map_tim_peri_to_num(tim_reg_def const *p_timx)
{
    if (p_timx == TIM1) {
        return TIM1_INIT_NUM;
    } else if (p_timx == TIM2) {
        return TIM2_INIT_NUM;
    } else if (p_timx == TIM3) {
        return TIM3_INIT_NUM;
    } else if (p_timx == TIM4) {
        return TIM4_INIT_NUM;
    } else if (p_timx == TIM5) {
        return TIM5_INIT_NUM;
    } else if (p_timx == TIM6) {
        return TIM6_INIT_NUM;
    } else if (p_timx == TIM7) {
        return TIM7_INIT_NUM;
    } else if (p_timx == TIM8) {
        return TIM8_INIT_NUM;
    } else if (p_timx == TIM9) {
        return TIM9_INIT_NUM;
    } else if (p_timx == TIM10) {
        return TIM10_INIT_NUM;
    } else if (p_timx == TIM11) {
        return TIM11_INIT_NUM;
    } else if (p_timx == TIM12) {
        return TIM12_INIT_NUM;
    } else if (p_timx == TIM13) {
        return TIM13_INIT_NUM;
    } else if (p_timx == TIM14) {
        return TIM14_INIT_NUM;
    } else {
        ASSERT(0);
    }
    return 0;
}

// TODO: Change other drivers to follow this practice
static inline void set_tim_base_init_status(tim_reg_def const *p_timx)
{
    g_tim_peri_init |= (1 << map_tim_peri_to_num(p_timx));
}

static inline tim_init_check_e get_tim_base_init_status(tim_reg_def const *p_timx)
{
    return g_tim_peri_init & (1 << map_tim_peri_to_num(p_timx));
}

static inline void set_tim_channel_init_status(tim_reg_def const *p_timx, tim_channel_sel_e channel)
{
    g_tim_channel_init[map_tim_peri_to_num(p_timx)] |= (1 << channel);
}

static inline tim_init_check_e get_tim_channel_init_status(tim_reg_def const *p_timx, tim_channel_sel_e channel)
{
    return g_tim_channel_init[map_tim_peri_to_num(p_timx)] & (1 << channel);
}
