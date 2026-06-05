#ifndef ASSERT_HANDLER
#define ASSERT_HANDLER

#define ASSERT(expression)    \
    do {                      \
        if (!(expression)) {  \
            assert_handler(); \
        }                     \
    } while (0)

void assert_handler(void);

#endif
