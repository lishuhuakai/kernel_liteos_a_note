#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/atanhf_data.h"
#include "math_test_data/atanh_data.h"

using namespace testing::ext;

class MathAtanhTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: atanh_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the atanh interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAtanhTest, atanh_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_atanhData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_atanhData[i].expected, atanh(g_atanhData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: atanh_002
* @tc.desc: When the parameter of atanh is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAtanhTest, atanh_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, atanh(0.0));
}

/**
* @tc.name: atanhf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the atanhf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAtanhTest, atanhf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_atanhfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_atanhfData[i].expected, atanhf(g_atanhfData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: atanhf_002
* @tc.desc: When the parameter of atanhf is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAtanhTest, atanhf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, atanhf(0.0f));
}

/**
* @tc.name: atanhl_001
* @tc.desc: When the parameter of atanhl is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAtanhTest, atanhl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, atanhl(0.0L));
}