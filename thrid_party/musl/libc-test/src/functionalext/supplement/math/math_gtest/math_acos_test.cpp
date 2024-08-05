#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/acosf_data.h"
#include "math_test_data/acos_data.h"

using namespace testing::ext;

class MathAcosTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: acos_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the acos interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_acosData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_acosData[i].expected, acos(g_acosData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: acos_002
* @tc.desc: When the parameter of acos is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acos_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, acos(1.0));
}

/**
* @tc.name: acosf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the acosf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_acosfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_acosfData[i].expected, acosf(g_acosfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: acosf_002
* @tc.desc: When the parameter of acosf is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, acosf(1.0f));
}

/**
* @tc.name: acosl_001
* @tc.desc: When the parameter of acosl is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathAcosTest, acosl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, acosl(1.0L));
}