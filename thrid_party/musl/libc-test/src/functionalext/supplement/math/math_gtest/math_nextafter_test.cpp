#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/nextafterf_data.h"
#include "math_test_data/nextafter_data.h"

using namespace testing::ext;

class MathNearafterTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: nextafter_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the nextafter interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathNearafterTest, nextafter_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_nextafterData) / sizeof(DataDouble3Expected1); i++) {
        bool testResult = DoubleUlpCmp(g_nextafterData[i].expected, nextafter(g_nextafterData[i].input1,
            g_nextafterData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: nextafter_002
 * @tc.desc: When the input parameters are valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathNearafterTest, nextafter_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0, nextafter(0.0, 0.0));
    EXPECT_DOUBLE_EQ(4.9406564584124654e-324, nextafter(0.0, 1.0));
    EXPECT_DOUBLE_EQ(-4.9406564584124654e-324, nextafter(0.0, -1.0));
}

/**
 * @tc.name: nextafterf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the nextafterf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathNearafterTest, nextafterf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_nextafterfData) / sizeof(DataFloat3Expected1); i++) {
        bool testResult = FloatUlpCmp(g_nextafterfData[i].expected, nextafterf(g_nextafterfData[i].input1,
            g_nextafterfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: nextafterf_002
 * @tc.desc: When the input parameter is of float type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathNearafterTest, nextafterf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(0.0f, nextafterf(0.0f, 0.0f));
    EXPECT_FLOAT_EQ(1.4012985e-45f, nextafterf(0.0f, 1.0f));
    EXPECT_FLOAT_EQ(-1.4012985e-45f, nextafterf(0.0f, -1.0f));
}

/**
 * @tc.name: nextafterl_001
 * @tc.desc: When the input parameter is of long double type and valid, test the return value of this function.
 * @tc.type: FUNC
 */
HWTEST_F(MathNearafterTest, nextafterl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(0.0L, nextafterl(0.0L, 0.0L));
    long double minPositive = ldexpl(1.0L, LDBL_MIN_EXP - LDBL_MANT_DIG);
    EXPECT_DOUBLE_EQ(minPositive, nextafterl(0.0L, 1.0L));
    EXPECT_DOUBLE_EQ(-minPositive, nextafterl(0.0L, -1.0L));
}
