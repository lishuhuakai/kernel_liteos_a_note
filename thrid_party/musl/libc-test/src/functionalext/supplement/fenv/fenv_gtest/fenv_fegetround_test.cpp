#include <fenv.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class FenvFegetroundTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fegetround_001
 * @tc.desc: Set the floating-point rounding mode to "nearest mode" using fesetround, retrieves the current rounding
 *           mode using fegetround, and verifies whether it is correct. Then, it performs some floating-point
 *           calculations and verifies whether the results are as expected.
 * @tc.type: FUNC
 */
HWTEST_F(FenvFegetroundTest, fegetround_001, TestSize.Level1)
{
    fesetround(FE_TONEAREST);
    EXPECT_TRUE(FE_TONEAREST == fegetround());
    float x = 1.968750f + 0x1.0p23f;
    EXPECT_FLOAT_EQ(8388610.0f, x);
    x -= 0x1.0p23f;
    EXPECT_FLOAT_EQ(2.0f, x);
}