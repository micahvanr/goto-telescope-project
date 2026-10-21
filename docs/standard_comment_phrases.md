# Standard Comment Phrases
* Sometimes comments from different sections have similar ideas, topics, or categories. For example, both
USART and GPIO have a funtion to initialize their peripheral. They will both have an "Overview" comment
section. Both of these should use the same phrase or sentence to stay consistent and clear. Below will contain
a list of all standard phrases.
* This list also provides a broader more general format for how different comment sections might be worded
to help consistency.
* The item in parentheses is what should change between comments, the rest should stay to be consistent. Parts
of each phrase may be optional and are wrapped in brackets "\[optional\]".
* Not all functions need to use a standard phrase or even follow the general format but it should be
used when there are multiple functions that perform similar actions or have similar ideas but are
just performed on different objects.
* If there are only a couple of functions that use similar ideas, they do not need to be added
to this list and instead just refer to other parts of the file. This list is primarily for multi-file consistencies.
* Each list item will start more specific and get broader as you go down.

## Driver Functions
```C
//======================================================================================//{
//  File: driver.c
//  Overview: Provides functions for initializing, configuring, and using the (object) peripheral.
//  Overview: Description of the driver's purpose.
//  Features:
//      - List of features (should describe important functions)
//  Notes: Additional notes that user might/should want to know. (None)
//======================================================================================//}

//============================================================================//{
//  Overview: Enable or disable interrupts for (object).
//  Overview: Initialize the specified (object) according to settings in (object).
//  Overview: Initialize the (object).
//  Overview: (action) the specified (object) according to settings in (object).
//  Overview: (action) the specified (object)'s (object).
//  Overview: (action) with interrupts the (object).
//  Overview: (action) the (object) from the specified (object).
//  Overview: (action) the (object).
//  Overview: Brief description of function

//  Parameters:
//      p_(object)_handle: Structure with the corresponding settings to configure the (object).
//      (object)_conf: Structure with the corresponding settings to configure the (object).
//      p_(object)x: (object) register structure macro.
//      var name: brief overview
//          OPTIONS_x (x-x)

//  Return:
//      var type: brief overview
//          OPTIONS_x (x-x)

//  Note: This function should be called by the applications IT handler.
//============================================================================//}
```

## Test Functions
```C
//======================================================================================//{
//  File: driver_test.h
//  Overview: Tests some of the (object) functionality.
//  Overview: Description of the test driver's purpose.
//  Tests:
//      - List of tests (should describe test functions)
//  Notes: Additional notes that user might/should want to know. (None)
//======================================================================================//}

//============================================================================//{
//  Overview: Initialize (object) [with (extra)] and/to (result).
//  Overview: Test (object) with/by (tool).
//  Overview: What is being tested and how.
//  Overview: Brief description what is being tested and how/with what.

//  Test goals:
//      - (object) initialization [with (extra)]
//      - (object) (action)
//      - Detailed list of test goals.

//  Setup:
//      - (pin) -> (line/tool/pin)[: (reason)] [via x]
//      - (pin) [(function)] -> (line/tool/pin) [(function)] [via x]
//      - Detailed list of items that need to be set up and how they should be. (None)

//  Expectations: [When (action),] (object) should (action).
//  Expectations: What should be expected from the test.
//============================================================================//}
```
