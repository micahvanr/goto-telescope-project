#include "i2c.h"
#include "assert_handler.h"
#include "rcc.h"
#include "stm32f4xx.h"

// TODO: Implement function callbacks
// TODO: Create comment macros for nvim

/*****************************************************************
                        Helper Function Prototypes
*****************************************************************/

static inline void verify_i2c_init_asserts(i2c_handle const *const p_i2c_handle);

static inline i2c_init_port_num_e map_i2c_ports_to_num(i2c_reg_def const *const p_i2cx);
static inline bool verify_i2c_initialized(i2c_reg_def const *p_i2cx);
static void set_ccr_and_trise(i2c_handle const *const p_i2c_handle);

static inline void i2c_clock_enable(i2c_reg_def const *const p_i2cx);
static inline void i2c_clock_disable(i2c_reg_def const *const p_i2cx);

static inline void generate_start_condition(i2c_reg_def *p_i2cx);
static inline void generate_stop_condition(i2c_reg_def *p_i2cx);
static inline void clear_start_flag(i2c_reg_def const *p_i2cx);
static inline void send_address(i2c_reg_def *p_i2cx, uint8_t target_addr, i2c_status_e i2c_status);
static inline void clear_address_flag(i2c_reg_def const *p_i2cx);
static inline void enable_ack(i2c_reg_def *p_i2cx);
static inline void disable_ack(i2c_reg_def *p_i2cx);
static inline void enable_i2c_interrupts(i2c_reg_def *p_i2cx);
static inline void disable_i2c_interrupts(i2c_reg_def *p_i2cx);
static inline void receive_byte(i2c_handle *p_i2c_handle);
static inline void send_byte(i2c_handle *p_i2c_handle);
static inline void close_com(i2c_handle *p_i2c_handle);
static inline void clear_stopf(i2c_reg_def *p_i2cx);
static inline void clear_af(i2c_reg_def *p_i2cx);

static void master_sb(i2c_handle *p_i2c_handle);
static void master_addr(i2c_handle *p_i2c_handle);
static void slave_addr(i2c_handle const *p_i2c_handle);
/*****************************************************************
                        Global variables
*****************************************************************/

// I2C1 = bit pos 0
// I2C2 = bit pos 1
// I2C3 = bit pos 2
uint8_t g_i2c_port_init = 0;
// If peripheral bit is 1, it is running. If 0, it is free.
uint8_t g_i2c_port_status = 0;

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
    uint8_t found_setting = false;

    // Peripheral check
    found_setting = (p_i2c_handle->p_i2cx == I2C1) ? true
                  : (p_i2c_handle->p_i2cx == I2C2) ? true
                  : (p_i2c_handle->p_i2cx == I2C3) ? true
                                                   : false;
    ASSERT(found_setting);

    // Fast mode duty cycle check
    found_setting = false;
    switch (p_i2c_handle->i2c_conf.fm_duty_cycle) {
    case I2C_FM_DUTY_2:    found_setting = true; break;
    case I2C_FM_DUTY_16_9: found_setting = true; break;
    }
    ASSERT(found_setting);

    // Speed mode check
    found_setting = false;
    switch (p_i2c_handle->i2c_conf.speed_mode) {
    case I2C_SPEED_MODE_STANDARD: found_setting = true; break;
    case I2C_SPEED_MODE_FAST:     found_setting = true; break;
    }
    ASSERT(found_setting);

    // Clock frequency check
    found_setting = false;
    switch (p_i2c_handle->i2c_conf.clock_freq_hz) {
    case I2C_CLK_FREQ_100KHZ: found_setting = true; break;
    case I2C_CLK_FREQ_200KHZ: found_setting = true; break;
    case I2C_CLK_FREQ_300KHZ: found_setting = true; break;
    case I2C_CLK_FREQ_400KHZ: found_setting = true; break;
    }
    ASSERT(found_setting);
}

/***************************************************************************
Function: i2c_reset
Overview: disables the clock for the given i2c peripheral, resetting it
Parameters:
    p_i2cx: I2C register peripheral to be reset
Return: 
    None
Note: None
***************************************************************************/
void i2c_reset(i2c_reg_def const *const p_i2cx)
{
    i2c_clock_disable(p_i2cx);
}

// TODO: Might need to return success/fail incase the device never receives and ack

/***************************************************************************
Function: i2c_master_transmit
Overview: takes the role of master and transmits data
Parameters:
	p_i2cx: I2C peripheral used to communicate
	target_addr: Address of device to communicate to
	p_data: Pointer to data that will be sent. Can be dereferenced variable or list
	length: Length of data being sent
	repeated_start: Whether the transmission will end with a stop signal
		I2C_REPEATED_START_ENABLE
		I2C_REPEATED_START_DISABLE
Return: 
    None
Note: None
***************************************************************************/
void i2c_master_transmit(i2c_reg_def *const p_i2cx, uint8_t target_addr, uint8_t const *p_data, uint32_t length,
                         i2c_repeated_start_e repeated_start)
{
    ASSERT(verify_i2c_initialized(p_i2cx));

    // Ensure peripheral is not busy
    while (p_i2cx->SR2 & I2C_SR2_BUSY);

    enable_ack(p_i2cx);

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
Function: i2c_master_receive
Overview: takes the role of master and receives data
Parameters:
	p_i2cx: I2C peripheral used to communicate
	target_addr: Address of device to communicate to
	p_data: Pointer to data that will be recieved. Can be dereferenced variable or list
	length: Length of data being sent
	repeated_start: Whether the transmission will end with a stop signal
		I2C_REPEATED_START_ENABLE
		I2C_REPEATED_START_DISABLE
Return: 
    None
Note: None
***************************************************************************/
void i2c_master_receive(i2c_reg_def *const p_i2cx, uint8_t target_addr, uint8_t *p_data, uint32_t length,
                        i2c_repeated_start_e repeated_start)
{
    ASSERT(verify_i2c_initialized(p_i2cx));

    // Ensure peripheral is not busy
    while (p_i2cx->SR2 & I2C_SR2_BUSY);

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
Function: i2c_slave_transmit
Overview: takes the role of slave and transmits data
Parameters:
	p_i2cx: I2C peripheral used to communicate
	target_addr: Address of device to communicate to
	p_data: Pointer to data that will be transmitted. Can be dereferenced variable or list
	length: Length of data being sent
Return: 
    None
Note: None
***************************************************************************/
void i2c_slave_transmit(i2c_reg_def *const p_i2cx, uint8_t const *p_data, uint32_t length)
{
    ASSERT(verify_i2c_initialized(p_i2cx));

    // Ensure peripheral is not busy
    while (p_i2cx->SR2 & I2C_SR2_BUSY);

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
    clear_af(p_i2cx);

    // Turn acking off
    disable_ack(p_i2cx);
}

/***************************************************************************
Function: i2c_slave_receive
Overview: takes the role of slave and receives data
Parameters:
	p_i2cx: I2C peripheral used to communicate
	target_addr: Address of device to communicate to
	p_data: Pointer to data that will be recieved. Can be dereferenced variable or list
	length: Length of data being sent
Return: 
    None
Note: None
***************************************************************************/
void i2c_slave_receive(i2c_reg_def *const p_i2cx, uint8_t *p_data, uint32_t length)
{
    ASSERT(verify_i2c_initialized(p_i2cx));

    // Ensure peripheral is not busy
    while (p_i2cx->SR2 & I2C_SR2_BUSY);

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
    clear_stopf(p_i2cx);

    // Turn acking off
    disable_ack(p_i2cx);
}

/***************************************************************************
Function: i2c_master_transmit_it
Overview: takes the role of master and transmits data using interrupts
Parameters:
    p_i2c_handle: Structure with the settings and data for it handling
	target_addr: Address of device to communicate to
	p_data: Pointer to data that will be sent. Can be dereferenced variable or list
	length: Length of data being sent
	repeated_start: Whether the transmission will end with a stop signal
		I2C_REPEATED_START_ENABLE
		I2C_REPEATED_START_DISABLE
Return: 
    None
Note: None
***************************************************************************/
void i2c_master_transmit_it(i2c_handle *p_i2c_handle, uint8_t target_addr, uint8_t *p_data, uint32_t length,
                            i2c_repeated_start_e repeated_start)
{
    ASSERT(verify_i2c_initialized(p_i2c_handle->p_i2cx));

    // Wait until usart is available (blocking)
    while (!(p_i2c_handle->i2c_it_data.status == I2C_STATUS_READY));
    p_i2c_handle->i2c_it_data.status = I2C_STATUS_MASTER_TX;

    // Set corresponding handle variables
    p_i2c_handle->i2c_it_data.txrx_buffer    = p_data;
    p_i2c_handle->i2c_it_data.txrx_length    = length;
    p_i2c_handle->i2c_it_data.target_addr    = target_addr;
    p_i2c_handle->i2c_it_data.repeated_start = repeated_start;

    generate_start_condition(p_i2c_handle->p_i2cx);

    enable_i2c_interrupts(p_i2c_handle->p_i2cx);
}

/***************************************************************************
Function: i2c_master_recieve_it
Overview: takes the role of master and receives data using interrupts
Parameters:
    p_i2c_handle: Structure with the settings and data for it handling
	target_addr: Address of device to communicate to
	p_data: Pointer to data that will be received. Can be dereferenced variable or list
	length: Expected length of data being received
	repeated_start: Whether the transmission will end with a stop signal
		I2C_REPEATED_START_ENABLE
		I2C_REPEATED_START_DISABLE
Return: 
    None
Note: None
 ***************************************************************************/
void i2c_master_receive_it(i2c_handle *p_i2c_handle, uint8_t target_addr, uint8_t *p_data, uint32_t length,
                           i2c_repeated_start_e repeated_start)
{
    ASSERT(verify_i2c_initialized(p_i2c_handle->p_i2cx));

    // Wait until usart is available (blocking)
    while (!(p_i2c_handle->i2c_it_data.status == I2C_STATUS_READY));
    p_i2c_handle->i2c_it_data.status = I2C_STATUS_MASTER_RX;

    // Set corresponding handle variables
    p_i2c_handle->i2c_it_data.txrx_buffer    = p_data;
    p_i2c_handle->i2c_it_data.txrx_length    = length;
    p_i2c_handle->i2c_it_data.target_addr    = target_addr;
    p_i2c_handle->i2c_it_data.repeated_start = repeated_start;

    generate_start_condition(p_i2c_handle->p_i2cx);

    enable_ack(p_i2c_handle->p_i2cx);

    enable_i2c_interrupts(p_i2c_handle->p_i2cx);
}

void i2c_slave_transmit_it(i2c_handle *p_i2c_handle, uint8_t *p_data, uint32_t length)
{
    ASSERT(verify_i2c_initialized(p_i2c_handle->p_i2cx));

    // Wait until usart is available (blocking)
    while (!(p_i2c_handle->i2c_it_data.status == I2C_STATUS_READY));
    p_i2c_handle->i2c_it_data.status = I2C_STATUS_SLAVE_TX;

    // Set corresponding handle variables
    p_i2c_handle->i2c_it_data.txrx_buffer = p_data;
    p_i2c_handle->i2c_it_data.txrx_length = length;

    enable_ack(p_i2c_handle->p_i2cx);

    enable_i2c_interrupts(p_i2c_handle->p_i2cx);
}

void i2c_slave_receive_it(i2c_handle *p_i2c_handle, uint8_t *p_data, uint32_t length)
{
    ASSERT(verify_i2c_initialized(p_i2c_handle->p_i2cx));

    // Wait until usart is available (blocking)
    while (!(p_i2c_handle->i2c_it_data.status == I2C_STATUS_READY));
    p_i2c_handle->i2c_it_data.status = I2C_STATUS_SLAVE_RX;

    // Set corresponding handle variables
    p_i2c_handle->i2c_it_data.txrx_buffer = p_data;
    p_i2c_handle->i2c_it_data.txrx_length = length;

    enable_ack(p_i2c_handle->p_i2cx);

    enable_i2c_interrupts(p_i2c_handle->p_i2cx);
}

/***************************************************************************
Function: i2c_it_config
Overview: Controls interrupts for I2C peripheral
Parameters:
	p_i2cx: I2C peripheral to be modified
    toggle: 
        ENABLE (1)
        DISABLE (0)
Return: 
    None
Note: None
 ***************************************************************************/
void i2c_it_config(i2c_reg_def const *const p_i2cx, togglable_e toggle)
{
    if (p_i2cx == I2C1) {
        irq_config(I2C1_EV_IRQ_NO_31, toggle);
    } else if (p_i2cx == I2C2) {
        irq_config(I2C2_EV_IRQ_NO_33, toggle);
    } else if (p_i2cx == I2C3) {
        irq_config(I2C3_EV_IRQ_NO_72, toggle);
    }
}

/***************************************************************************
Function: i2c_it_handler
Overview: Handles interrupts for I2C peripheral
Parameters:
	p_i2cx: I2C peripheral to be modified
    toggle: 
        ENABLE (1)
        DISABLE (0)
Return: 
    None
Note: Should be called from the ISR function
 ***************************************************************************/
void i2c_it_handler(i2c_handle *const p_i2c_handle)
{
    uint32_t __vo dummy_read = 0;

    switch (p_i2c_handle->i2c_it_data.status) {

    case I2C_STATUS_MASTER_TX:
    case I2C_STATUS_MASTER_RX:
        if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_SB) {
            master_sb(p_i2c_handle);
        }

        if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_ADDR) {
            master_addr(p_i2c_handle);
        }

        switch (p_i2c_handle->i2c_it_data.status) {
        case I2C_STATUS_MASTER_TX:
            if ((p_i2c_handle->i2c_it_data.txrx_length == 0) && (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_BTF)
                && (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_TxE)) {
                close_com(p_i2c_handle);
            }

            else if ((p_i2c_handle->p_i2cx->SR1 & I2C_SR1_TxE) && (p_i2c_handle->i2c_it_data.txrx_length != 0)) {
                send_byte(p_i2c_handle);
            }
            break;

        case I2C_STATUS_MASTER_RX:
            if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_RxNE) {
                if (p_i2c_handle->i2c_it_data.txrx_length == 2) {
                    disable_ack(p_i2c_handle->p_i2cx);
                }

                receive_byte(p_i2c_handle);

                if (p_i2c_handle->i2c_it_data.txrx_length == 0) {
                    close_com(p_i2c_handle);
                }
            }
            break;

        default: ASSERT(false); break;
        }
        break;

    case I2C_STATUS_SLAVE_TX:
    case I2C_STATUS_SLAVE_RX:

        if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_ADDR) {
            slave_addr(p_i2c_handle);
        }

        switch (p_i2c_handle->i2c_it_data.status) {

        case I2C_STATUS_SLAVE_TX:
            // if (p_i2c_handle->i2c_it_data.txrx_length == 0) {
            // }

            if ((p_i2c_handle->p_i2cx->SR1 & I2C_SR1_TxE) && (p_i2c_handle->i2c_it_data.txrx_length != 0)) {
                send_byte(p_i2c_handle);
            }

            if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_AF) {
                clear_af(p_i2c_handle->p_i2cx);
                close_com(p_i2c_handle);
            }

            break;
        case I2C_STATUS_SLAVE_RX:
            if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_RxNE) {
                receive_byte(p_i2c_handle);
            }

            if (p_i2c_handle->p_i2cx->SR1 & I2C_SR1_STOPF) {
                clear_stopf(p_i2c_handle->p_i2cx);
                close_com(p_i2c_handle);
            }
            break;
        default: ASSERT(false); break;
        }

        break;

    default: ASSERT(false); break;
    }
    UNUSED(dummy_read);
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

static inline bool verify_i2c_initialized(i2c_reg_def const *p_i2cx)
{
    if (g_i2c_port_init & (1 << map_i2c_ports_to_num(p_i2cx))) {
        return true;
    } else {
        return false;
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
            ASSERT(false);
        }

        // Set CCR
        ccr_value = ((peripheral_clock_khz) / (2 * (desired_clock_khz)));

        // Set time rise value
        trise_value |= ((I2C_MAX_RISE_SM_1000_NS / (1000000 / (peripheral_clock_khz))) + 1);
        break;

    case I2C_SPEED_MODE_FAST:
        // Peripheral clock speed must be at leats 4Mhz in fast mode
        if (peripheral_clock_khz < 4000) {
            ASSERT(false);
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
static inline void clear_start_flag(i2c_reg_def const *p_i2cx)
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

    default:                   ASSERT(false);
    }
}

static inline void clear_address_flag(i2c_reg_def const *p_i2cx)
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

static inline void enable_i2c_interrupts(i2c_reg_def *p_i2cx)
{

    // Enable interrupts in cpu
    i2c_it_config(p_i2cx, ENABLE);

    uint32_t temp_reg;
    temp_reg = 0;
    temp_reg |= I2C_CR2_ITBUFEN;
    temp_reg |= I2C_CR2_ITEVTEN;
    temp_reg |= I2C_CR2_ITERREN;

    p_i2cx->CR2 |= temp_reg;
}

static inline void disable_i2c_interrupts(i2c_reg_def *p_i2cx)
{
    p_i2cx->CR2 &= ~I2C_CR2_ITBUFEN;
    p_i2cx->CR2 &= ~I2C_CR2_ITERREN;
    p_i2cx->CR2 &= ~I2C_CR2_ITEVTEN;
}

static inline void send_byte(i2c_handle *p_i2c_handle)
{
    p_i2c_handle->p_i2cx->DR = *p_i2c_handle->i2c_it_data.txrx_buffer;
    p_i2c_handle->i2c_it_data.txrx_buffer++;
    p_i2c_handle->i2c_it_data.txrx_length--;
}

static inline void receive_byte(i2c_handle *p_i2c_handle)
{
    *p_i2c_handle->i2c_it_data.txrx_buffer = p_i2c_handle->p_i2cx->DR;
    p_i2c_handle->i2c_it_data.txrx_buffer++;
    p_i2c_handle->i2c_it_data.txrx_length--;
}

static inline void close_com(i2c_handle *p_i2c_handle)
{
    if (p_i2c_handle->i2c_it_data.repeated_start == I2C_REPEATED_START_DISABLE) {
        generate_stop_condition(p_i2c_handle->p_i2cx);
    }
    disable_i2c_interrupts(p_i2c_handle->p_i2cx);
    switch (p_i2c_handle->i2c_it_data.status) {
    case I2C_STATUS_SLAVE_RX:
    case I2C_STATUS_SLAVE_TX: disable_ack(p_i2c_handle->p_i2cx); break;
    default:                  ;
    }
    p_i2c_handle->i2c_it_data.status      = I2C_STATUS_READY;
    p_i2c_handle->i2c_it_data.txrx_length = 0;
    p_i2c_handle->i2c_it_data.txrx_buffer = 0;
    p_i2c_handle->i2c_it_data.target_addr = 0;
}

static inline void clear_stopf(i2c_reg_def *p_i2cx)
{
    // cppcheck-suppress-begin redundantAssignment
    uint32_t dummy_action;
    dummy_action = p_i2cx->SR1;
    dummy_action = 0;
    p_i2cx->CR1 |= dummy_action;
    // cppcheck-suppress-end redundantAssignment
}

static inline void clear_af(i2c_reg_def *p_i2cx)
{
    p_i2cx->SR1 &= ~I2C_SR1_AF;
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

static void master_sb(i2c_handle *p_i2c_handle)
{
    clear_start_flag(p_i2c_handle->p_i2cx);
    send_address(p_i2c_handle->p_i2cx, p_i2c_handle->i2c_it_data.target_addr, p_i2c_handle->i2c_it_data.status);
}

static void master_addr(i2c_handle *p_i2c_handle)
{
    // Disable ack before clearing address flag in 1 byte reception
    if ((p_i2c_handle->i2c_it_data.status == I2C_STATUS_MASTER_RX) && (p_i2c_handle->i2c_it_data.txrx_length == 1)) {
        disable_ack(p_i2c_handle->p_i2cx);
    }
    clear_address_flag(p_i2c_handle->p_i2cx);
}
static void slave_addr(i2c_handle const *p_i2c_handle)
{
    clear_address_flag(p_i2c_handle->p_i2cx);
}
