//======================================================================================//{
//  File: misc_test.c
//  Overview: Test miscellaneous functions that don't need their own seperate test file.
//  Tests:
//      - Assertion triggering
//      - Get different bus clocks
//      - Printf output
//  Notes: None
//======================================================================================//}

//} Includes
//=========================================//{

#include "misc_test.h"
#include "assert_handler.h"
#include "printf.h"
#include "rcc.h"

//======================================================================================//}
//                  Public Test Function Implementation
//======================================================================================//{

//============================================================================//{
//  Function: test_assert
//  Overview: Test assert with debugger.
//  Test goals:
//      - Assert is not triggered when true or 1.
//      - Assert is triggered when false or 0.
//  Setup:
//      None
//  Expectations: Assert should be triggered when ASSERT(0) is performed.
//  Note: None
//============================================================================//}
void test_assert(void)
{
    ASSERT(1);
    ASSERT(0);
}

//============================================================================//{
//  Function: test_get_clock
//  Overview: Test get bus clock functionality with debugger.
//  Test goals:
//      - Get different bus clocks:
//          - AHB1
//          - APB1
//          - APB2
//  Setup:
//      None
//  Expectations: With unmodified bus clocks, all 3 bus clocks should be 16Mhz.
//  Note: None
//============================================================================//}
void test_get_clock(void)
{
    uint32_t volatile ahb1_freq = rcc_get_bus_clock_freq_hz(AHB1_BUS);
    uint32_t volatile apb1_freq = rcc_get_bus_clock_freq_hz(APB1_BUS);
    uint32_t volatile apb2_freq = rcc_get_bus_clock_freq_hz(APB2_BUS);
    BREAKPOINT();
    UNUSED(ahb1_freq);
    UNUSED(apb1_freq);
    UNUSED(apb2_freq);
}

//============================================================================//{
//  Function: test_printf
//  Overview: Test printf functionality with logic analyzer.
//  Test goals:
//      - Printing to USART with printf implementation.
//      - Output data is correct
//  Setup:
//      - PA2 (defined in printf.h) -> logic analyzer
//  Expectations: "Test" should be output on the USART TX pin. Use logic analyzer
//      to view.
//  Note: None
//============================================================================//}
void test_printf(void)
{
    while (1)
    {
        printf_("Test");
        for (uint32_t tick = 0; tick < 50000; tick++);
    }
}

// *End of file*
