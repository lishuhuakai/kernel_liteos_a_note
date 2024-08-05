#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/fmaxf_data.h"
#include "math_test_data/fmax_data.h"

using namespace testing::ext;

class MathFmaxTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fmax_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fmax interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmaxTest, fmax_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fmaxData) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_fmaxData[i].expected, fmax(g_fmaxData[i].input1, g_fmaxData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fmax_002
 * @tc.desc: When the parameter of fmax is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmaxTest, fmax_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(17.0, fmax(17.0, 17.0));
    EXPECT_DOUBLE_EQ(15.0, fmax(15.0, nan("")));
    EXPECT_DOUBLE_EQ(14.0, fmax(nan(""), 14.0));
}

/**
 * @tc.name: fmaxf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fmaxf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmaxTest, fmaxf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fmaxfData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_fmaxfData[i].expected, fmaxf(g_fmaxfData[i].input1, g_fmaxfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fmaxf_002
 * @tc.desc: When the parameter of fmaxf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmaxTest, fmaxf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(17.0f, fmaxf(17.0f, 17.0f));
    EXPECT_FLOAT_EQ(15.0f, fmaxf(15.0f, nanf("")));
    EXPECT_FLOAT_EQ(14.0f, fmaxf(nanf(""), 14.0f));
}

/**
 * @tc.name: fmaxl_001
 * @tc.desc: When the parameter of fmaxl is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmaxTest, fmaxl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(17.0L, fmaxl(17.0L, 17.0L));
    EXPECT_DOUBLE_EQ(15.0L, fmaxl(15.0L, nanl("")));
    EXPECT_DOUBLE_EQ(14.0L, fmaxl(nanl(""), 14.0L));
}