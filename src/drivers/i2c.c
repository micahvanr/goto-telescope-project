#include "i2c.h"
#include "rcc.h"
#include "stm32f4xx.h"
#include "assert.h"

/*****************************************************************
                        Helper Function Prototypes
*****************************************************************/

static inline void verify_i2c_init_asserts(i2c_handle const *const p_i2c_handle);

static inline i2c_init_port_num_e map_i2c_ports_to_num(i2c_reg_def const *const p_i2cx);
static inline bool_e verify_i2c_initialized(i2c_reg_def *p_i2cx);
static void set_ccr_and_trise(i2c_handle const *const p_i2c_handle);

// static void core_transmit(i2c_reg_def *const p_i2cx, uint8_t const *p_data, uint32_t length);
static inline void generate_start_condition(i2c_reg_def *p_i2cx);
static inline void clear_start_flag(i2c_reg_def *p_i2cx);
static inline void send_address(i2c_reg_def *p_i2cx, uint8_t target_addr, i2c_read_write_sel_e read_write_sel);
static inline void clear_address_flag(i2c_reg_def *p_i2cx);
static inline void enable_ack(i2c_reg_def *p_i2cx);
static inline void disable_ack(i2c_reg_def *p_i2cx);

static inline void i2c_clock_enable(i2c_reg_def const *const p_i2cx);
static inline void i2c_clock_disable(i2c_reg_def const *const p_i2cx);

/*****************************************************************
                        Global variables
*****************************************************************/

uint8_t g_i2c_port_init = 0;

/****************************************************************************************************
                                    Peripheral Function API Implementation
****************************************************************************************************/

/***************************************************************************
Function: i2c_init
Overview: Initializes the I2C peripheral inside the handle with the settings in the configuration structure
Parameters:
    p_i2c_handle: Structure with the corresponding settings to configure the I2C peripheral
Return: 
    None
Note: 10-bit addressing not implemented
***************************************************************************/
void i2c_init(i2c_handle *const p_i2c_handle)
{
    // Ensure handle structure values are valid
    verify_i2c_init_asserts(p_i2c_handle);

    // Initialize the port in the init variable
    g_i2c_port_init |= (1 << (map_i2c_ports_to_num(p_i2c_handle->p_i2cx)));

    // Enable clock
    i2c_clock_enable(p_i2c_handle->p_i2cx);

    // Set peripheral clock frequency
    p_i2c_handle->p_i2cx->CR2 |= (rcc_get_bus_clock_freq_hz(APB1_BUS) / 1000000);

    // Set own address
    p_i2c_handle->p_i2cx->OAR1 |= (p_i2c_handle->i2c_conf.own_address << I2C_OAR1_ADD1_POS);

    // Configure CCR
    set_ccr_and_trise(p_i2c_handle);

    // Enable peripheral
    p_i2c_handle->p_i2cx->CR1 |= I2C_CR1_PE;
}

static inline void verify_i2c_init_asserts(i2c_handle const *const p_i2c_handle)
{
    uint8_t found_setting = FALSE;

    // Peripheral check
    found_setting = (p_i2c_handle->p_i2cx == I2C1) ? TRUE
                  : (p_i2c_handle->p_i2cx == I2C2) ? TRUE
                  : (p_i2c_handle->p_i2cx == I2C3) ? TRUE
                                                   : FALSE;
    ASSERT(found_setting);

    // Fast mode duty cycle check
    found_setting = FALSE;
    switch (p_i2c_handle->i2c_conf.fm_duty_cycle) {
    case I2C_FM_DUTY_2:    found_setting = TRUE; break;
    case I2C_FM_DUTY_16_9: found_setting = TRUE; break;
    }
    ASSERT(found_setting);

    // Speed mode check
    found_setting = FALSE;
    switch (p_i2c_handle->i2c_conf.speed_mode) {
    case I2C_SPEED_MODE_STANDARD: found_setting = TRUE; break;
    case I2C_SPEED_MODE_FAST:     found_setting = TRUE; break;
    }
    ASSERT(found_setting);

    // Clock frequency check
    found_setting = FALSE;
    switch (p_i2c_handle->i2c_conf.clock_freq_hz) {
    case I2C_CLK_FREQ_100KHZ: found_setting = TRUE; break;
    case I2C_CLK_FREQ_200KHZ: found_setting = TRUE; break;
    case I2C_CLK_FREQ_300KHZ: found_setting = TRUE; break;
    case I2C_CLK_FREQ_400KHZ: found_setting = TRUE; break;
    }
    ASSERT(found_setting);
}

/***************************************************************************
function: i2c_reset
overview: disables the clock for the given i2c peripheral, resetting it
parameters:
    p_i2cx: I2C register peripheral to be reset
return: 
    none
note: none
***************************************************************************/
void i2c_reset(i2c_reg_def const *const p_i2cx)
{
    i2c_clock_disable(p_i2cx);
}

/***************************************************************************
function: i2c_master_transmit
overview: takes the role of master and transmits data
parameters:
	p_i2cx: I2C peripheral used to communicate
	target_addr: Address of device to communicate to
	p_data: Pointer to data that will be sent. Can be dereferenced variable or list
	length: Length of data being sent
	repeated_start: Whether the transmission will end with a stop signal
		I2C_REPEATED_START_ENABLE
		I2C_REPEATED_START_DISABLE
return: 
    none
note: none
***************************************************************************/
void i2c_master_transmit(i2c_reg_def *const p_i2cx, uint8_t target_addr, uint8_t const *p_data, uint32_t length,
                         i2c_repeated_start_e repeated_start)
{
    ASSERT(verify_i2c_initialized(p_i2cx));

    generate_start_condition(p_i2cx);
    while (!(p_i2cx->SR1 & I2C_SR1_SB));

    clear_start_flag(p_i2cx);

    // Wait for address to be sent
    send_address(p_i2cx, target_addr, I2C_STATUS_MASTER_TX);
    while (!(p_i2cx->SR1 & I2C_SR1_ADDR));
    clear_address_flag(p_i2cx);

    // Send data
    for (uint32_t i = length; i > 0; i--) {
        while (!(p_i2cx->SR1 & I2C_SR1_TxE));
        p_i2cx->DR = *p_data;
        p_data++;
        // If last byte send stop
        if (i == 1) {
            while (!(p_i2cx->SR1 & I2C_SR1_TxE));
            while (!(p_i2cx->SR1 & I2C_SR1_BTF));
            if (repeated_start == I2C_REPEATED_START_DISABLE) {
                generate_stop_condition(p_i2cx);
            }
        }
    }
}

/***************************************************************************
function: i2c_master_receive
overview: takes the role of master and receives data
parameters:
	p_i2cx: I2C peripheral used to communicate
	target_addr: Address of device to communicate to
	p_data: Pointer to data that will be recieved. Can be dereferenced variable or list
	length: Length of data being sent
	repeated_start: Whether the transmission will end with a stop signal
		I2C_REPEATED_START_ENABLE
		I2C_REPEATED_START_DISABLE
return: 
    none
note: none
***************************************************************************/
void i2c_master_receive(i2c_reg_def *const p_i2cx, uint8_t target_addr, uint8_t *p_data, uint32_t length,
                        i2c_repeated_start_e repeated_start)
{
    ASSERT(verify_i2c_initialized(p_i2cx));

    enable_ack(p_i2cx);

    generate_start_condition(p_i2cx);

    while (!(p_i2cx->SR1 & I2C_SR1_SB));
    clear_start_flag(p_i2cx);

    send_address(p_i2cx, target_addr, I2C_STATUS_MASTER_RX);
    while (!(p_i2cx->SR1 & I2C_SR1_ADDR));

    if (length == 1) {
        disable_ack(p_i2cx);
    }

    clear_address_flag(p_i2cx);

    for (uint32_t i = length; i > 0; i--) {
        if (i == 1) {
            disable_ack(p_i2cx);
        }
        while (!(p_i2cx->SR1 & I2C_SR1_RxNE));
        *p_data = p_i2cx->DR;
        p_data++;

        if ((i == 1) && (repeated_start == I2C_REPEATED_START_DISABLE)) {
            generate_stop_condition(p_i2cx);
        }
    }
}

/***************************************************************************
function: i2c_slave_transmit
overview: takes the role of slave and transmits data
parameters:
	p_i2cx: I2C peripheral used to communicate
	target_addr: Address of device to communicate to
	p_data: Pointer to data that will be transmitted. Can be dereferenced variable or list
	length: Length of data being sent
return: 
    none
note: none
***************************************************************************/
void i2c_slave_transmit(i2c_reg_def *const p_i2cx, uint8_t const *p_data, uint32_t length)
{
    ASSERT(verify_i2c_initialized(p_i2cx));

    enable_ack(p_i2cx);

    // Wait for ADDR flag and clear it
    while (!(p_i2cx->SR1 & I2C_SR1_ADDR));
    clear_address_flag(p_i2cx);

    // Send data
    for (uint32_t i = length; i > 0; i--) {
        while (!(p_i2cx->SR1 & I2C_SR1_TxE));
        p_i2cx->DR = *p_data;
        p_data++;
    }
    // Clear ack failure
    p_i2cx->SR1 &= ~I2C_SR1_AF;

    // Turn acking off
    p_i2cx->CR1 &= ~I2C_CR1_ACK;
}

/***************************************************************************
function: i2c_slave_receive
overview: takes the role of slave and receives data
parameters:
	p_i2cx: I2C peripheral used to communicate
	target_addr: Address of device to communicate to
	p_data: Pointer to data that will be recieved. Can be dereferenced variable or list
	length: Length of data being sent
return: 
    none
note: none
***************************************************************************/
void i2c_slave_receive(i2c_reg_def *const p_i2cx, uint8_t *p_data, uint32_t length)
{
    uint8_t __vo dummy_action;

    ASSERT(verify_i2c_initialized(p_i2cx));
    enable_ack(p_i2cx);

    // Wait for ADDR flag and clear it
    while (!(p_i2cx->SR1 & I2C_SR1_ADDR));
    clear_address_flag(p_i2cx);

    // Receive data
    for (uint32_t i = length; i > 0; i--) {
        while (!(p_i2cx->SR1 & I2C_SR1_RxNE));
        *p_data = p_i2cx->DR;
        p_data++;
    }

    // Wait until STOPF flag is set and clear it
    while (!(p_i2cx->SR1 & I2C_SR1_STOPF));
    dummy_action = p_i2cx->SR1;
    p_i2cx->CR1 |= 0;
    UNUSED(dummy_action);

    // Turn acking off
    p_i2cx->CR1 &= ~I2C_CR1_ACK;
}

// Write/read interrupt
void i2c_master_transmit_it(i2c_handle *p_i2c_handle, uint8_t target_addr, uint8_t *p_data, uint32_t length,
                            i2c_repeated_start_e repeated_start)
{
	UNUSED(p_i2c_handle);
	UNUSED(target_addr);
	UNUSED(p_data);
	UNUSED(length);
	UNUSED(repeated_start);
}
void i2c_master_receive_it(i2c_handle *p_i2c_handle, uint8_t target_addr, uint8_t *p_data, uint32_t length,
                           i2c_repeated_start_e repeated_start)
{
	UNUSED(p_i2c_handle);
	UNUSED(target_addr);
	UNUSED(p_data);
	UNUSED(length);
	UNUSED(repeated_start);
}

void i2c_slave_transmit_it(i2c_handle *p_i2c_handle, uint8_t const *p_data, uint32_t length)
{
	UNUSED(p_i2c_handle);
	UNUSED(p_data);
	UNUSED(length);
}
void i2c_slave_receive_it(i2c_handle *p_i2c_handle, uint8_t *p_data, uint32_t length)
{
	UNUSED(p_i2c_handle);
	UNUSED(p_data);
	UNUSED(length);
}
void i2c_transmit_it(i2c_handle *p_i2c_handle, uint8_t *p_data, uint32_t const length)
{
    UNUSED(p_i2c_handle);
    UNUSED(p_data);
    UNUSED(length);
}
void i2c_receive_it(i2c_handle *p_i2c_handle, uint8_t *p_data, uint32_t const length)
{
    UNUSED(p_i2c_handle);
    UNUSED(p_data);
    UNUSED(length);
}

// Interrupt handling
void i2c_it_config(i2c_reg_def const *const p_i2cx, togglable_e toggle)
{
    UNUSED(p_i2cx);
    UNUSED(toggle);
}


void i2c_it_handler(i2c_handle *const p_i2c_handle)
{
    UNUSED(p_i2c_handle);
}

/****************************************************************************************************
                                Helper Function Implementation
****************************************************************************************************/

static inline i2c_init_port_num_e map_i2c_ports_to_num(i2c_reg_def const *const p_i2cx)
{
    if (p_i2cx == I2C1) {
        return I2C1_INIT_NUM;
    } else if (p_i2cx == I2C2) {
        return I2C2_INIT_NUM;
    } else if (p_i2cx == I2C3) {
        return I2C3_INIT_NUM;
    } else {
        ASSERT(0);
    }
    return 0;
}

static inline bool_e verify_i2c_initialized(i2c_reg_def *p_i2cx)
{
    if (g_i2c_port_init & (1 << map_i2c_ports_to_num(p_i2cx))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void set_ccr_and_trise(i2c_handle const *const p_i2c_handle)
{
    uint32_t ccr_value            = 0;
    uint32_t trise_value          = 0;
    uint32_t desired_clock_khz    = p_i2c_handle->i2c_conf.clock_freq_hz / 1000;
    uint32_t peripheral_clock_khz = (rcc_get_bus_clock_freq_hz(APB1_BUS) / 1000);

    // Speed configuration
    p_i2c_handle->p_i2cx->CCR |= (p_i2c_handle->i2c_conf.speed_mode << I2C_CCR_FS_POS);

    switch (p_i2c_handle->i2c_conf.speed_mode) {
    case I2C_SPEED_MODE_STANDARD:
        // Peripheral clock speed must be at leats 2Mhz in standard mode
        if (peripheral_clock_khz < 2000) {
            ASSERT(FALSE);
        }

        // Set CCR
        ccr_value = ((peripheral_clock_khz) / (2 * (desired_clock_khz)));

        // Set time rise value
        trise_value |= ((I2C_MAX_RISE_SM_1000_NS / (1000000 / (peripheral_clock_khz))) + 1);
        break;

    case I2C_SPEED_MODE_FAST:
        // Peripheral clock speed must be at leats 4Mhz in fast mode
        if (peripheral_clock_khz < 4000) {
            ASSERT(FALSE);
        }

        // Set duty cycle
        p_i2c_handle->p_i2cx->CCR |= (p_i2c_handle->i2c_conf.fm_duty_cycle << I2C_CCR_DUTY_POS);

        // Set CCR
        switch (p_i2c_handle->i2c_conf.fm_duty_cycle) {
        case I2C_FM_DUTY_2:    ccr_value = ((peripheral_clock_khz) / (3 * (desired_clock_khz))); break;
        case I2C_FM_DUTY_16_9: ccr_value = ((peripheral_clock_khz) / (25 * (desired_clock_khz))); break;
        }
        break;

        // Set TRISE
        trise_value |= ((I2C_MAX_RISE_FM_300_NS / (1000000 / (peripheral_clock_khz))) + 1);
    }
    p_i2c_handle->p_i2cx->TRISE = (trise_value & I2C_TRISE_MASK);
    p_i2c_handle->p_i2cx->CCR |= ccr_value;
}

static inline void generate_start_condition(i2c_reg_def *p_i2cx)
{
    // Sends start and waits until SB is set
    p_i2cx->CR1 |= I2C_CR1_START;
}

static inline void generate_stop_condition(i2c_reg_def *p_i2cx)
{
    p_i2cx->CR1 |= I2C_CR1_STOP;
}
static inline void clear_start_flag(i2c_reg_def *p_i2cx)
{
    uint32_t __vo dummy_read;
    dummy_read = p_i2cx->SR1;
    UNUSED(dummy_read);
}

static inline void send_address(i2c_reg_def *p_i2cx, uint8_t target_addr, i2c_status_e i2c_status)
{
    // Clears SB and writes address
    switch (i2c_status) {
    case I2C_STATUS_MASTER_TX:
    case I2C_STATUS_SLAVE_TX:  p_i2cx->DR = ((target_addr << 1) & ~I2C_READ_WRITE_BIT); break;
    case I2C_STATUS_MASTER_RX:
    case I2C_STATUS_SLAVE_RX:  p_i2cx->DR = ((target_addr << 1) | I2C_READ_WRITE_BIT); break;

    default:                   ASSERT(FALSE);
    }
}

static inline void clear_address_flag(i2c_reg_def *p_i2cx)
{
    uint32_t __vo dummy_read;
    dummy_read = p_i2cx->SR1;
    dummy_read = p_i2cx->SR2;
    UNUSED(dummy_read);
}

static inline void enable_ack(i2c_reg_def *p_i2cx)
{
    p_i2cx->CR1 |= I2C_CR1_ACK;
}

static inline void disable_ack(i2c_reg_def *p_i2cx)
{
    p_i2cx->CR1 &= ~I2C_CR1_ACK;
}

static inline void i2c_clock_enable(i2c_reg_def const *const p_i2cx)
{
    RCC->APB1ENR |= (p_i2cx == I2C1) ? RCC_APB1ENR_I2C1
                  : (p_i2cx == I2C2) ? RCC_APB1ENR_I2C2
                  : (p_i2cx == I2C3) ? RCC_APB1ENR_I2C3
                                     : 0;
}
static inline void i2c_clock_disable(i2c_reg_def const *const p_i2cx)
{
    RCC->APB1RSTR |= (p_i2cx == I2C1) ? RCC_APB1ENR_I2C1
                   : (p_i2cx == I2C2) ? RCC_APB1ENR_I2C2
                   : (p_i2cx == I2C3) ? RCC_APB1ENR_I2C3
                                      : 0;
}
