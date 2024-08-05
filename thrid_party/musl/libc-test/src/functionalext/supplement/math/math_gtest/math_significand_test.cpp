#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/significandf_data.h"
#include "math_test_data/significand_data.h"

using namespace testing::ext;

class MathSignificandTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: significand_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range
 *           of the significand interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathSignificandTest, significand_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_significandData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_significandData[i].expected, significand(g_significandData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: significand_002
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSignificandTest, significand_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, significand(0.0));
    EXPECT_DOUBLE_EQ(1.2, significand(1.2));
    EXPECT_DOUBLE_EQ(1.53125, significand(12.25));
}

/**
 * @tc.name: significandf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range
 *           of the significandf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathSignificandTest, significandf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_significandfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_significandfData[i].expected, significandf(g_significandfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: significandf_002
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathSignificandTest, significandf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, significandf(0.0f));
    EXPECT_FLOAT_EQ(1.2f, significandf(1.2f));
    EXPECT_FLOAT_EQ(1.53125f, significandf(12.25f));
}