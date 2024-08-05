#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/log1pf_data.h"
#include "math_test_data/log1p_data.h"

using namespace testing::ext;

class MathLog1pTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: log1p_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the log1p interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog1pTest, log1p_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_log1pData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_log1pData[i].expected, log1p(g_log1pData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: log1p_002
 * @tc.desc: When the value is -1.0, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog1pTest, log1p_002, TestSize.Level1)
{
    EXPECT_EQ(-HUGE_VAL, log1p(-1.0));
}

/**
 * @tc.name: log1p_003
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog1pTest, log1p_003, TestSize.Level1)
{
    EXPECT_TRUE(isnan(log1p(-HUGE_VAL)));
    EXPECT_TRUE(isnan(log1p(nan(""))));
}

/**
 * @tc.name: log1p_004
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog1pTest, log1p_004, TestSize.Level1)
{
    EXPECT_TRUE(isinf(log1p(HUGE_VAL)));
}

/**
 * @tc.name: log1p_005
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog1pTest, log1p_005, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0, log1p(M_E - 1.0));
}

/**
 * @tc.name: log1pf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the log1pf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog1pTest, log1pf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_log1pfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_log1pfData[i].expected, log1pf(g_log1pfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: log1pf_002
 * @tc.desc: When the float value is -1.0f, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog1pTest, log1pf_002, TestSize.Level1)
{
    EXPECT_EQ(-HUGE_VAL, log1pf(-1.0f));
}

/**
 * @tc.name: log1pf_003
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog1pTest, log1pf_003, TestSize.Level1)
{
    EXPECT_TRUE(isnan(log1pf(-HUGE_VAL)));
    EXPECT_TRUE(isnan(log1pf(nanf(""))));
}

/**
 * @tc.name: log1pf_004
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog1pTest, log1pf_004, TestSize.Level1)
{
    EXPECT_TRUE(isinf(log1pf(HUGE_VAL)));
}

/**
 * @tc.name: log1pf_005
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLog1pTest, log1pf_005, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0f, log1pf(static_cast<float>(M_E) - 1.0f));
}