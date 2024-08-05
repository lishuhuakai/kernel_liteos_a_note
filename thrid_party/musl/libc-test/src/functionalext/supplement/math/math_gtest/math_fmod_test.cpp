#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/fmodf_data.h"
#include "math_test_data/fmod_data.h"

using namespace testing::ext;

class MathFmodTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fmod_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fmod interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmodTest, fmod_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fmodData) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_fmodData[i].expected, fmod(g_fmodData[i].input1, g_fmodData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fmod_002
 * @tc.desc: When the parameter of fmod is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmodTest, fmod_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(5.0, fmod(15.0, 10.0));
}

/**
 * @tc.name: fmod_003
 * @tc.desc: When the parameter of fmod is infinite, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmodTest, fmod_003, TestSize.Level1)
{
    EXPECT_TRUE(isnan(fmod(HUGE_VAL, 15.0f)));
    EXPECT_TRUE(isnan(fmod(-HUGE_VAL, 15.0f)));
}

/**
 * @tc.name: fmod_004
 * @tc.desc: When one of the numbers is NaN, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmodTest, fmod_004, TestSize.Level1)
{
    EXPECT_TRUE(isnan(fmod(nan(""), 14.0)));
    EXPECT_TRUE(isnan(fmod(14.0, nan(""))));
}

/**
 * @tc.name: fmod_005
 * @tc.desc: When the second operand is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmodTest, fmod_005, TestSize.Level1)
{
    EXPECT_TRUE(isnan(fmod(4.0, 0.0)));
}

/**
 * @tc.name: fmodf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fmodf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmodTest, fmodf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fmodfData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_fmodfData[i].expected, fmodf(g_fmodfData[i].input1, g_fmodfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fmodf_002
 * @tc.desc: When the parameter of fmodf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmodTest, fmodf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(5.0f, fmodf(15.0f, 10.0f));
}

/**
 * @tc.name: fmodf_003
 * @tc.desc: When the parameter of fmod is infinite, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmodTest, fmodf_003, TestSize.Level1)
{
    EXPECT_TRUE(isnan(fmodf(HUGE_VAL, 15.0f)));
    EXPECT_TRUE(isnan(fmodf(-HUGE_VAL, 15.0f)));
}

/**
 * @tc.name: fmodf_004
 * @tc.desc: When one of the numbers is NaN, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmodTest, fmodf_004, TestSize.Level1)
{
    EXPECT_TRUE(isnan(fmodf(nanf(""), 14.0f)));
    EXPECT_TRUE(isnan(fmodf(14.0f, nan(""))));
}

/**
 * @tc.name: fmodf_005
 * @tc.desc: When the second operand is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmodTest, fmodf_005, TestSize.Level1)
{
    EXPECT_TRUE(isnan(fmodf(4.0f, 0.0f)));
}