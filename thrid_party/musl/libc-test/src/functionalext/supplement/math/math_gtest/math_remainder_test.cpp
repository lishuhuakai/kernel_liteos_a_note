#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/remainderf_data.h"
#include "math_test_data/remainder_data.h"

using namespace testing::ext;

class MathRemainderTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: remainder_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the remainder interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemainderTest, remainder_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_remainderData) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_remainderData[i].expected, remainder(g_remainderData[i].input1,
            g_remainderData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: remainder_002
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemainderTest, remainder_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(4.0, remainder(15.0, 11.0));
}

/**
 * @tc.name: remainder_003
 * @tc.desc: When the value is nan, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemainderTest, remainder_003, TestSize.Level1)
{
    EXPECT_TRUE(isnan(remainder(nan(""), 11.0)));
    EXPECT_TRUE(isnan(remainder(13.0, nan(""))));
}

/**
 * @tc.name: remainder_004
 * @tc.desc: When the value is infinite, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemainderTest, remainder_004, TestSize.Level1)
{
    EXPECT_TRUE(isnan(remainder(HUGE_VAL, 14.0)));
    EXPECT_TRUE(isnan(remainder(-HUGE_VAL, 14.0)));
}

/**
 * @tc.name: remainder_005
 * @tc.desc: When the dividend is a non-zero constant and the divisor is zero, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemainderTest, remainder_005, TestSize.Level1)
{
    EXPECT_TRUE(isnan(remainder(15.0, 0.0)));
}

/**
 * @tc.name: remainderf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the remainderf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemainderTest, remainderf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_remainderfData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_remainderfData[i].expected, remainderf(g_remainderfData[i].input1,
            g_remainderfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: remainderf_002
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemainderTest, remainderf_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(4.0f, remainderf(15.0f, 11.0f));
}

/**
 * @tc.name: remainderf_003
 * @tc.desc: When the value is nan, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemainderTest, remainderf_003, TestSize.Level1)
{
    EXPECT_TRUE(isnan(remainderf(nanf(""), 11.0f)));
    EXPECT_TRUE(isnan(remainderf(13.0f, nanf(""))));
}

/**
 * @tc.name: remainderf_004
 * @tc.desc: When the value is infinite, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemainderTest, remainderf_004, TestSize.Level1)
{
    EXPECT_TRUE(isnan(remainderf(HUGE_VAL, 14.0f)));
    EXPECT_TRUE(isnan(remainderf(-HUGE_VAL, 14.0f)));
}

/**
 * @tc.name: remainderf_005
 * @tc.desc: When the dividend is a non-zero constant and the divisor is zero, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemainderTest, remainderf_005, TestSize.Level1)
{
    EXPECT_TRUE(isnan(remainderf(15.0f, 0.0f)));
}