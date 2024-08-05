#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathScalblnTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: scalbln_001
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalblnTest, scalbln_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(32.0, scalbln(4.0, 3L));
}

/**
 * @tc.name: scalblnf_001
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalblnTest, scalblnf_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(32.0f, scalblnf(4.0f, 3L));
}

/**
 * @tc.name: scalblnl_001
 * @tc.desc: When the value long double is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalblnTest, scalblnl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(32.0L, scalblnl(4.0L, 3L));
}