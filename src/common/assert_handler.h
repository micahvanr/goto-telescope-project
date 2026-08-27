#ifndef ASSERT_HANDLER
#define ASSERT_HANDLER

#define ASSERT(expression)    \
    do {                      \
        if (!(expression)) {  \
            assert_handler(); \
        }                     \
    } while (0)

#define BREAKPOINT(void)        \
    do {                        \
        __asm volatile("BKPT"); \
    } while (0)

void assert_handler(void);

#endif
