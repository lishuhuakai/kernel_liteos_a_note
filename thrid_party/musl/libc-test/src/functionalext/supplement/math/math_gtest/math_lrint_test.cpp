#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/lrintf_data.h"
#include "math_test_data/lrint_data.h"

using namespace testing::ext;

class MathLrintTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: lrint_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the lrint interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLrintTest, lrint_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_lrintData) / sizeof(DataLongDouble); i++) {
        bool testResult = DoubleUlpCmp(g_lrintData[i].expected, lrint(g_lrintData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}
/**
 * @tc.name: lrintf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the lrintf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLrintTest, lrintf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_lrintfData) / sizeof(DataLongFloat); i++) {
        bool testResult = FloatUlpCmp(g_lrintfData[i].expected, lrintf(g_lrintfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}