#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathY1Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: y1_001
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathY1Test, y1_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(-HUGE_VAL, y1(0.0));
    EXPECT_DOUBLE_EQ(-0.78121282130028868, y1(1.0));
}

/**
 * @tc.name: y1f_001
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathY1Test, y1f_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(-HUGE_VALF, y1f(0.0f));
    EXPECT_FLOAT_EQ(-0.78121281f, y1f(1.0f));
}