#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/exp2f_data.h"
#include "math_test_data/exp2_data.h"

using namespace testing::ext;

class MathExp2Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: exp2_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the exp2 interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathExp2Test, exp2_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_exp2Data) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_exp2Data[i].expected, exp2(g_exp2Data[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: exp2_002
 * @tc.desc: When the parameter of exp2 is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathExp2Test, exp2_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(8.0, exp2(3.0));
}

/**
 * @tc.name: exp2_003
 * @tc.desc: The test code can check whether the expo2() and log2() functions correctly
 *           implement exponential and logarithmic operations on the OpenBSD system, and return the expected results.
 * @tc.type: FUNC
 */
HWTEST_F(MathExp2Test, exp2_003, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(10.0, exp2(log2(10)));
    EXPECT_FLOAT_EQ(10.0f, exp2f(log2f(10)));
    EXPECT_DOUBLE_EQ(10.0L, exp2l(log2l(10)));
}

/**
 * @tc.name: exp2f_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the exp2f interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathExp2Test, exp2f_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_exp2fData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_exp2fData[i].expected, exp2f(g_exp2fData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: exp2f_002
 * @tc.desc: When the parameter of exp2f is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathExp2Test, exp2f_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(8.0f, exp2f(3.0f));
}

/**
 * @tc.name: exp2l_001
 * @tc.desc: When the parameter of exp2l is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathExp2Test, exp2l_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(8.0L, exp2l(3.0L));
}