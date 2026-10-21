# Intro
The coding guidelines I will be following will be the Barr Group's Embedded C Coding Standard. It is
a set of rules/guidelines for embedded C and it focuses on preventing bugs. The sections listed here 
follow the section structure in the Barr C standard and expland on those sections with some additional
sections. These guidelines are to keep the project files consistent and easy to read. 

# Barr C Standard
The Barr C coding guidelines document can be downloaded for free from [here](https://barrgroup.com/embedded-c-coding-standard). 
This standard provides a very solid foundation with reasoning behind each rule beyond just style preference. 
Most rules aim to keep bugs from appearing in the first place. Some rules I have chosen to not use or modify slightly. 
These rules will be listed in their corresponding section along with their reasoning. In addition, I may expand on 
these rules in those sections. Some sections will be blank and that just means I do not have any additional info
to provide. Any code that breaks these rules should have comments explaining why the rule was broken.


# General Rules
## Which C?
* Exception: "Preprocessor directive #define shall not be used to alter or rename any keyword or other aspect 
of the programming language" except with the assert library. Including this add a lot of bloat and I prefer 
my own implementation.

## Line Widths
* Exception: "The width of all lines in a program shall be limited to a maximum of 80 characters". I will not be 
printing my code out and I'd prefer to not have too many line breaks so I will be using max 120 characters.

## Common Abbreviations
See [common_abbreviations.md](common_abbreviations.md) file for table of all abbreviations.

# Comment Rules
* Exception: "Any line or block of code that exists specifically to increase the level of debug output information
shall be surrounded by #ifndef NDEBUG ... #endif" except in test code as that code will be used to debug and ensure
driver code runs as expected.
* Code should be self documenting. Comments should not explain what is going on unless it is complex and requires 
explaination. Comments should explain why something is necessary and sometimes how.
* Use "//" for single and multi line comments.
* See [standard_commend_phrases.md](standard_commend_phrases.md) file for list of standard phrases that might
be used.

# White Space Rules
## Blank Lines
* Exception: "No line of code shall contain more than one statement" except with assertion statements to check parameters.
Breaking these statements up makes it more easier to read. Otherwise this rule remains intact.

# Modules Rules
## Header Files
* Header files should be in the general format of: comment block, include statements, constants, public structures, 
and public function prototypes.
* See [Header File Layout](#header-file-layout) for driver specific layout.

## Source Files
* Source files should be in the general format of: comment block, include statements, constants, private structures, 
    private global variables, private function prototypes, public function implementation, and private function implementation.
* See [Source File Layout](#source-file-layout) for driver specific layout.

## File Templates
See [templates/](../src/templates/) to view specific source and header file templates.

# Data Type Rules

# Function Rules (Procedure Rules)
* Include void as parameter when there are no parameters. This is because in C, empty parameters can take any number of unknown types.

## Naming Conventions
* Function names should follow the following standard: This process adds more consistency when creating
functions that can be categorized. I prefer verb-noun function names as I find them more natural.
    * module_submod_verb_noun
        * module: The logical module the function is apart of. Should be the file name. Private functions
        are not required to include this.
        * submod: *Optional.* The logical sub module. 
        * verb: The action being performed.
        * noun: *Optional.* What the action is being performed on or to. Necessary when what is being 
        manipulated requires more detail than just the module name.

### Prefer to use the following verbs:
* init
* get/set: Instead of read/write unless read/write makes a lot more sense.
* is_x: Used when returning bool. Is allowed to be after the noun as it makes more sense.
* clear
* togglable: Prefer to not use enable/disable_x as that increases code size. Instead use togglable_x 
with parameter to enable or disable the object.
* assert_**x**_param: Used putting assertions for parameters for **x** in seperate function.
* setup: Only use this for test functions, differentiates between init.

## Comment Blocks
* See [templates/](../src/templates/) for function comment blocks

## Private Functions
* Names should not contain a peripheral prefix to indicate they are a helper function and should not be used outside the file.
* Do not require a comment block above function. The function name should explain what it does but a small comment above is okay

## Inline Functions
* Private/helper inline functions should use the "static inline" keywords
* Public inline functions do not use the "inline" keyword. That tells the compiler the definition is 
implemented somewhere else. Instead, in normal cases, leave the function as is but if the function needs
to be inlined, use a macro.
    * If using macro function use a "do {} while (0)" statement
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

# Variables Rules

# Statement Rules

## Switch Statements
* Exception: "All switch statements shall contain a default block". I prefer to not include default 
block so the compiler will warn when there are options from an enum that are not used.

# Constants
This section provides additional guidelines and structure for constants.
## Types
* Enums - main constant used
    * Should be the main constant used
    * Append "_e" to the end of enum names to signify they are an enum
    * Make the type be "uint32_t" if there is even a slight chance the sign bit gets manipulated as enums are 
    integers by default and changing the sign bit is undefined behavior. Only leave if certain the sign bit wont
    be modified.
* Macros
    * Use paranthesis (even for single numbers) to avoid unexpected macro expansion
* Constant variables
    * Use when the constant is only used locally to that function.
## General Guidelines
* They should be self explainatory but use comments above them if they require more explaination
* Explicitly state what the enum/macro value should be if the constant needs to be a specific value
otherwise leaving blank is fine. Do not rely on compiler setting enum values to necessary values.
``` C
#define CONSTANT_NUMBER (1) 

typedef enum : uint32_t {
    CONSTANT_VALUE_IMPORTANT = 0,
    CONSTANT_VALUE_NOT_IMPORTANT,
} example_e;
```

# Driver Specific
## Driver Register Structures:
* Should be generated with reg_constant_gen.py
* Each register structure definition will be in the order of the register it corresponds to. 
* They will be in all caps and abbreviated to some degree. 
* This is to signify you are modifying a register and for consistency between the register and the referance manual

## Driver Constants
### Driver register constants
#### Miscellaneous constants
* Register field constants should be created using the reg_constant_gen Python script. This keeps constants consistent
and easy to create. If additional register related constants are needed name them as following
and place them at the top of the register constants section. 
* If using enums they should have a type  of "uint32_t" as enums are integers by default and if they 
are shifted to the end and change the sign bit. That is undefined behavior.
* Append "U" to the value to ensure it is interpreted as unsigned inside the enum. Other types of constants are fine
and do not need this.
``` C
// Example using USART. The position of the field was not generated cleanly.
typedef enum : uint32_t {
    USART_BRR_OVER8EN_CLEAR_POS = 3U,
} usart_reg_misc_e;
``` 

#### Field constants
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

## Driver Implementation 
### Initialization
Driver initialization for each register should follow the procedure below:
If setting multiple fields:
If setting one field follow steps 2 & 3:
Repeat for each register
1. Read current register value to temp variable named temp_x (reg name). In temp variable:
2. Clear register field to be set
3. Set register field
4. Repeat 2 & 3 for each field to be set in the register
5. After all fields set, set register with temp variable
This keeps consistency between each driver. In addition, it keeps configuration of other parts of the register intact
and ensures each field is cleared before being set.
``` C
void example_init(handler example_handler) {
    // Multiple field change
    uint32_t temp_cr1 = example_handler->CR1;

    // If field is single bits wide
    temp_cr1 &= ~EX_CR1_FIELD_1;
    // If field is multi bits wide
    temp_cr1 &= ~(EX_CR1_FIELD_2_MASK << EX_CR1_FIELD_2_POS)
    example_handler->CR1 = temp_cr1;

    // Single field change
    example_handler->CR2 &= ~(EX_CR2_FIELD)
    example_handler->CR2 = example_handler.cr2_field;
}
```
