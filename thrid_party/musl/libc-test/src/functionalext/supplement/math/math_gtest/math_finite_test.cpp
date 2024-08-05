#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathFiniteTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: finite_001
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFiniteTest, finite_001, TestSize.Level1)
{
    EXPECT_TRUE(finite(100.0));
}

/**
 * @tc.name: finite_002
 * @tc.desc: When the input parameters are infinite, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFiniteTest, finite_002, TestSize.Level1)
{
    EXPECT_FALSE(finite(HUGE_VAL));
    EXPECT_FALSE(finite(-HUGE_VAL));
}
