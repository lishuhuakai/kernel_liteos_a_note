#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathTgammaTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: tgamma_001
 * @tc.desc: When the parameter of tgamma is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathTgammaTest, tgamma_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0, tgamma(2.0));
}

/**
 * @tc.name: tgammaf_001
 * @tc.desc: When the parameter of tgammaf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathTgammaTest, tgammaf_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(1.0f, tgammaf(2.0f));
}

/**
 * @tc.name: tgammal_001
 * @tc.desc: When the parameter of tgammal is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathTgammaTest, tgammal_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0L, tgammal(2.0L));
}