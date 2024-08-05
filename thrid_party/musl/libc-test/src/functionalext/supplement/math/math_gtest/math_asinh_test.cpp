#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/asinhf_data.h"
#include "math_test_data/asinh_data.h"

using namespace testing::ext;

class MathAsinhTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: asinh_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the asinh interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAsinhTest, asinh_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_asinhData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_asinhData[i].expected, asinh(g_asinhData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: asinh_002
* @tc.desc: When the parameter of asinh is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAsinhTest, asinh_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, asinh(0.0));
}

/**
* @tc.name: asinhf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the asinhf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAsinhTest, asinhf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_asinhfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_asinhfData[i].expected, asinhf(g_asinhfData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: asinhf_002
* @tc.desc: When the parameter of asinhf is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAsinhTest, asinhf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, asinhf(0.0f));
}

/**
* @tc.name: asinhl_001
* @tc.desc: When the parameter of asinhl is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAsinhTest, asinhl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, asinhl(0.0L));
}