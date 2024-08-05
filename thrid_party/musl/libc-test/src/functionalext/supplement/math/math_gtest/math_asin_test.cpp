#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/asin_data.h"
#include "math_test_data/asinf_data.h"

using namespace testing::ext;

class MathAsinTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: asin_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the asin interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAsinTest, asin_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_asinData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_asinData[i].expected, asin(g_asinData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: asin_002
* @tc.desc: When the parameter of asin is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAsinTest, asin_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, asin(0.0));
}

/**
* @tc.name: asinf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the asinf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAsinTest, asinf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_asinfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_asinfData[i].expected, asinf(g_asinfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: asinf_002
* @tc.desc: When the parameter of asinf is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAsinTest, asinf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, asinf(0.0f));
}

/**
* @tc.name: asinl_001
* @tc.desc: When the parameter of asinl is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAsinTest, asinl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, asinl(0.0L));
}