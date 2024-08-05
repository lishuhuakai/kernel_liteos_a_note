#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/fdimf_data.h"
#include "math_test_data/fdim_data.h"

using namespace testing::ext;

class MathFdimTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fdim_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fdim interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFdimTest, fdim_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fdimData) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_fdimData[i].expected, fdim(g_fdimData[i].input1, g_fdimData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fdim_002
 * @tc.desc: When the parameter of fdim is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFdimTest, fdim_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(2.0, fdim(3.0, 1.0));
    EXPECT_DOUBLE_EQ(2.0, fdim(5.0, 3.0));
    EXPECT_DOUBLE_EQ(0.0, fdim(1.0, 2.0));
}

/**
 * @tc.name: fdimf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fdimf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFdimTest, fdimf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fdimfData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_fdimfData[i].expected, fdimf(g_fdimfData[i].input1, g_fdimfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fdimf_002
 * @tc.desc: When the parameter of fdimf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFdimTest, fdimf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(2.0f, fdimf(3.0f, 1.0f));
    EXPECT_FLOAT_EQ(2.0f, fdimf(5.0f, 3.0f));
    EXPECT_FLOAT_EQ(0.0f, fdimf(1.0f, 2.0f));
}

/**
 * @tc.name: fdiml_001
 * @tc.desc: When the parameter of fdiml is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFdimTest, fdiml_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(2.0L, fdiml(3.0L, 1.0L));
    EXPECT_DOUBLE_EQ(2.0L, fdiml(5.0L, 3.0L));
    EXPECT_DOUBLE_EQ(0.0L, fdiml(1.0L, 2.0L));
}