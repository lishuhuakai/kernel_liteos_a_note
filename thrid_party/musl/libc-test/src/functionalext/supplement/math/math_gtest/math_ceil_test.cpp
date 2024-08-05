#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/ceilf_data.h"
#include "math_test_data/ceil_data.h"

using namespace testing::ext;

class MathCeilTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: ceil_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the ceil interface.
* @tc.type: FUNC
*/
HWTEST_F(MathCeilTest, ceil_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_ceilData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_ceilData[i].expected, ceil(g_ceilData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: ceil_002
* @tc.desc: When the parameter of ceil is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathCeilTest, ceil_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0, ceil(0.8));
}

/**
* @tc.name: ceilf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the ceilf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathCeilTest, ceilf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_ceilfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_ceilfData[i].expected, ceilf(g_ceilfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: ceilf_002
* @tc.desc: When the parameter of ceilf is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathCeilTest, ceilf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(1.0f, ceilf(0.8f));
}

/**
* @tc.name: ceill_001
* @tc.desc: When the parameter of ceill is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathCeilTest, ceill_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0L, ceill(0.8L));
}