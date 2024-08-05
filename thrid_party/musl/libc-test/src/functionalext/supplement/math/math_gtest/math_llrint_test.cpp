#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/llrintf_data.h"
#include "math_test_data/llrint_data.h"

using namespace testing::ext;

class MathLlrintTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: llrint_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the llrint interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLlrintTest, llrint_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_llrintData) / sizeof(DataLlongDouble); i++) {
        bool testResult = DoubleUlpCmp(g_llrintData[i].expected, llrint(g_llrintData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: llrintf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the llrintf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLlrintTest, llrintf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_llrintfData) / sizeof(DataLlongFloat); i++) {
        bool testResult = FloatUlpCmp(g_llrintfData[i].expected, llrintf(g_llrintfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}