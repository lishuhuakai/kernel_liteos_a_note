#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathErfTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: erf_001
 * @tc.desc: When the parameter of erf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathErfTest, erf_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.84270079294971489, erf(1.0));
}

/**
 * @tc.name: erff_001
 * @tc.desc: When the parameter of erff is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathErfTest, erff_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.84270078f, erff(1.0f));
}

/**
 * @tc.name: erfl_001
 * @tc.desc: When the parameter of erfl is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathErfTest, erfl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.84270079294971489L, erfl(1.0L));
}