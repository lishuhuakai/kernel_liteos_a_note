#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/sincosf_data.h"
#include "math_test_data/sincos_data.h"

using namespace testing::ext;

class MathSincosTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: sincos_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the sincos interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathSincosTest, sincos_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_sincosData) / sizeof(DataDouble3Expected2); i++) {
        double s, c;
        sincos(g_sincosData[i].input, &s, &c);
        bool testResult1 = DoubleUlpCmp(g_sincosData[i].expected1, s, 1);
        EXPECT_TRUE(testResult1);
        bool testResult2 = DoubleUlpCmp(g_sincosData[i].expected2, c, 1);
        EXPECT_TRUE(testResult2);
    }
}

/**
 * @tc.name: sincos_002
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSincosTest, sincos_002, TestSize.Level1)
{
    double sineResult, cosineResult;
    sincos(0.0, &sineResult, &cosineResult);
    EXPECT_DOUBLE_EQ(0.0, sineResult);
    EXPECT_DOUBLE_EQ(1.0, cosineResult);
}

/**
 * @tc.name: sincosf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the sincos interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathSincosTest, sincosf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_sincosfData) / sizeof(DataFloat3Expected2); i++) {
        float s, c;
        sincosf(g_sincosfData[i].input, &s, &c);
        bool testResult1 = FloatUlpCmp(g_sincosfData[i].expected1, s, 1);
        EXPECT_TRUE(testResult1);
        bool testResult2 = FloatUlpCmp(g_sincosfData[i].expected2, c, 1);
        EXPECT_TRUE(testResult2);
    }
}

/**
 * @tc.name: sincosf_002
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSincosTest, sincosf_002, TestSize.Level1)
{
    float sineResult, cosineResult;
    sincosf(0.0f, &sineResult, &cosineResult);
    EXPECT_FLOAT_EQ(0.0f, sineResult);
    EXPECT_FLOAT_EQ(1.0f, cosineResult);
}

/**
 * @tc.name: sincosl_001
 * @tc.desc: When the input parameter is of long double type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSincosTest, sincosl_001, TestSize.Level1)
{
    long double sineResult, cosineResult;
    sincosl(0.0L, &sineResult, &cosineResult);
    EXPECT_DOUBLE_EQ(0.0L, sineResult);
    EXPECT_DOUBLE_EQ(1.0L, cosineResult);
}