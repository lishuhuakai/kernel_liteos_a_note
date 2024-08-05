#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/fmaf_data.h"
#include "math_test_data/fma_data.h"

using namespace testing::ext;

class MathFmaTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fma_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fma interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmaTest, fma_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fmaData) / sizeof(DataDouble4); i++) {
        bool testResult = DoubleUlpCmp(g_fmaData[i].expected, fma(g_fmaData[i].input1, g_fmaData[i].input2,
            g_fmaData[i].input3), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fma_002
 * @tc.desc: When the parameter of fma is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmaTest, fma_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(17.0, fma(3.0, 4.0, 5.0));
}

/**
 * @tc.name: fmaf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fmaf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmaTest, fmaf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fmafData) / sizeof(DataFloat4); i++) {
        bool testResult = FloatUlpCmp(g_fmafData[i].expected, fmaf(g_fmafData[i].input1, g_fmafData[i].input2,
            g_fmafData[i].input3), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fmaf_002
 * @tc.desc: When the parameter of fmaf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmaTest, fmaf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(17.0f, fmaf(3.0f, 4.0f, 5.0f));
}

/**
 * @tc.name: fmal_001
 * @tc.desc: When the parameter of fmal is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFmaTest, fmal_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(17.0L, fmal(3.0L, 4.0L, 5.0L));
}