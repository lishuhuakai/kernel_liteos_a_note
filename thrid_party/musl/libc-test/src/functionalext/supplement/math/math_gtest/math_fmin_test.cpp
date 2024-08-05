#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/fminf_data.h"
#include "math_test_data/fmin_data.h"

using namespace testing::ext;

class MathFminTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fmin_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fmin interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFminTest, fmin_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fminData) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_fminData[i].expected, fmin(g_fminData[i].input1, g_fminData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fmin_002
 * @tc.desc: When the parameter of fmin is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFminTest, fmin_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(14.0, fmin(15.0, 14.0));
    EXPECT_DOUBLE_EQ(11.0, fmin(11.0, nan("")));
    EXPECT_DOUBLE_EQ(12.0, fmin(nan(""), 12.0));
}

/**
 * @tc.name: fminf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the fminf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFminTest, fminf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_fminfData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_fminfData[i].expected, fminf(g_fminfData[i].input1, g_fminfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: fminf_002
 * @tc.desc: When the parameter of fminf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFminTest, fminf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(14.0f, fminf(15.0f, 14.0f));
    EXPECT_FLOAT_EQ(11.0f, fminf(11.0f, nanf("")));
    EXPECT_FLOAT_EQ(12.0f, fminf(nanf(""), 12.0f));
}

/**
 * @tc.name: fminl_001
 * @tc.desc: When the parameter of fminl is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFminTest, fminl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(14.0L, fminl(15.0L, 14.0L));
    EXPECT_DOUBLE_EQ(11.0L, fminl(11.0L, nanl("")));
    EXPECT_DOUBLE_EQ(12.0L, fminl(nanl(""), 12.0L));
}