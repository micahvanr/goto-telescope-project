//======================================================================================//{
//  File: timer_test.h
//  Overview: Tests some of the timer's functionality.
//  Tests:
//      - Auto setup
//      - Manual setup
//      - Frequency setup
//      - Delay function
//      - Output compare PWM
//      - Input capture time difference (using interrupts)
//      - Basic interrupt
//  Notes:
//======================================================================================//}

#ifndef TIMER_TEST_H
#define TIMER_TEST_H

//} Includes
//=========================================//{

//======================================================================================//}
//                  Public Test Function Prototypes
//======================================================================================//{

void test_timer_auto(void);
void test_timer_manual(void);
void test_timer_freq(void);
void test_delay(void);
void test_oc_pwm(void);
void test_ic_it(void);
void test_it(void);

#endif

// *End of file*
