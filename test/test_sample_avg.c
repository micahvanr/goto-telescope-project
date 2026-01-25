#include "../tools/unity/src/unity.h"
#include "../src/common/sample_avg.h"

void setUp(void) {
    
}

void tearDown(void) {

}

void test_sample_avg(void) {
    TEST_FAIL_MESSAGE("TEST");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_sample_avg);

    return UNITY_END();
}