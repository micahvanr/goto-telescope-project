//======================================================================================//{
//  File: assert_handler.h
//  Overview: Provide ASSERT() function and handler with custom handling.
//  Features:
//      ASSERT(): Calls assert_handler() to perform fail safe.
//  Notes: None
//======================================================================================//}

#ifndef ASSERT_HANDLER
#define ASSERT_HANDLER

#define ASSERT(expression)    \
    do                        \
    {                         \
        if (!(expression))    \
        {                     \
            assert_handler(); \
        }                     \
    } while (0)

void assert_handler(void);

#endif // DRIVER_H
       
// *End of file*
