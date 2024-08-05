#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/logf_data.h"
#include "math_test_data/log_data.h"

using namespace testing::ext;

class MathLogTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: log_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the log interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogTest, log_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_logData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_logData[i].expected, log(g_logData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: log_002
 * @tc.desc: When the value is M_E, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogTest, log_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0, log(M_E));
}

/**
 * @tc.name: logf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the logf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogTest, logf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_logfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_logfData[i].expected, logf(g_logfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: logf_002
 * @tc.desc: When the float value is M_E, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogTest, logf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(1.0f, logf(static_cast<float>(M_E)));
}

/**
 * @tc.name: logl_001
 * @tc.desc: When the long double value is M_E, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLogTest, logl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0L, logl(M_E));
}