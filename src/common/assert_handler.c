#include "assert_handler.h"

static void blink_led(void);

void assert_handler(void)
{
    // TODO: Turn off telescope motors
    // TODO: Log to console

    // Breakpoint
    __asm volatile("BKPT");

    blink_led();
}

static void blink_led(void)
{
    // Blink LED
    uint32_t *p_rcc_ahb1enr  = (uint32_t *)(RCC_BASE_ADDR + 0x30);
    uint32_t *p_gpio_led_reg = (uint32_t *)(GPIOA_BASE_ADDR + ((LED_RED_PORT) * 0x0400));

    // Enable clock on led port
    *p_rcc_ahb1enr |= (1 << LED_RED_PORT);

    // Configure led port
    // Mode Register
    *p_gpio_led_reg |= (0b01 << (LED_RED_PIN * 2));

    p_gpio_led_reg++; // Move to offset 0x04 - Output type register
    *p_gpio_led_reg &= ~(1 << LED_RED_PIN);

    p_gpio_led_reg += 4; // Move to offset 0x14 - Output data register

    // Toggle LED
    while (1) {
        *p_gpio_led_reg ^= (1 << LED_RED_PIN);
        for (uint32_t i = 0; i < 500000; i++) {}
    }
}