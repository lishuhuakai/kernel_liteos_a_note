#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/hypotf_data.h"
#include "math_test_data/hypot_data.h"

using namespace testing::ext;

class MathHypotTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: hypot_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the hypot interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathHypotTest, hypot_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_hypotData) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_hypotData[i].expected, hypot(g_hypotData[i].input1, g_hypotData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: hypot_002
 * @tc.desc: When the value is 6.0, 8.0, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathHypotTest, hypot_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(10.0, hypot(6.0, 8.0));
}

/**
 * @tc.name: hypot_003
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathHypotTest, hypot_003, TestSize.Level1)
{
    EXPECT_EQ(HUGE_VAL, hypot(4.0, HUGE_VAL));
    EXPECT_EQ(HUGE_VAL, hypot(4.0, -HUGE_VAL));
    EXPECT_EQ(HUGE_VAL, hypot(HUGE_VAL, 5.0));
    EXPECT_EQ(HUGE_VAL, hypot(-HUGE_VAL, 5.0));
}

/**
 * @tc.name: hypot_004
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathHypotTest, hypot_004, TestSize.Level1)
{
    EXPECT_TRUE(isnan(hypot(6.0, nan(""))));
    EXPECT_TRUE(isnan(hypot(nan(""), 8.0)));
}

/**
 * @tc.name: hypotf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the hypotf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathHypotTest, hypotf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_hypotfData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_hypotfData[i].expected, hypotf(g_hypotfData[i].input1, g_hypotfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: hypotf_002
 * @tc.desc: When the value is 6.0f, 8.0f, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathHypotTest, hypotf_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(10.0f, hypotf(6.0f, 8.0f));
}

/**
 * @tc.name: hypotf_003
 * @tc.desc:  This test is used to detect whether expectations are met when the parameter
 *           is equal to infinity.
 * @tc.type: FUNC
 */
HWTEST_F(MathHypotTest, hypotf_003, TestSize.Level1)
{
    EXPECT_EQ(HUGE_VAL, hypotf(4.0f, HUGE_VAL));
    EXPECT_EQ(HUGE_VAL, hypotf(4.0f, -HUGE_VAL));
    EXPECT_EQ(HUGE_VAL, hypotf(HUGE_VAL, 5.0f));
    EXPECT_EQ(HUGE_VAL, hypotf(-HUGE_VAL, 5.0f));
}

/**
 * @tc.name: hypotf_004
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathHypotTest, hypotf_004, TestSize.Level1)
{
    EXPECT_TRUE(isnan(hypotf(6.0f, nan(""))));
    EXPECT_TRUE(isnan(hypotf(nan(""), 8.0f)));
}