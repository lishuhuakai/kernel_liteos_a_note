#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/modff_data.h"
#include "math_test_data/modf_data.h"

using namespace testing::ext;

class MathModfTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: modf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the modf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathModfTest, modf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_modfData) / sizeof(DataDouble3Expected2); i++) {
        double di;
        bool testResult1 = DoubleUlpCmp(g_modfData[i].expected1, modf(g_modfData[i].input, &di), 1);
        EXPECT_TRUE(testResult1);
        bool testResult2 = DoubleUlpCmp(g_modfData[i].expected2, di, 1);
        EXPECT_TRUE(testResult2);
    }
}

/**
 * @tc.name: modf_002
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathModfTest, modf_002, TestSize.Level1)
{
    double integerPart;
    double decimalPart = modf(8.125, &integerPart);
    EXPECT_DOUBLE_EQ(8.0, integerPart);
    EXPECT_DOUBLE_EQ(0.125, decimalPart);
}

/**
 * @tc.name: modff_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the modff interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathModfTest, modff_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_modffData) / sizeof(DataFloat3Expected2); i++) {
        float di;
        bool testResult1 = FloatUlpCmp(g_modffData[i].expected1, modff(g_modffData[i].input, &di), 1);
        EXPECT_TRUE(testResult1);
        bool testResult2 = FloatUlpCmp(g_modffData[i].expected2, di, 1);
        EXPECT_TRUE(testResult2);
    }
}

/**
 * @tc.name: modff_002
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathModfTest, modff_002, TestSize.Level1)
{
    float integralPart;
    float fractionalPart = modff(8.125f, &integralPart);
    EXPECT_FLOAT_EQ(8.0f, integralPart);
    EXPECT_FLOAT_EQ(0.125f, fractionalPart);
}

/**
 * @tc.name: modfl_001
 * @tc.desc: When the input parameter is of long double type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathModfTest, modfl_001, TestSize.Level1)
{
    long double integralPart;
    long double fractionalPart = modfl(8.125L, &integralPart);
    EXPECT_FLOAT_EQ(8.0L, integralPart);
    EXPECT_FLOAT_EQ(0.125L, fractionalPart);
}