#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/atan2f_data.h"
#include "math_test_data/atan2_data.h"

using namespace testing::ext;

class MathAtan2Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: atan2_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the atan2 interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAtan2Test, atan2_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_atan2Data) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_atan2Data[i].expected, atan2(g_atan2Data[i].input1, g_atan2Data[i].input2), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: atan2_002
* @tc.desc: When the parameter of atan2 is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAtan2Test, atan2_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, atan2(0.0, 0.0));
}

/**
* @tc.name: atan2f_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the atan2f interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAtan2Test, atan2f_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_atan2fData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_atan2fData[i].expected, atan2f(g_atan2fData[i].input1, g_atan2fData[i].input2), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: atan2f_002
* @tc.desc: When the parameter of atan2f is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAtan2Test, atan2f_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, atan2f(0.0f, 0.0f));
}

/**
* @tc.name: atan2l_001
* @tc.desc: When the parameter of atan2l is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAtan2Test, atan2l_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, atan2l(0.0L, 0.0L));
}