//======================================================================================//{
//  File: gpio_test.h
//  Overview: Tests some of the GPIO functionality.
//  Tests:
//      - Blink led by toggling
//      - Turn on led if button is pressed
//      - Toggle led when button is pressed using interrupts
//  Notes: None
//======================================================================================//}

#ifndef GPIO_TEST_H
#define GPIO_TEST_H

//======================================================================================//}
//                  Public Test Function Prototypes
//======================================================================================//{

void test_gpio_blink_led(void);
void test_gpio_read_write(void);
void test_gpio_it(void);

#endif

// *End of file*
