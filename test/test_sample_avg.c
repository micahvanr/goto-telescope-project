#include "../tools/unity/src/unity.h"
#include "../src/common/sample_avg.h"

void setUp(void) {
    
}

void tearDown(void) {

}

void test_sample_avg(void) {
    float array[3] = {5, 5, 5,};
    float avg = average(array, 3);
    TEST_ASSERT_EQUAL_FLOAT(avg, 5);
}

void test_fail(void) {
    TEST_FAIL_MESSAGE("Testing fail test");
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_sample_avg);
    //RUN_TEST(test_fail);

    return UNITY_END();
}