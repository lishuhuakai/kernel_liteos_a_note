#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/pow_data.h"
#include "math_test_data/powf_data.h"

using namespace testing::ext;

class MathPowTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: pow_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the pow interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathPowTest, pow_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_powData) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_powData[i].expected, pow(g_powData[i].input1, g_powData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: pow_002
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathPowTest, pow_002, TestSize.Level1)
{
    EXPECT_TRUE(isnan(pow(nan(""), 5.0)));
    EXPECT_TRUE(isnan(pow(4.0, nan(""))));
}

/**
 * @tc.name: pow_003
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathPowTest, pow_003, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0, (pow(1.0, nan(""))));
    EXPECT_DOUBLE_EQ(9.0, pow(3.0, 2.0));
}

/**
 * @tc.name: powf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the powf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathPowTest, powf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_powfData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_powfData[i].expected, powf(g_powfData[i].input1, g_powfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: powf_002
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathPowTest, powf_002, TestSize.Level1)
{
    EXPECT_TRUE(isnan(powf(nanf(""), 3.0f)));
    EXPECT_TRUE(isnan(powf(2.0f, nanf(""))));
}

/**
 * @tc.name: powf_003
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathPowTest, powf_003, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(1.0f, (powf(1.0f, nanf(""))));
    EXPECT_DOUBLE_EQ(9.0f, powf(3.0f, 2.0f));
}