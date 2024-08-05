#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathYnTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: yn_001
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathYnTest, yn_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(-HUGE_VAL, yn(4, 0.0));
    EXPECT_DOUBLE_EQ(-33.278423028972114, yn(4, 1.0));
}

/**
 * @tc.name: ynf_001
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathYnTest, ynf_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(-HUGE_VALF, ynf(4, 0.0f));
    EXPECT_FLOAT_EQ(-33.278423f, ynf(4, 1.0f));
}