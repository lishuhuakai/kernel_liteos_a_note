#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/coshf_data.h"
#include "math_test_data/cosh_data.h"

using namespace testing::ext;

class MathCoshTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: cosh_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the cosh interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathCoshTest, cosh_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_coshData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_coshData[i].expected, cosh(g_coshData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: cosh_002
 * @tc.desc: When the parameter of cosh is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathCoshTest, cosh_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0, cosh(0.0));
}

/**
 * @tc.name: coshf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the coshf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathCoshTest, coshf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_coshfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_coshfData[i].expected, coshf(g_coshfData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: coshf_002
 * @tc.desc: When the parameter of coshf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathCoshTest, coshf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(1.0f, coshf(0.0f));
}

/**
 * @tc.name: coshl_001
 * @tc.desc: When the parameter of coshl is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathCoshTest, coshl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0L, coshl(0.0L));
}