#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathLgammarTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: lgamma_r_001
 * @tc.desc: The goal of this test case is to validate lgamma_r function can correctly
 *           calculate the logarithm of the lgamma_r function and store the symbols in sign.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammarTest, lgamma_r_001, TestSize.Level1)
{
    int signResult = 0;
    EXPECT_DOUBLE_EQ(log(1.0), lgamma_r(2.0, &signResult));
    EXPECT_EQ(1, signResult);
}

/**
 * @tc.name: lgamma_r_002
 * @tc.desc: This test is used to detect whether the set parameters meet expectations when
 *           they are positive.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammarTest, lgamma_r_002, TestSize.Level1)
{
    int signResult = 0;
    EXPECT_DOUBLE_EQ(HUGE_VAL, lgamma_r(0.0, &signResult));
    EXPECT_EQ(1, signResult);
}

/**
 * @tc.name: lgamma_r_003
 * @tc.desc: This test is used to detect whether the set parameter meets expectations when
 *           it is negative.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammarTest, lgamma_r_003, TestSize.Level1)
{
    int signResult = 0;
    EXPECT_DOUBLE_EQ(HUGE_VAL, lgamma_r(-0.0, &signResult));
    EXPECT_EQ(-1, signResult);
}

/**
 * @tc.name: lgammaf_r_001
 * @tc.desc: The goal of this test case is to validate lgammaf_r function can correctly
 *           calculate the logarithm of the lgammaf_r function and store the symbols in sign.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammarTest, lgammaf_r_001, TestSize.Level1)
{
    int signResult = 0;
    EXPECT_DOUBLE_EQ(log(1.0f), lgammaf_r(2.0f, &signResult));
    EXPECT_EQ(1, signResult);
}

/**
 * @tc.name: lgammaf_r_002
 * @tc.desc: This test is used to detect whether the set parameters meet expectations when
 *           they are positive.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammarTest, lgammaf_r_002, TestSize.Level1)
{
    int signResult = 0;
    EXPECT_DOUBLE_EQ(HUGE_VAL, lgammaf_r(0.0f, &signResult));
    EXPECT_EQ(1, signResult);
}

/**
 * @tc.name: lgammaf_r_003
 * @tc.desc: This test is used to detect whether the set parameter meets expectations when
 *           it is negative.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammarTest, lgammaf_r_003, TestSize.Level1)
{
    int signResult = 0;
    EXPECT_DOUBLE_EQ(HUGE_VAL, lgammaf_r(-0.0f, &signResult));
    EXPECT_EQ(-1, signResult);
}

/**
 * @tc.name: lgammal_r_001
 * @tc.desc: The goal of this test case is to validate lgammal_r function can correctly
 *           calculate the logarithm of the lgammal_r function and store the symbols in sign.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammarTest, lgammal_r_001, TestSize.Level1)
{
    int signResult = 0;
    EXPECT_DOUBLE_EQ(log(1.0L), lgamma_r(2.0L, &signResult));
    EXPECT_EQ(1, signResult);
}

/**
 * @tc.name: lgammal_r_002
 * @tc.desc: This test is used to detect whether the set parameters meet expectations when
 *           they are positive.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammarTest, lgammal_r_002, TestSize.Level1)
{
    int signResult = 0;
    EXPECT_DOUBLE_EQ(HUGE_VAL, lgammal_r(0.0L, &signResult));
    EXPECT_EQ(1, signResult);
}

/**
 * @tc.name: lgammal_r_003
 * @tc.desc: This test is used to detect whether the set parameter meets expectations when
 *           it is negative.
 * @tc.type: FUNC
 */
HWTEST_F(MathLgammarTest, lgammal_r_003, TestSize.Level1)
{
    int signResult = 0;
    EXPECT_DOUBLE_EQ(HUGE_VAL, lgammal_r(-0.0L, &signResult));
    EXPECT_EQ(-1, signResult);
}