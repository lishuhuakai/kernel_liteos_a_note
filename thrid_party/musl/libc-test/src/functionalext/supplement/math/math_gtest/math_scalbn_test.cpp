#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/scalbnf_data.h"
#include "math_test_data/scalbn_data.h"

using namespace testing::ext;

class MathScalbnTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: scalbn_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the scalbn interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalbnTest, scalbn_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_scalbnData) / sizeof(DataDoubleDoubleInt); i++) {
        bool testResult = DoubleUlpCmp(g_scalbnData[i].expected,
            scalbn(g_scalbnData[i].input1, g_scalbnData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: scalbn_002
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalbnTest, scalbn_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(40.0, scalbn(5.0, 3));
}

/**
 * @tc.name: scalbnf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the scalbnf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalbnTest, scalbnf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_scalbnfData) / sizeof(DataFloatFloatInt); i++) {
        bool testResult = FloatUlpCmp(g_scalbnfData[i].expected,
            scalbnf(g_scalbnfData[i].input1, g_scalbnfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: scalbnf_002
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalbnTest, scalbnf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(40.0f, scalbnf(5.0f, 3));
}

/**
 * @tc.name: scalbnl_001
 * @tc.desc: When the long double value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalbnTest, scalbnl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(40.0L, scalbnl(5.0L, 3));
}