#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/truncf_data.h"
#include "math_test_data/trunc_data.h"

using namespace testing::ext;

class MathTruncTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: trunc_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the trunc interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathTruncTest, trunc_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_truncData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_truncData[i].expected, trunc(g_truncData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: truncf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the truncf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathTruncTest, truncf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_truncfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_truncfData[i].expected, truncf(g_truncfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}