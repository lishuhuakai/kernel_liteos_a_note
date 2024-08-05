#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/cbrtf_data.h"
#include "math_test_data/cbrt_data.h"

using namespace testing::ext;

class MathCbrtTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: cbrt_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the cbrt interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathCbrtTest, cbrt_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_cbrtData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_cbrtData[i].expected, cbrt(g_cbrtData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: cbrt_002
 * @tc.desc: When the parameter of cbrt is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathCbrtTest, cbrt_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(2.0, cbrt(8.0));
}


/**
 * @tc.name: cbrtf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the cbrtf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathCbrtTest, cbrtf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_cbrtfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_cbrtfData[i].expected, cbrtf(g_cbrtfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: cbrtf_002
 * @tc.desc: When the parameter of cbrtf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathCbrtTest, cbrtf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(2.0f, cbrtf(8.0f));
}

/**
 * @tc.name: cbrtl_001
 * @tc.desc: When the parameter of cbrtl is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathCbrtTest, cbrtl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(2.0L, cbrtl(8.0L));
}