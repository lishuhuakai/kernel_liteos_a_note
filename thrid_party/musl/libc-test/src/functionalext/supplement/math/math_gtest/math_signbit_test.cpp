#include <gtest/gtest.h>
#include <math.h>

using namespace testing::ext;

class MathSignbitTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __signbit_001
 * @tc.desc: When the input parameter is valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSignbitTest, __signbit_001, TestSize.Level1)
{
    EXPECT_EQ(0, __signbit(0.0));
    EXPECT_EQ(0, __signbit(1.0));
    EXPECT_NE(0, __signbit(-1.0));
}

/**
 * @tc.name: __signbitf_001
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSignbitTest, __signbitf_001, TestSize.Level1)
{
    EXPECT_EQ(0, __signbitf(0.0f));
    EXPECT_EQ(0, __signbitf(1.0f));
    EXPECT_NE(0, __signbitf(-1.0f));
}

/**
 * @tc.name: __signbitl_001
 * @tc.desc: When the input parameter is of long double type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSignbitTest, __signbitl_001, TestSize.Level1)
{
    EXPECT_EQ(0L, __signbitl(0.0L));
    EXPECT_EQ(0L, __signbitl(1.0L));
    EXPECT_NE(0L, __signbitl(-1.0L));
}