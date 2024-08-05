#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/acoshf_data.h"
#include "math_test_data/acosh_data.h"


using namespace testing::ext;

class MathAcoshTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: acosh_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the acosh interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAcoshTest, acosh_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_acoshData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_acoshData[i].expected, acosh(g_acoshData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: acosh_002
* @tc.desc: When the parameter of acosh is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAcoshTest, acosh_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, acosh(1.0));
}

/**
* @tc.name: acoshf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the acoshf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAcoshTest, acoshf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_acoshfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_acoshfData[i].expected, acoshf(g_acoshfData[i].input), 2);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: acoshf_002
* @tc.desc: When the parameter of acosh is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAcoshTest, acoshf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, acoshf(1.0f));
}

/**
* @tc.name: acoshl_001
* @tc.desc: When the parameter of acosh is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAcoshTest, acoshl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, acoshl(1.0L));
}