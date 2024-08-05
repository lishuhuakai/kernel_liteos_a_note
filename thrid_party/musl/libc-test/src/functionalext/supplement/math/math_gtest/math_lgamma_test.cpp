#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathLgammaTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: lgamma_001
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammaTest, lgamma_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(log(1.0), lgamma(2.0));
}

/**
 * @tc.name: lgammaf_001
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammaTest, lgammaf_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(logf(1.0f), lgammaf(2.0f));
}

/**
 * @tc.name: lgammal_001
 * @tc.desc: When the long double value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammaTest, lgammal_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(logl(1.0L), lgammal(2.0L));
}