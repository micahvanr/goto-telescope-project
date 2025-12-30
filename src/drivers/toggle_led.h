#define GPIOD_BASEADDR 		0x40020C00UL
#define RCC_BASEADDR        0x40023800UL

#include <stdint.h>

void Led_Init(void);
void Led_Toggle(void);