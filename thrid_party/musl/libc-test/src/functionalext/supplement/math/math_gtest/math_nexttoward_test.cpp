#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathNexttowardTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: nexttoward_001
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathNexttowardTest, nexttoward_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, nexttoward(0.0, 0.0L));
    EXPECT_DOUBLE_EQ(4.9406564584124654e-324, nexttoward(0.0, 1.0L));
    EXPECT_DOUBLE_EQ(-4.9406564584124654e-324, nexttoward(0.0, -1.0L));
}

/**
 * @tc.name: nexttowardf_001
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathNexttowardTest, nexttowardf_001, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, nexttowardf(0.0f, 0.0L));
    EXPECT_FLOAT_EQ(1.4012985e-45f, nexttowardf(0.0f, 1.0L));
    EXPECT_FLOAT_EQ(-1.4012985e-45f, nexttowardf(0.0f, -1.0L));
}

/**
 * @tc.name: nexttowardl_001
 * @tc.desc: When the input parameter is of long double type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathNexttowardTest, nexttowardl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, nexttowardl(0.0L, 0.0L));
    long double minPositive = ldexpl(1.0L, LDBL_MIN_EXP - LDBL_MANT_DIG);
    EXPECT_DOUBLE_EQ(minPositive, nexttowardl(0.0L, 1.0L));
    EXPECT_DOUBLE_EQ(-minPositive, nexttowardl(0.0L, -1.0L));
}
