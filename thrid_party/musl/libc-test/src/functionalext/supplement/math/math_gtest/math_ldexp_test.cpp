#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/ldexpf_data.h"
#include "math_test_data/ldexp_data.h"

using namespace testing::ext;

class MathLdexpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: ldexp_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the ldexp interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLdexpTest, ldexp_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_ldexpData) / sizeof(DataDoubleDoubleInt); i++) {
        bool testResult = DoubleUlpCmp(g_ldexpData[i].expected, ldexp(g_ldexpData[i].input1, g_ldexpData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: ldexp_002
 * @tc.desc: When the value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLdexpTest, ldexp_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(24.0, ldexp(3.0, 3));
}

/**
 * @tc.name: ldexpf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the ldexpf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathLdexpTest, ldexpf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_ldexpfData) / sizeof(DataFloatFloatInt); i++) {
        bool testResult = FloatUlpCmp(g_ldexpfData[i].expected, ldexpf(g_ldexpfData[i].input1,
            g_ldexpfData[i].input2), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: ldexpf_002
 * @tc.desc: When the float value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLdexpTest, ldexpf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(24.0f, ldexpf(3.0f, 3));
}

/**
 * @tc.name: ldexpl_001
 * @tc.desc: When the long double value is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathLdexpTest, ldexpl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(24.0L, ldexpl(3.0L, 3));
}