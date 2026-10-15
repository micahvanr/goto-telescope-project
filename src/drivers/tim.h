//======================================================================================//{
//  File: tim.h
//  Overview: Provides functions for initializing, configuring, and using the TIMER peripheral.
//  Features:
//      Base time keeping (w/ interrupt capability)
//      Input capture (w/ interrupt capability)
//      Output compare (w/ interrupt capability)
//  Notes:
//      Basic Timers: TIM6 & TIM7
//      General Purpose Timers: TIM2 - TIM5
//      General Purpose Timers: TIM9 - TIM14
//      Advanced Control Timers: TIM1 & TIM8
//======================================================================================//}

#ifndef TIM_H
#define TIM_H

//} Include Statements
//=========================================//{

#include "stm32f4xx.h"
#include <stdint.h>

//======================================================================================//}
//                  Public Constants
//======================================================================================//{

//} Address Definitions
//=========================================//{

typedef enum {
    TIM1_BASE_ADDR = ((APB2_BASE_ADDR) + (0x0000U)),

    TIM2_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0000U)),
    TIM3_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0400U)),
    TIM4_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0800U)),
    TIM5_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x0C00U)),
    TIM6_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x1000U)),
    TIM7_BASE_ADDR  = ((APB1_BASE_ADDR) + (0x1400U)),
    TIM12_BASE_ADDR = ((APB1_BASE_ADDR) + (0x1800U)),
    TIM13_BASE_ADDR = ((APB1_BASE_ADDR) + (0x1C00U)),
    TIM14_BASE_ADDR = ((APB1_BASE_ADDR) + (0x2000U)),

    TIM8_BASE_ADDR  = ((APB2_BASE_ADDR) + (0x0400U)),
    TIM9_BASE_ADDR  = ((APB2_BASE_ADDR) + (0x4000U)),
    TIM10_BASE_ADDR = ((APB2_BASE_ADDR) + (0x4400U)),
    TIM11_BASE_ADDR = ((APB2_BASE_ADDR) + (0x4800U)),
} tim_base_addr_e;

//} Config Options
//=========================================//{

// Numbers used to select register
typedef enum {
    TIM_CHANNEL_SEL_1 = 0,
    TIM_CHANNEL_SEL_2 = 1,
    TIM_CHANNEL_SEL_3 = 2,
    TIM_CHANNEL_SEL_4 = 3,
    TIM_CHANNEL_NA,
} tim_channel_sel_e;

typedef enum {
    TIM_ARR_PRELOAD_DI = 0b0, // Count value will count to this new value immediately.
    TIM_ARR_PRELOAD_EN = 0b1, // Count value will count to the new value after finishing what its counting to now.
} tim_arr_preload_e;

typedef enum {
    TIM_ONE_PULSE_MODE_DI = 0b0,
    TIM_ONE_PULSE_MODE_EN = 0b1,
} tim_one_pulse_mode_e;

typedef enum {
    TIM_CLK_SEL_INTERNAL         = 0b000,
    TIM_CLK_SEL_INTERNAL_TRIGGER = 0b110,
    TIM_CLK_SEL_EXTERNAL         = 0b111,
} tim_clock_sel_e;

typedef enum {
    TIM_TRIGGER_SEL_ITR0    = 0b000,
    TIM_TRIGGER_SEL_ITR1    = 0b001,
    TIM_TRIGGER_SEL_ITR2    = 0b010,
    TIM_TRIGGER_SEL_ITR3    = 0b011,
    TIM_TRIGGER_SEL_TI1F_ED = 0b100,
    TIM_TRIGGER_SEL_TI1FP1  = 0b101,
    TIM_TRIGGER_SEL_TI1FP2  = 0b110,
    TIM_TRIGGER_SEL_ETRF    = 0b111,
} tim_trigger_sel_e;

typedef enum {
    TIM_CNT_DIR_UP   = 0b0,
    TIM_CNT_DIR_DOWN = 0b1,
} tim_cnt_dir_e;

typedef enum {
    TIM_ALIGN_EDGE              = 0b00,
    TIM_ALIGN_CENTER_IT_DOWN    = 0b01,
    TIM_ALIGN_CENTER_IT_UP      = 0b10,
    TIM_ALIGN_CENTER_IT_DOWN_UP = 0b11,
} tim_align_mode_e;

//} tim_ic_config Options
//=========================================//{

// DTS = dead-time and sampling clock (derived from CR1->CKD).
typedef enum {
    TIM_DTS_FREQ_CLK_INT        = 0b00,
    TIM_DTS_FREQ_CLK_INT_MULT_2 = 0b01,
    TIM_DTS_FREQ_CLK_INT_MULT_4 = 0b10,
} tim_dts_mult_e;

typedef enum {
    TIM_INPUT_MAP_MAIN         = 0b01, // Input will match the channel number.
    TIM_INPUT_MAP_TI_ALTERNATE = 0b10, // Input will be the alternate channel number (either +- 1).
    TIM_INPUT_MAP_TRC          = 0b11,
} tim_input_map_type_e;

typedef enum {
    TIM_IC_PRESCALER_NA   = 0b00,
    TIM_IC_PRESCALER_2_EV = 0b01,
    TIM_IC_PRESCALER_4_EV = 0b10,
    TIM_IC_PRESCALER_8_EV = 0b11,
} tim_ic_prescaler_e;

typedef enum {
    TIM_IC_FILTER_NA            = 0b0000,
    TIM_IC_FILTER_FCLKINT_N2    = 0b0001,
    TIM_IC_FILTER_FCLKINT_N4    = 0b0010,
    TIM_IC_FILTER_FCLKINT_N8    = 0b0011,
    TIM_IC_FILTER_FDTS_DIV2_N6  = 0b0100,
    TIM_IC_FILTER_FDTS_DIV2_N8  = 0b0101,
    TIM_IC_FILTER_FDTS_DIV4_N6  = 0b0110,
    TIM_IC_FILTER_FDTS_DIV4_N8  = 0b0111,
    TIM_IC_FILTER_FDTS_DIV8_N6  = 0b1000,
    TIM_IC_FILTER_FDTS_DIV8_N8  = 0b1001,
    TIM_IC_FILTER_FDTS_DIV16_N5 = 0b1010,
    TIM_IC_FILTER_FDTS_DIV16_N6 = 0b1011,
    TIM_IC_FILTER_FDTS_DIV16_N8 = 0b1100,
    TIM_IC_FILTER_FDTS_DIV32_N5 = 0b1101,
    TIM_IC_FILTER_FDTS_DIV32_N6 = 0b1110,
    TIM_IC_FILTER_FDTS_DIV32_N8 = 0b1111,
} tim_ic_filter_e;

typedef enum {
    TIM_IC_EDGE_DETECTION_RISING         = 0b00,
    TIM_IC_EDGE_DETECTION_FALLING        = 0b01,
    TIM_IC_EDGE_DETECTION_RISING_FALLING = 0b101, // Special case where the bits are not adjacent to eachother.
} tim_ic_edge_detection_e;

//} tim_oc_config Options
//=========================================//{

// clang-format off
typedef enum {
    TIM_OC_OPM_FROZEN         = 0b000, // Comparison between output compare register and counter has no effect on OCxREF.
    TIM_OC_OPM_EQ_ACTIVE      = 0b001, // Sets OCxREF active level on match.
    TIM_OC_OPM_EQ_INACTIVE    = 0b010, // Sets OCxREF inactive level on match.
    TIM_OC_OPM_TOGGLE         = 0b011, // Toggles OCxREF when there is a match.
    TIM_OC_OPM_FORCE_INACTIVE = 0b100, // Forces OCxREF low.
    TIM_OC_OPM_FORCE_ACTIVE   = 0b101, // Forces OCxREF high.
    TIM_OC_OPM_PWM_1          = 0b110, // Upcounting: OCxREF is active when count < CCR, else it is inactive. Downcounting: Vice versa.
    TIM_OC_OPM_PWM_2          = 0b111, // Upcounting: OCxREF is inactive when count < CCR, else it is active. Downcounting: Vice versa.
} tim_oc_output_mode_e;
// clang-format on

typedef enum {
    TIM_OC_POL_ACTIVE_HIGH = 0,
    TIM_OC_POL_ACTIVE_LOW  = 1,
} tim_oc_polarity_e;

//} API Function Argument/Return Options
//=========================================//{

typedef enum {
    // Cannot start from 0. When initializing a value of 0 is assumed to be uninitialized
    TIM_UNIT_S = 1, // Seconds
    TIM_UNIT_MS,    // Miliseconds
    TIM_UNIT_US,    // Microseconds
    TIM_UNIT_HZ,
    TIM_UNIT_KHZ,
    TIM_UNIT_MHZ,
} tim_unit_of_time_e;

//} Callback Options
//=========================================//{

//} Interrupt Handler Constants
//=========================================//{

//} Misc. Constants
//=========================================//{

typedef enum {
    TIM_MAX_NUM_TIMERS = 14,
} tim_gen_info_e;

typedef enum {
    TIM_TYPE_BASIC,
    TIM_TYPE_GENERAL_2_5,
    TIM_TYPE_GENERAL_9_14,
    TIM_TYPE_ADVANCED,
} tim_type_e;

typedef enum {
    TIM_UPDATE_NA    = 0b0,
    TIM_UPDATE_FOUND = 0b1,
} tim_status_e;

//======================================================================================//}
//                  Structure Definitions
//======================================================================================//{

typedef volatile struct {
    uint32_t CR1;     // control register 1                                 Offset: 0x0
    uint32_t CR2;     // control register 2                                 Offset: 0x4
    uint32_t SMCR;    // slave mode control register                        Offset: 0x8
    uint32_t DIER;    // DMA/Interrupt enable register                      Offset: 0xC
    uint32_t SR;      // status register                                    Offset: 0x10
    uint32_t EGR;     // event generation register                          Offset: 0x14
    uint32_t CCMR[2]; // capture/compare mode register 1 & 2                Offset: 0x18
    uint32_t CCER;    // capture/compare enable register                    Offset: 0x20
    uint32_t CNT;     // counter                                            Offset: 0x24
    uint32_t PSC;     // prescaler                                          Offset: 0x28
    uint32_t ARR;     // auto-reload register                               Offset: 0x2C
    uint32_t RCR;     // repetition counter register                        Offset: 0x30
    uint32_t CCR[4];  // capture/compare register 1-4                       Offset: 0x34
    uint32_t BDTR;    // break and dead-time register                       Offset: 0x44
    uint32_t DCR;     // DMA control register                               Offset: 0x48
    uint32_t DMAR;    // DMA address for full transfer                      Offset: 0x4C
    uint32_t OR;      // TIM5 option register                               Offset: 0x50
} tim_reg_def;

// Base config settings
typedef struct {
    tim_arr_preload_e preload;           // Default: TIM_ARR_PRELOAD_DI
    tim_one_pulse_mode_e one_pulse_mode; // Default: TIM_ONE_PULSE_MODE_DI
    tim_clock_sel_e clock_sel;           // Default: TIM_CLK_SEL_INTERNAL
    tim_trigger_sel_e trigger_sel;       // Default: TIM_TRIGGER_SEL_ITR0
    tim_cnt_dir_e direction;             // Default: TIM_CNT_DIR_UP
    tim_align_mode_e align_mode;         // Default: TIM_ALIGN_EDGE
} tim_config;

// Base timer timing settings
typedef struct {
    uint32_t auto_reload;    // Default: 0
    uint32_t prescaler;      // Default: 0
    uint32_t time;           // Default: 0
    tim_unit_of_time_e unit; // Default: NA
} tim_timing_config;

// Base timer handler
typedef struct {
    tim_reg_def *p_timx;
    tim_config tim_conf;
    tim_timing_config timing_conf;
} tim_handle;

// Timer input capture channel settings
typedef struct {
    tim_channel_sel_e channel_num;          // Default: TIM_CHANNEL_SEL_1
    tim_dts_mult_e dts_mult;                // Default: TIM_DTS_FREQ_CLK_INT
    tim_input_map_type_e input_map;         // Default: TIM_INPUT_MAP_MAIN
    tim_ic_prescaler_e prescaler;           // Default: TIM_IC_PRESCALER_NA
    tim_ic_filter_e filter;                 // Default: TIM_IC_FILTER_NA
    tim_ic_edge_detection_e edge_detection; // Default: TIM_IC_EDGE_DETECTION_RISING
} tim_ic_config;

// Timer output compare channel settings
typedef struct {
    tim_channel_sel_e channel_num;    // Default: TIM_CHANNEL_SEL_1
    tim_oc_output_mode_e output_mode; // Default: TIM_OC_OPM_FROZEN
    tim_oc_polarity_e polarity;       // Default: TIM_OC_POL_ACTIVE_HIGH
    tim_arr_preload_e arr_preload;    // Default: TIM_ARR_PRELOAD_DI
    uint32_t duty_cycle;              // Default: 0
    uint32_t ccr_value;               // Default: 0
} tim_oc_config;

//======================================================================================//}
//                  Peripheral Structure Macros
//======================================================================================//{

#define TIM1  ((tim_reg_def *) TIM1_BASE_ADDR)
#define TIM2  ((tim_reg_def *) TIM2_BASE_ADDR)
#define TIM3  ((tim_reg_def *) TIM3_BASE_ADDR)
#define TIM4  ((tim_reg_def *) TIM4_BASE_ADDR)
#define TIM5  ((tim_reg_def *) TIM5_BASE_ADDR)
#define TIM6  ((tim_reg_def *) TIM6_BASE_ADDR)
#define TIM7  ((tim_reg_def *) TIM7_BASE_ADDR)
#define TIM8  ((tim_reg_def *) TIM8_BASE_ADDR)
#define TIM9  ((tim_reg_def *) TIM9_BASE_ADDR)
#define TIM10 ((tim_reg_def *) TIM10_BASE_ADDR)
#define TIM11 ((tim_reg_def *) TIM11_BASE_ADDR)
#define TIM12 ((tim_reg_def *) TIM12_BASE_ADDR)
#define TIM13 ((tim_reg_def *) TIM13_BASE_ADDR)
#define TIM14 ((tim_reg_def *) TIM14_BASE_ADDR)

//} Other Configuration
//=========================================//{
#define DELAY_TIMER TIM6

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

// Delay functions
void tim_delay(uint32_t time, tim_unit_of_time_e unit);

void tim_init(tim_handle *const p_tim_handle);
void tim_reset(tim_reg_def const *const p_timx);

void tim_ic_init(tim_reg_def *const p_timx, tim_ic_config ic_config);
void tim_oc_init(tim_reg_def *const p_timx, tim_oc_config oc_config);

void tim_start(tim_reg_def *const p_timx);
void tim_start_it(tim_reg_def *const p_timx);
void tim_channel_start(tim_reg_def *const p_timx, tim_channel_sel_e channel);
void tim_channel_start_it(tim_reg_def *const p_timx, tim_channel_sel_e channel);

void tim_config_it(tim_reg_def const *const p_timx, tim_channel_sel_e channel, togglable_e toggle);

tim_status_e tim_read_status(tim_reg_def const *const p_timx);
void tim_reset_status(tim_reg_def *const p_timx);
tim_status_e tim_read_channel_status(tim_reg_def const *const p_timx, tim_channel_sel_e channel);
void tim_reset_channel_status(tim_reg_def *const p_timx, tim_channel_sel_e channel);

uint32_t tim_get_channel_ccr(tim_reg_def *const p_timx, tim_channel_sel_e channel);
bus_types tim_get_bus(tim_reg_def const *const p_timx);

#endif // TIM_H

// *End of file*
