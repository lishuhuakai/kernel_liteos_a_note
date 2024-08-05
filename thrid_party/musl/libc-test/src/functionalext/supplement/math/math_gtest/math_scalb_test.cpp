#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/scalbf_data.h"
#include "math_test_data/scalb_data.h"

using namespace testing::ext;

class MathScalbTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: scalb_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the scalb interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalbTest, scalb_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_scalbData) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_scalbData[i].expected, scalb(g_scalbData[i].input1, g_scalbData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: scalb_002
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalbTest, scalb_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(32.0, scalb(4.0, 3.0));
}

/**
 * @tc.name: scalbf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the scalbf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalbTest, scalbf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_scalbfData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_scalbfData[i].expected, scalbf(g_scalbfData[i].input1, g_scalbfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: scalbf_002
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathScalbTest, scalbf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(32.0f, scalbf(4.0f, 3.0f));
}