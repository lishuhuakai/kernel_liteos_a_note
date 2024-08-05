#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/fabsf_data.h"
#include "math_test_data/fabs_data.h"

using namespace testing::ext;

class MathFabsTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: fabs_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fabs interface.
* @tc.type: FUNC
*/
HWTEST_F(MathFabsTest, fabs_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fabsData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_fabsData[i].expected, fabs(g_fabsData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fabs_002
 * @tc.desc: When the parameter of fabs is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFabsTest, fabs_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(5.0, fabs(-5.0));
}

/**
* @tc.name: fabsf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fabsf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathFabsTest, fabsf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fabsfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_fabsfData[i].expected, fabsf(g_fabsfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fabsf_002
 * @tc.desc: When the parameter of fabsf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFabsTest, fabsf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(5.0f, fabsf(-5.0f));
}

/**
 * @tc.name: fabsl_001
 * @tc.desc: When the parameter of fabsl is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFabsTest, fabsl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(5.0L, fabsl(-5.0L));
}