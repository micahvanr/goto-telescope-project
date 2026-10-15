//======================================================================================//{
//  File: sample_math.c
//  Overview: Sample math functions to test with unit testing frameworks.
//  Features:
//      Compute average
//      Compute add
//  Notes: Used to test unity testing framework and fake function framework
//======================================================================================//}

#ifndef SAMPLE_MATH_H
#define SAMPLE_MATH_H

//} Include Statements
//=========================================//{

#include <stdint.h>

//======================================================================================//}
//                  Public Function Prototypes
//======================================================================================//{

float average(float const float_list[], uint32_t size_of_list);
uint32_t add(uint32_t num1, uint32_t num2);

#endif

// *End of file*
