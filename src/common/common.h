//======================================================================================//{
//  File: common.h
//  Overview: Provide macros for general/common uses.
//  Features:
//      Breakpoint macro
//      Unused variable macro
//      Weak attribute macro
//  Notes:
//======================================================================================//}

#ifndef COMMON_H
#define COMMON_H

#define BREAKPOINT(void)        \
    do                          \
    {                           \
        __asm volatile("BKPT"); \
    } while (0)

#define UNUSED(x) (void) (x)
#define __weak    __attribute__((weak))

#endif

// *End of file*
