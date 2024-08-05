#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/expm1f_data.h"
#include "math_test_data/expm1_data.h"

using namespace testing::ext;

class MathExpm1Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: expm1_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the expm1 interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathExpm1Test, expm1_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_expm1Data) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_expm1Data[i].expected, expm1(g_expm1Data[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: expm1_002
 * @tc.desc: When the parameter of expm1 is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathExpm1Test, expm1_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(M_E - 1.0, expm1(1.0));
}

/**
 * @tc.name: expm1f_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the expm1f interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathExpm1Test, expm1f_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_expm1fData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_expm1fData[i].expected, expm1f(g_expm1fData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: expm1f_002
 * @tc.desc: When the parameter of expm1f is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathExpm1Test, expm1f_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(static_cast<float>(M_E) - 1.0f, expm1f(1.0f));
}

/**
 * @tc.name: expm1l_001
 * @tc.desc: When the parameter of expm1l is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathExpm1Test, expm1l_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(M_E - 1.0L, expm1l(1.0L));
}