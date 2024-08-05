#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathDremTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: drem_001
 * @tc.desc: When the parameter of drem is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathDremTest, drem_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(4.0, drem(15.0, 11.0));
}

/**
 * @tc.name: dremf_001
 * @tc.desc: When the parameter of dremf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathDremTest, dremf_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(4.0f, dremf(15.0f, 11.0f));
}