// Used to test unity testing framework and fake function framework

#include "sample_math.h"

uint32_t extern_add(uint32_t num1, uint32_t num2);

float average(float const float_list[], uint32_t size_of_list)
{
    float avg = 0;
    for (uint32_t i = 0; i < size_of_list; i++) {
        avg += float_list[i];
    }

    return (avg / (float)size_of_list);
}

uint32_t add(uint32_t num1, uint32_t num2)
{

    extern_add(num1, num2);
}