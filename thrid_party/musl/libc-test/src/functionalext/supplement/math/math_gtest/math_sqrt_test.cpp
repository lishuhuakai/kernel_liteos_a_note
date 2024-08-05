#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/sqrtf_data.h"
#include "math_test_data/sqrt_data.h"

using namespace testing::ext;

class MathSqrtTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: sqrt_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the sqrt interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathSqrtTest, sqrt_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_sqrtData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_sqrtData[i].expected, sqrt(g_sqrtData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: sqrt_002
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSqrtTest, sqrt_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(4.0, sqrt(16.0));
}

/**
 * @tc.name: sqrtf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the sqrtf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathSqrtTest, sqrtf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_sqrtfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_sqrtfData[i].expected, sqrtf(g_sqrtfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: sqrtf_002
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSqrtTest, sqrtf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(4.0f, sqrtf(16.0f));
}

/**
 * @tc.name: sqrtl_001
 * @tc.desc: When the long double value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSqrtTest, sqrtl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(4.0L, sqrtl(16.0L));
}