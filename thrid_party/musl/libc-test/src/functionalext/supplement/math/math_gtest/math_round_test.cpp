#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/roundf_data.h"
#include "math_test_data/round_data.h"

using namespace testing::ext;

class MathRoundTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: round_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the round interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathRoundTest, round_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_roundData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_roundData[i].expected, round(g_roundData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: roundf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the roundf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathRoundTest, roundf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_roundfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_roundfData[i].expected, roundf(g_roundfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}