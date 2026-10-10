//======================================================================================//{
//  File: i2c_test.c
//  Overview: Tests some of the I2C functionality.
//  Tests:
//      - Master/slave transmit/receive (w/ interrupts)
//  Notes: Check ./i2c_test_arduino/i2c_com_config.h file for more information on I2C
//      comminication settings.
//======================================================================================//}

#ifndef I2C_TEST_H
#define I2C_TEST_H

//======================================================================================//}
//                  Public Test Function Prototypes
//======================================================================================//{

void test_i2c_master_transmit(void);
void test_i2c_slave_transmit(void);
void test_i2c_master_receive(void);
void test_i2c_slave_receive(void);
void test_i2c_master_transmit_it(void);
void test_i2c_slave_transmit_it(void);
void test_i2c_master_receive_it(void);
void test_i2c_slave_receive_it(void);

#endif

// *End of file*
