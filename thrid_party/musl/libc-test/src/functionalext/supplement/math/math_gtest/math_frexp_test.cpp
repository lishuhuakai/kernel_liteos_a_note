#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/frexpf_data.h"
#include "math_test_data/frexp_data.h"

using namespace testing::ext;

class MathFrexpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: frexp_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the frexp interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFrexpTest, frexp_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_frexpData) / sizeof(DataDoubleIntDouble); i++) {
        int exp;
        bool testResult = DoubleUlpCmp(g_frexpData[i].expected1, frexp(g_frexpData[i].input, &exp), 1);
        EXPECT_TRUE(testResult);
        EXPECT_EQ(exp, g_frexpData[i].expected2);
    }
}

/**
 * @tc.name: frexp_002
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFrexpTest, frexp_002, TestSize.Level1)
{
    int exponent;
    double inputNum = 2048.0;
    double fraction = frexp(inputNum, &exponent);
    EXPECT_DOUBLE_EQ(2048.0, scalbn(fraction, exponent));
}

/**
 * @tc.name: frexpf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the frexpf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFrexpTest, frexpf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_frexpfData) / sizeof(DataFloatIntFloat); i++) {
        int exp;
        bool testResult = FloatUlpCmp(g_frexpfData[i].expected1, frexpf(g_frexpfData[i].input, &exp), 1);
        EXPECT_TRUE(testResult);
        EXPECT_EQ(exp, g_frexpfData[i].expected2);
    }
}

/**
 * @tc.name: frexpf_002
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFrexpTest, frexpf_002, TestSize.Level1)
{
    int exponent;
    float inputNum = 2048.0f;
    float fraction = frexpf(inputNum, &exponent);
    EXPECT_FLOAT_EQ(2048.0f, scalbnf(fraction, exponent));
}

/**
 * @tc.name: frexpf_003
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFrexpTest, frexpf_003, TestSize.Level1)
{
    int exponent;
    float inputNum = 6.3f;
    float fraction = frexpf(inputNum, &exponent);
    EXPECT_FLOAT_EQ(6.3f, scalbnf(fraction, exponent));
}

/**
 * @tc.name: frexpl_001
 * @tc.desc: When the input parameter is of long double type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFrexpTest, frexpl_001, TestSize.Level1)
{
    int exponent;
    long double inputNum = 2048.0L;
    long double fraction = frexpl(inputNum, &exponent);
    EXPECT_DOUBLE_EQ(2048.0L, scalbnl(fraction, exponent));
}