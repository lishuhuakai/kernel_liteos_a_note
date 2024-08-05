#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathJ0Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: j0_001
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathJ0Test, j0_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0, j0(0.0));
    EXPECT_DOUBLE_EQ(0.76519768655796661, j0(1.0));
}

/**
 * @tc.name: j0f_001
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathJ0Test, j0f_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(1.0f, j0f(0.0f));
    EXPECT_FLOAT_EQ(0.76519769f, j0f(1.0f));
}