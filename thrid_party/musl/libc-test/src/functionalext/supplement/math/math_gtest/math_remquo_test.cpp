#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/remquof_data.h"
#include "math_test_data/remquo_data.h"

using namespace testing::ext;

class MathRemquoTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: remquo_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the remquo interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemquoTest, remquo_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_remquoData) / sizeof(DataDouble3Int1); i++) {
        int q;
        bool testResult = DoubleUlpCmp(g_remquoData[i].expected1, remquo(g_remquoData[i].input1,
            g_remquoData[i].input2, &q), 1);
        EXPECT_TRUE(testResult);
        EXPECT_EQ(g_remquoData[i].expected2, q);
    }
}

/**
 * @tc.name: remquo_002
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemquoTest, remquo_002, TestSize.Level1)
{
    int quotient;
    double result = remquo(15.0, 6.0, &quotient);
    EXPECT_EQ(2, quotient);
    EXPECT_DOUBLE_EQ(3.0, result);
}

/**
 * @tc.name: remquo_003
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemquoTest, remquo_003, TestSize.Level1)
{
    int quotient;
    EXPECT_TRUE(isnan(remquo(nan(""), 15.0, &quotient)));
    EXPECT_TRUE(isnan(remquo(11.0, nan(""), &quotient)));
}

/**
 * @tc.name: remquo_004
 * @tc.desc: When the input parameter is infinite, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemquoTest, remquo_004, TestSize.Level1)
{
    int quotient;
    EXPECT_TRUE(isnan(remquo(HUGE_VAL, 15.0, &quotient)));
    EXPECT_TRUE(isnan(remquo(-HUGE_VAL, 11.0, &quotient)));
}

/**
 * @tc.name: remquo_005
 * @tc.desc: When the input parameter is valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemquoTest, remquo_005, TestSize.Level1)
{
    int quotient;
    EXPECT_TRUE(isnan(remquo(14.0, 0.0, &quotient)));
}

/**
 * @tc.name: remquof_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the remquof interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemquoTest, remquof_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_remquofData) / sizeof(DataFloat3Int1); i++) {
        int q;
        bool testResult1 = FloatUlpCmp(g_remquofData[i].expected1, remquof(g_remquofData[i].input1,
            g_remquofData[i].input2, &q), 1);
        EXPECT_TRUE(testResult1);
        bool testResult2 = FloatUlpCmp(g_remquofData[i].expected2, q, 1);
        EXPECT_TRUE(testResult2);
    }
}

/**
 * @tc.name: remquof_002
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemquoTest, remquof_002, TestSize.Level1)
{
    int quotient;
    double result = remquof(15.0f, 6.0f, &quotient);
    EXPECT_EQ(2, quotient);
    EXPECT_DOUBLE_EQ(3.0, result);
}

/**
 * @tc.name: remquof_003
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemquoTest, remquof_003, TestSize.Level1)
{
    int quotient;
    EXPECT_TRUE(isnan(remquof(nanf(""), 15.0f, &quotient)));
    EXPECT_TRUE(isnan(remquof(11.0f, nanf(""), &quotient)));
}

/**
 * @tc.name: remquof_004
 * @tc.desc: When the input parameter is infinite, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemquoTest, remquof_004, TestSize.Level1)
{
    int quotient;
    EXPECT_TRUE(isnan(remquof(HUGE_VAL, 15.0f, &quotient)));
    EXPECT_TRUE(isnan(remquof(-HUGE_VAL, 11.0f, &quotient)));
}

/**
 * @tc.name: remquof_005
 * @tc.desc: When the input parameter is valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathRemquoTest, remquof_005, TestSize.Level1)
{
    int quotient;
    EXPECT_TRUE(isnan(remquof(14.0f, 0.0f, &quotient)));
}