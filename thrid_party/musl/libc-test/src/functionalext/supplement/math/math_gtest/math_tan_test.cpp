#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/tanf_data.h"
#include "math_test_data/tan_data.h"

using namespace testing::ext;

class MathTanTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: tan_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range
 *           of the tan interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathTanTest, tan_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_tanData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_tanData[i].expected, tan(g_tanData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: tan_002
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathTanTest, tan_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, tan(0.0));
}

/**
 * @tc.name: tanf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range
 *           of the tanf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathTanTest, tanf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_tanfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_tanfData[i].expected, tanf(g_tanfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: tanf_002
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathTanTest, tanf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, tanf(0.0f));
}

/**
 * @tc.name: tanl_001
 * @tc.desc: When the long double value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathTanTest, tanl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, tanl(0.0L));
}