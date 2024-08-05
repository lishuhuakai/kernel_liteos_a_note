#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathJ1Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: j1_001
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathJ1Test, j1_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, j1(0.0));
    EXPECT_DOUBLE_EQ(0.44005058574493355, j1(1.0));
}

/**
 * @tc.name: j1f_001
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathJ1Test, j1f_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, j1f(0.0f));
    EXPECT_FLOAT_EQ(0.44005057f, j1f(1.0f));
}