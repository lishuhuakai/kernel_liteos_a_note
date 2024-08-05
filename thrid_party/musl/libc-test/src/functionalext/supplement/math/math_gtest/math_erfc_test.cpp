#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathErfcTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: erfc_001
 * @tc.desc: When the parameter of erfc is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathErfcTest, erfc_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.15729920705028513, erfc(1.0));
}

/**
 * @tc.name: erfcf_001
 * @tc.desc: When the parameter of erfcf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathErfcTest, erfcf_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.15729921f, erfcf(1.0f));
}

/**
 * @tc.name: erfcl_001
 * @tc.desc: When the parameter of erfcl is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathErfcTest, erfcl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.15729920705028513L, erfcl(1.0L));
}