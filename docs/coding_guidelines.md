# Intro
This document contains the coding guidelines I will follow in order to keep the project consistent and readable. Some guidelines are my personal preferance while others are best practices. In addition, I will be using clang to enforce some formatting. 

# Naming
* I will be using snake case in all cases except macros and enum values. Macros and enum values will be in all caps.
``` C
#define THIS_IS_A_MACRO (50)
void this_is_a_function(void) {
    uint8_t this_is_a_variable;
}
```
### When naming try to follow these rules
* Avoid single letter variables
* Avoid abbreviations (except stated below)
* Prefix pointers and global scope variables (e.g. p_gpioa or g_task_num)
* Enums should be typedefined and suffixed with _e (e.g. gpio_mode_values_e)
* Include units in the name if applicable (e.g. delay_seconds)
* Prefix module names their corresponding module (e.g. gpio_init) except for helper functions 
### Other naming conventions:
* One code module per header and source file
    * A module being something that makes sense as a single sections (e.g. uart.c/uart.h, i2c.c/i2c.h)

# Indentation
* Use 4 spaces
* No tabs

# Comments
* Use comments when necessary to explain why and how
* Don't overuse them
* Prefer good variable and function names over comments
* Use // for single line comments and /* */ for multi-line comments
``` C
// This is a comment

/* 
This is a multi-line 
comment
*/
```
* Comment a block before each function giving a brief overview, what parameters it has, what it returns, and any notes. Use None if it nothing applies to that section.
``` C
/*
Function: gpio_init
Overview: Initilizes the given GPIO port with the settings provided in the structure
Parameters:
    gpio_settings   - The handler structure with the configuration settings to be applied              
Return: None
Note: None
*/
void gpio_init(gpio_handler gpio_settings) {
    // ... 
}
```

# Defines
* Define constant variables and use comments if it is unclear where it comes from
* Prefer to use enums where possible
* Use static inline functions instead of macro functions as they are less error prone and easier to read.
* Always use paranthesis (even for single numbers) to avoid unexpected macro expansion 
``` C
#define CONSTANT_NUMBER (1) 
```

# Header files
* Always keep #include guards (in caps) in every header file to avoid duplicated and recursive includsions.
``` C
#ifndef UART_H
    // Defines and all code goes in here
#endif
```

* Include header files in order from global to local
``` C
#include "stm32f407xx.h"
#include "common/defines.h"
#include "drivers/uart.h"
```

# Switch statements
* Use switch statements to avoid if else-if chain which can become unreadable
* Switches are convenient with enums as the compiler will warn when there are enum values that haven't been used in the switch statement

# if/else statements

* Always use brackets, even for single line statements
* Ternary if statements are okay but should not be overused

# Functions
* Helper functions that will only be used inside a single module should be defined as static and defined at the top of their corresponding .c file.
* Very short functions should be defined as static inline
* Include void as parameter when there are no parameters. This is because in C, empty parameters can take any number of unknown types.
``` C
static inline void helper_function(void) {
    // ...
}
```

# Data types
## Typedef
* Don't typedef structs so it is clear whether a variable is an enum or struct
* Enums can be typedefined so we don't have to put enum before each one but ensure to suffix enum definitions with _e as stated in the naming section
* Enum values should also be all uppercase
``` C
typedef enum {
    GPIO_A,
    GPIO_B
}gpio_ports_e;

struct gpio_handle {
    uint8_t gpio_pin_number;
}
```

## stdint.h
* Use fixed-width integers (e.g. int32_t, uint32_t uin16_t, uint8_t, etc.)
* This explicitly states how large the variables will be and is easier to port

## Const and volatile
* All variables and pointers should be defined as const and/or volatile when applicable.
* Const causes the compiler to output an error programmer if tries to modify the variable.
* Volatile tells the compiler to not perform optimization on the variables. This is useful when variables will change by external means, such as an external device.
* They can be used in combination to create more complex variables.
