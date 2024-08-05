#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/logbf_data.h"
#include "math_test_data/logb_data.h"

using namespace testing::ext;

class MathLogbTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: logb_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the logb interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logb_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_logbData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_logbData[i].expected, logb(g_logbData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: logb_002
 * @tc.desc: When the input parameter is 0, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logb_002, TestSize.Level1)
{
    EXPECT_EQ(-HUGE_VAL, logb(0.0));
}

/**
 * @tc.name: logb_003
 * @tc.desc: When the input parameter is NaN, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logb_003, TestSize.Level1)
{
    EXPECT_TRUE(isnan(logb(nan(""))));
}

/**
 * @tc.name: logb_004
 * @tc.desc: When the input parameter is positive infinity or negative infinity, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logb_004, TestSize.Level1)
{
    EXPECT_TRUE(isinf(logb(HUGE_VAL)));
    EXPECT_TRUE(isinf(logb(-HUGE_VAL)));
}

/**
 * @tc.name: logb_005
 * @tc.desc: When the input parameter is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logb_005, TestSize.Level1)
{
    EXPECT_EQ(0.0, logb(1.0));
    EXPECT_EQ(3.0, logb(10.0));
}

/**
 * @tc.name: logbf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the logbf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logbf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_logbfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_logbfData[i].expected, logbf(g_logbfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: logbf_002
 * @tc.desc: When the input parameter is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logbf_002, TestSize.Level1)
{
    EXPECT_EQ(-HUGE_VAL, logbf(0.0f));
}

/**
 * @tc.name: logbf_003
 * @tc.desc: When the input parameter is NaN, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logbf_003, TestSize.Level1)
{
    EXPECT_TRUE(isnan(logbf(nanf(""))));
}

/**
 * @tc.name: logbf_004
 * @tc.desc: When the input parameter is positive infinity or negative infinity, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logbf_004, TestSize.Level1)
{
    EXPECT_TRUE(isinf(logbf(HUGE_VAL)));
    EXPECT_TRUE(isinf(logbf(-HUGE_VAL)));
}

/**
 * @tc.name: logbf_005
 * @tc.desc: When the input parameter is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logbf_005, TestSize.Level1)
{
    EXPECT_EQ(0.0f, logbf(1.0f));
    EXPECT_EQ(3.0f, logbf(10.0f));
}

/**
 * @tc.name: logbl_001
 * @tc.desc: When the input parameter is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logbl_001, TestSize.Level1)
{
    EXPECT_EQ(-HUGE_VAL, logbl(0.0L));
}

/**
 * @tc.name: logbl_002
 * @tc.desc: When the input parameter is NaN, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logbl_002, TestSize.Level1)
{
    EXPECT_TRUE(isnan(logbl(nanl(""))));
}

/**
 * @tc.name: logbl_003
 * @tc.desc: When the input parameter is positive infinity or negative infinity, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logbl_003, TestSize.Level1)
{
    EXPECT_TRUE(isinf(logbl(HUGE_VAL)));
    EXPECT_TRUE(isinf(logbl(-HUGE_VAL)));
}

/**
 * @tc.name: logbl_004
 * @tc.desc: When the input parameter is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogbTest, logbl_004, TestSize.Level1)
{
    EXPECT_EQ(0.0L, logbl(1.0L));
    EXPECT_EQ(3.0L, logbl(10.0L));
}