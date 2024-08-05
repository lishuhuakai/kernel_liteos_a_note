#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/sinhf_data.h"
#include "math_test_data/sinh_data.h"

using namespace testing::ext;

class MathSinhTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: sinh_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range
 *           of the sinh interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathSinhTest, sinh_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_sinhData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_sinhData[i].expected, sinh(g_sinhData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: sinh_002
 * @tc.desc: When the value is 0.0, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSinhTest, sinh_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, sinh(0.0));
}

/**
 * @tc.name: sinhf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range
 *           of the sinhf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathSinhTest, sinhf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_sinhfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_sinhfData[i].expected, sinhf(g_sinhfData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: sinhf_002
 * @tc.desc: When the float value is 0.0f, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSinhTest, sinhf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, sinhf(0.0f));
}

/**
 * @tc.name: sinhl_001
 * @tc.desc: When the long double value is 0.0L, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSinhTest, sinhl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, sinhl(0.0L));
}