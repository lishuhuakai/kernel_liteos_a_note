#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathJnTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: jn_001
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathJnTest, jn_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, jn(4, 0.0));
    EXPECT_DOUBLE_EQ(0.0024766389641099553, jn(4, 1.0));
}

/**
 * @tc.name: jnf_001
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathJnTest, jnf_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, jnf(4, 0.0f));
    EXPECT_FLOAT_EQ(0.0024766389f, jnf(4, 1.0f));
}