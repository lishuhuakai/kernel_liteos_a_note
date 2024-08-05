#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/expf_data.h"
#include "math_test_data/exp_data.h"

using namespace testing::ext;

class MathExpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: exp_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the exp interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathExpTest, exp_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_expData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_expData[i].expected, exp(g_expData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: exp_002
 * @tc.desc: When the parameter of exp is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathExpTest, exp_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0, exp(0.0));
    EXPECT_DOUBLE_EQ(M_E, exp(1.0));
}

/**
 * @tc.name: expf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the expf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathExpTest, expf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_expfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_expfData[i].expected, expf(g_expfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: expf_002
 * @tc.desc: When the parameter of expf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathExpTest, expf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(1.0f, expf(0.0f));
    EXPECT_FLOAT_EQ(static_cast<float>(M_E), expf(1.0f));
}

/**
 * @tc.name: expl_001
 * @tc.desc: When the parameter of expl is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathExpTest, expl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0L, expl(0.0L));
    EXPECT_DOUBLE_EQ(M_E, expl(1.0L));
}