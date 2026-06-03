#ifndef DEBUG_PIN_H
#define DEBUG_PIN_H

#define DEBUG_PIN_PORT GPIOA
#define DEBUG_PIN_NO PIN_NO_3

void debug_pin_init(void);
void toggle_debug_pin(void);

#endif
