#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/cosf_data.h"
#include "math_test_data/cos_data.h"

using namespace testing::ext;

class MathCosTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: cos_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the cos interface.
* @tc.type: FUNC
*/
HWTEST_F(MathCosTest, cos_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_cosData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_cosData[i].expected, cos(g_cosData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: cos_002
 * @tc.desc: When the parameter of cos is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathCosTest, cos_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0, cos(0.0));
}

/**
* @tc.name: cosf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the cosf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathCosTest, cosf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_cosfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_cosfData[i].expected, cosf(g_cosfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: cosf_002
 * @tc.desc: When the parameter of cosf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathCosTest, cosf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(1.0f, cosf(0.0f));
}

/**
 * @tc.name: cosl_001
 * @tc.desc: When the parameter of cosl is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathCosTest, cosl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0L, cosl(0.0L));
}