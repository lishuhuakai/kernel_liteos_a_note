#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/log2f_data.h"
#include "math_test_data/log2_data.h"

using namespace testing::ext;

class MathLog2Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: log2_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the log2 interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog2Test, log2_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_log2Data) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_log2Data[i].expected, log2(g_log2Data[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: log2_002
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog2Test, log2_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(10.0, log2(1024.0));
}

/**
 * @tc.name: log2f_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the log2f interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog2Test, log2f_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_log2fData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_log2fData[i].expected, log2f(g_log2fData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: log2f_002
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog2Test, log2f_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(10.0f, log2f(1024.0f));
}

/**
 * @tc.name: log2l_001
 * @tc.desc: When the long double value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog2Test, log2l_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(10.0L, log2l(1024.0L));
}