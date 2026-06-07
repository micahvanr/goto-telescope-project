#include "../tools/unity/src/unity.h"
#include "../tools/fff/fff.h"
#include "../src/common/sample_math.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(uint32_t, extern_add, uint32_t, uint32_t);

void setUp(void) {
    RESET_FAKE(extern_add);
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

void test_add_fake(void) {
    extern_add_fake.return_val = 10;
    uint32_t sum = add(5, 5);
    TEST_ASSERT_EQUAL_UINT32(sum, 10);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_sample_avg);
    RUN_TEST(test_add_fake);
    //RUN_TEST(test_fail);

    return UNITY_END();
}