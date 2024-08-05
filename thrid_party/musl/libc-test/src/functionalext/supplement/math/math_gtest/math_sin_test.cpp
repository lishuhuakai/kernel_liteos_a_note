#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/sinf_data.h"
#include "math_test_data/sin_data.h"

using namespace testing::ext;

class MathSinTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: sin_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the sin interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathSinTest, sin_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_sinData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_sinData[i].expected, sin(g_sinData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: sin_002
 * @tc.desc: When the value is 0.0, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSinTest, sin_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, sin(0.0));
}

/**
 * @tc.name: sinf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the sinf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathSinTest, sinf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_sinfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_sinfData[i].expected, sinf(g_sinfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: sinf_002
 * @tc.desc: When the float value is 0.0f, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSinTest, sinf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, sinf(0.0f));
}

/**
 * @tc.name: sinl_001
 * @tc.desc: When the long double value is 0.0L, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSinTest, sinl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, sinl(0.0L));
}