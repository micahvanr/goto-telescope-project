# Intro
This document contains the coding guidelines I will follow in order to keep the project consistent and readable. 
Some guidelines are my personal preferance while others are best practices.  

# Naming
* I will be using snake case in all cases except constant values. Constants will be in all caps.
``` C
#define THIS_IS_A_MACRO (50)
void this_is_a_function(void) 
{
    uint8_t this_is_a_variable;
}
```
### When naming try to follow these rules
* Avoid single letter variables
* Avoid abbreviations (unless very obvious)
* Prefix pointers and global scope variables (e.g. p_gpioa or g_task_num)
* Enums should be typedefined and suffixed with _e (e.g. gpio_mode_values_e)
* Include units in the name if applicable (e.g. delay_seconds)
* Prefix module function names their corresponding module (e.g. gpio_init) except for helper functions 

### Driver register structures:
* Each register structure definition will be in the format of the register it corresponds to. 
* Most will be in all caps and abbreviated to some degree. 
* This is to signify you are modifying a register and for consistency between the register and the referance manual
* Should be generated with reg_constant_gen.py

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

// This 
// is 
// a 
// multi-line 
// comment
```

## Source file comments
* Before each function there should be a short function block. It should contain a brief overview, what parameters it has, the values those parameters can be, what it returns, and any notes. Use None if it nothing applies to that section.
``` C
/***************************************************************************
Function: gpio_write
Overview: Sets a pin on a given port to high or low
Parameters:
    p_gpiox: Register structure for the GPIO port
    pin_no: Pin number for to be changed
        PIN_NO_x (0-15)
    pin_level: Pin logic level
        HIGH (1) 
        LOW (0)
Return: 
    None
Note: None
***************************************************************************/
void gpio_write(gpio_reg_def *p_gpiox, pin_number_e pin_no, pin_logic_level_e pin_level)
{
    // ... 
}
```
## Header file section comments
``` C
//======================================================================================//
//                  Header 1
//======================================================================================//

//========================================================//
//          Header 2
//========================================================//

//  Header 3
//=========================================//

// Basic comment
```
### Other comments
* Apart from the comments defined above I will add other comments here and there if something requires more explaination

# Constants
* Define constant variables and use comments if it is unclear where it comes from
* Prefer to use enums where possible but macros are fine
* Use static inline functions instead of macro functions as they are less error prone and easier to read.
* Always use paranthesis (even for single numbers) to avoid unexpected macro expansion 
``` C
#define CONSTANT_NUMBER (1) 
```
* Some macros act like functions and use a do {} while (0) statement like below and can be used as one with a semi-colon following it.
``` C
#define UNUSED(x) void(x)
// Or
#define ASSERT(expression)          \   
        do {                        \
            if(!(expression)) {     \
                assert_handler();   \
            }                       \
        } while(0)                  
```

## Driver register constants
### Miscellaneous constants
Register field constants should be created using the reg_constant_gen Python script. This keeps constants consistent
and easy to create. If additional register related constants are needed name them as following
and place them at the top of the register constants section.
``` C
// Example using USART. The position of the field was not generated cleanly.
typedef enum {
    USART_BRR_OVER8EN_CLEAR_POS = 3,
} usart_reg_misc_e;
``` 

### Field constants
If the constant is used to set the value of a field in a register use the bit format (0bx) to show the 
configuration of the bits. This makes comparing the values to the referance manual easier and 
it makes it easier to know what bits are being manipulated. The width of the bits should match the 
width of the field in the register.
``` C
typedef enum {
    // Example 
    PERI_REG_NAME = 0b001,
} peri_reg_misc_e;
```

# Header files
* Always keep #include guards (in caps) in every header file to avoid duplicated and recursive includsions.
``` C
#ifndef UART_H
#define UART_H
    // Defines and all code goes in here
#endif
```

* Include header files in order from local to global
``` C
#include "common/defines.h"
#include "drivers/uart.h"
#include "stm32f407xx.h"
```
### Driver header Organization
Driver header files should be organized in the following way:
* Address Definitions
* Peripheral Constants
* Register Constants
* Structure Definitions
* Peripheral Structure Macros
* Function API Prototypes

# Switch statements
* Use switch statements to avoid if else-if chain which can become unreadable
* Switches are convenient with enums as the compiler will warn when there are enum values that haven't been used in the switch statement

# if/else statements
* Always use brackets, even for single line statements
* Ternary if statements are okay but should not be overused

# Functions
* Driver functions should be prefixed with the peripheral they correspond to.
* Include void as parameter when there are no parameters. This is because in C, empty parameters can take any number of unknown types.
* Very short functions should be defined as static inline
``` C
static inline void helper_function(void) {
    // ...
}
```
### Helper functions
* Should only be used inside a single module should be defined as static and defined at the top of their corresponding .c file.
* Names should not contain a peripheral prefix to indicate they are a helper function and should not be used outside the file.
* Do not require a comment block above function. The function name should explain what it does.


# Data types
## Typedef
* Structures can be type defined
* Enums can be typedefined so we don't have to put enum before each one but ensure to suffix enum definitions with _e as stated in the naming section
* Enum member names should also be all uppercase
* Enum values should be explicitly stated if they are used as that value. If the value does not matter it should not be stated and let the compiler autofill the values.
``` C
typedef enum {
    GPIO_A,
    GPIO_B,
} gpio_ports_e;

typedef struct {
    uint8_t gpio_pin_number;
} gpio_handle;
```

## stdint.h
* Use fixed-width integers (e.g. int32_t, uint32_t uin16_t, uint8_t, etc.)
* This explicitly states how large the variables will be and is easier to port

## Const and volatile
* All variables and pointers should be defined as const and/or volatile when applicable.
* Const causes the compiler to output an error programmer if tries to modify the variable.
* Volatile tells the compiler to not perform optimization on the variables. This is useful when variables will change by external means, such as an external device.
* They can be used in combination to create more complex variables.
