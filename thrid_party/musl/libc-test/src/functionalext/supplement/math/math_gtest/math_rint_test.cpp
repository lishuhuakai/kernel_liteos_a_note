#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/rintf_data.h"
#include "math_test_data/rint_data.h"

using namespace testing::ext;

class MathRintTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: rint_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the rint interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathRintTest, rint_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_rintData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_rintData[i].expected, rint(g_rintData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: rintf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the rintf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathRintTest, rintf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_rintfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_rintfData[i].expected, rintf(g_rintfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}