#include "toggle_led.h"

static uint32_t *pGPIOD_ODR = (uint32_t *)((GPIOD_BASEADDR) + (0x14));

void Led_Init(void)
{
    // Enable clock for GPIOD
    uint32_t *pRCC_AHB1_EN = (uint32_t *)((RCC_BASEADDR) + (0x30));
    *pRCC_AHB1_EN |= (1 << 3);

    // Toggle PD15 - Blue led
    uint32_t *pGPIOD_Mode = (uint32_t *)((GPIOD_BASEADDR) + (0x00));
    uint32_t *pGPIOD_OP_Type = (uint32_t *)((GPIOD_BASEADDR) + (0x04));

    *pGPIOD_Mode |= (1 << 30);
    *pGPIOD_OP_Type &= ~(1 << 15);

    uint32_t b = 5;
    uint32_t a = 5;
    uint32_t c = a + b;
    if ((a + b) == c) {
        c = b;
    }
}

void Led_Toggle(void)
{
    *pGPIOD_ODR ^= (1 << 15);
    uint32_t five = 5;
}