#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/nearbyintf_data.h"
#include "math_test_data/nearbyint_data.h"

using namespace testing::ext;

class MathNearbyintTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: nearbyint_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the nearbyint interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathNearbyintTest, nearbyint_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_nearbyintData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_nearbyintData[i].expected, nearbyint(g_nearbyintData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: nearbyintf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the nearbyintf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathNearbyintTest, nearbyintf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_nearbyintfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_nearbyintfData[i].expected, nearbyintf(g_nearbyintfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}