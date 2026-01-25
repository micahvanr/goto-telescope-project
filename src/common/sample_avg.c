// Used to test unity testing framework

#include "sample_avg.h"

float average(float const float_list[], uint32_t size_of_list) {
    float avg = 0;
    for (uint32_t i = 0; i < size_of_list; i++) {
        avg += float_list[i];
    }

    return (avg / (float) size_of_list);
}