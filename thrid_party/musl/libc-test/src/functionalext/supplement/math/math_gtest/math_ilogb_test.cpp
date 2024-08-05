#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/ilogbf_data.h"
#include "math_test_data/ilogb_data.h"

using namespace testing::ext;

class MathIlogbTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: ilogb_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the ilogb interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogb_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_ilogbData) / sizeof(DataIntDouble); i++) {
        EXPECT_EQ(g_ilogbData[i].expected, ilogb(g_ilogbData[i].input));
    }
}

/**
 * @tc.name: ilogb_002
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogb_002, TestSize.Level1)
{
    EXPECT_EQ(FP_ILOGB0, ilogb(0.0));
}

/**
 * @tc.name: ilogb_003
 * @tc.desc: When the value is nan, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogb_003, TestSize.Level1)
{
    EXPECT_EQ(FP_ILOGBNAN, ilogb(nan("")));
}

/**
 * @tc.name: ilogb_004
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogb_004, TestSize.Level1)
{
    EXPECT_EQ(INT_MAX, ilogb(HUGE_VAL));
    EXPECT_EQ(INT_MAX, ilogb(-HUGE_VAL));
}

/**
 * @tc.name: ilogb_005
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogb_005, TestSize.Level1)
{
    EXPECT_EQ(2, ilogb(4.0));
    EXPECT_EQ(3, ilogb(8.0));
}

/**
 * @tc.name: ilogbf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the ilogbf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogbf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_ilogbfData) / sizeof(DataIntFloat); i++) {
        EXPECT_EQ(g_ilogbfData[i].expected, ilogbf(g_ilogbfData[i].input));
    }
}

/**
 * @tc.name: ilogbf_002
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogbf_002, TestSize.Level1)
{
    EXPECT_EQ(FP_ILOGB0, ilogbf(0.0f));
}

/**
 * @tc.name: ilogbf_003
 * @tc.desc: When the float value is nan, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogbf_003, TestSize.Level1)
{
    EXPECT_EQ(FP_ILOGBNAN, ilogbf(nan("")));
}

/**
 * @tc.name: ilogbf_004
 * @tc.desc: When the input parameter is valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogbf_004, TestSize.Level1)
{
    EXPECT_EQ(INT_MAX, ilogbf(HUGE_VAL));
    EXPECT_EQ(INT_MAX, ilogbf(-HUGE_VAL));
}

/**
 * @tc.name: ilogbf_005
 * @tc.desc: When the float value is 4.0f, 8.0f, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogbf_005, TestSize.Level1)
{
    EXPECT_EQ(2, ilogbf(4.0f));
    EXPECT_EQ(3, ilogbf(8.0f));
}

/**
 * @tc.name: ilogbl_001
 * @tc.desc: When the long double value is 0.0L, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogbl_001, TestSize.Level1)
{
    EXPECT_EQ(FP_ILOGB0, ilogbl(0.0L));
}

/**
 * @tc.name: ilogbl_002
 * @tc.desc: When the long double value is nan, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogbl_002, TestSize.Level1)
{
    EXPECT_EQ(FP_ILOGBNAN, ilogbl(nan("")));
}

/**
 * @tc.name: ilogbl_003
 * @tc.desc: When the input parameter is valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogbl_003, TestSize.Level1)
{
    EXPECT_EQ(INT_MAX, ilogbl(HUGE_VAL));
    EXPECT_EQ(INT_MAX, ilogbl(-HUGE_VAL));
}

/**
 * @tc.name: ilogbl_004
 * @tc.desc: When the long double value is 4.0f, 8.0f, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathIlogbTest, ilogbl_004, TestSize.Level1)
{
    EXPECT_EQ(2, ilogbl(4.0L));
    EXPECT_EQ(3, ilogbl(8.0L));
}