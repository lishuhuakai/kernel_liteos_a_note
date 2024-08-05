#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/log10f_data.h"
#include "math_test_data/log10_data.h"

using namespace testing::ext;

class MathLog10Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: log10_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the log10 interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog10Test, log10_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_log10Data) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_log10Data[i].expected, log10(g_log10Data[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: log10_002
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog10Test, log10_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(2.0, log10(100.0));
}

/**
 * @tc.name: log10f_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the log10f interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog10Test, log10f_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_log10fData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_log10fData[i].expected, log10f(g_log10fData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: log10f_002
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog10Test, log10f_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(2.0f, log10f(100.0f));
}

/**
 * @tc.name: log10l_001
 * @tc.desc: When the long double value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog10Test, log10l_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(2.0L, log10l(100.0L));
}