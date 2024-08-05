#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathY0Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: y0_001
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathY0Test, y0_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(-HUGE_VAL, y0(0.0));
    EXPECT_DOUBLE_EQ(0.08825696421567697, y0(1.0));
}

/**
 * @tc.name: y0f_001
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathY0Test, y0f_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(-HUGE_VALF, y0f(0.0f));
    EXPECT_FLOAT_EQ(0.088256963f, y0f(1.0f));
}