#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/atanf_data.h"
#include "math_test_data/atan_data.h"

using namespace testing::ext;

class MathAtanTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: atan_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the atan interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAtanTest, atan_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_atanData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_atanData[i].expected, atan(g_atanData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: atan_002
* @tc.desc: When the parameter of atan is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAtanTest, atan_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, atan(0.0));
}

/**
* @tc.name: atanf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the atanf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAtanTest, atanf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_atanfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_atanfData[i].expected, atanf(g_atanfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: atanf_002
* @tc.desc: When the parameter of atanf is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAtanTest, atanf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, atanf(0.0f));
}

/**
* @tc.name: atanl_001
* @tc.desc: When the parameter of atanl is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAtanTest, atanl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, atanl(0.0L));
}