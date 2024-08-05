#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/copysignf_data.h"
#include "math_test_data/copysign_data.h"

using namespace testing::ext;

class MathCopysignTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
* @tc.name: copysign_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the copysign interface.
* @tc.type: FUNC
*/
HWTEST_F(MathCopysignTest, copysign_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_copysignData) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_copysignData[i].expected, copysign(g_copysignData[i].input1,
        g_copysignData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: copysign_002
* @tc.desc: When the parameter of copysign is valid, test the return value of the
            function.
* @tc.type: FUNC
*/
HWTEST_F(MathCopysignTest, copysign_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0, copysign(1.0, 2.0));
    EXPECT_DOUBLE_EQ(-1.0, copysign(1.0, -2.0));
    EXPECT_DOUBLE_EQ(3.0, copysign(3.0, 2.0));
    EXPECT_DOUBLE_EQ(-3.0, copysign(3.0, -2.0));
}

/**
* @tc.name: copysignf_001
* @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the copysignf interface.
* @tc.type: FUNC
*/
HWTEST_F(MathCopysignTest, copysignf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_copysignfData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_copysignfData[i].expected, copysignf(g_copysignfData[i].input1,
            g_copysignfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
* @tc.name: copysignf_002
* @tc.desc: When the parameter of copysignf is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathCopysignTest, copysignf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(1.0f, copysignf(1.0f, 2.0f));
    EXPECT_FLOAT_EQ(-1.0f, copysignf(1.0f, -2.0f));
    EXPECT_FLOAT_EQ(3.0f, copysignf(3.0f, 2.0f));
    EXPECT_FLOAT_EQ(-3.0f, copysignf(3.0f, -2.0f));
}

/**
* @tc.name: copysignl_001
* @tc.desc: When the parameter of copysignl is valid, test the return value of the function.
* @tc.type: FUNC
*/
HWTEST_F(MathCopysignTest, copysignl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0L, copysignl(1.0L, 2.0L));
    EXPECT_DOUBLE_EQ(-1.0L, copysignl(1.0L, -2.0L));
    EXPECT_DOUBLE_EQ(3.0L, copysignl(3.0L, 2.0L));
    EXPECT_DOUBLE_EQ(-3.0L, copysignl(3.0L, -2.0L));
}