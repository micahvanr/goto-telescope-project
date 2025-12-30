#include "toggle_led.h"
#include <stdint.h>

void delay(void);

int main(void)
{

    Led_Init();

    while (1) {
        Led_Toggle();
        delay();
    }

    return 0;
}

void delay(void)
{
    for (uint32_t i = 0; i < 500000; i++) {}
}