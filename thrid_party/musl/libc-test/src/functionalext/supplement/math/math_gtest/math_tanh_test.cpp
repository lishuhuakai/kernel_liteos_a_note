#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/tanhf_data.h"
#include "math_test_data/tanh_data.h"

using namespace testing::ext;

class MathTanhTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: tanh_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range
 *           of the tanh interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathTanhTest, tanh_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_tanhData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_tanhData[i].expected, tanh(g_tanhData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: tanh_002
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathTanhTest, tanh_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, tanh(0.0));
}

/**
 * @tc.name: tanhf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range
 *           of the tanhf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathTanhTest, tanhf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_tanhfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_tanhfData[i].expected, tanhf(g_tanhfData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: tanhf_002
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathTanhTest, tanhf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, tanhf(0.0f));
}

/**
 * @tc.name: tanhl_001
 * @tc.desc: When the long double value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathTanhTest, tanhl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, tanhl(0.0L));
}