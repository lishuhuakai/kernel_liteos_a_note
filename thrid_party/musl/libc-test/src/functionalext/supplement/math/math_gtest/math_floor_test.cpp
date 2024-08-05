#include <gtest/gtest.h>
#include <math.h>

#include "math_data_test.h"
#include "math_test_data/floorf_data.h"
#include "math_test_data/floor_data.h"

using namespace testing::ext;

class MathFloorTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: floor_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the floor interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFloorTest, floor_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_floorData) / sizeof(DataDoubleDouble); i++) {
        bool testResult = DoubleUlpCmp(g_floorData[i].expected, floor(g_floorData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: floor_002
 * @tc.desc: When the parameter of floor is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFloorTest, floor_002, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(2.0, floor(2.2));
}

/**
 * @tc.name: floorf_001
 * @tc.desc: Obtain test data in sequence and check if it is within the expected error range of the floorf interface.
 * @tc.type: FUNC
 */
HWTEST_F(MathFloorTest, floorf_001, TestSize.Level1)
{
    fesetenv(FE_DFL_ENV);
    for (int i = 0; i < sizeof(g_floorfData) / sizeof(DataFloatFloat); i++) {
        bool testResult = FloatUlpCmp(g_floorfData[i].expected, floorf(g_floorfData[i].input), 1);
        EXPECT_TRUE(testResult);
    }
}

/**
 * @tc.name: floorf_002
 * @tc.desc: When the parameter of floorf is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFloorTest, floorf_002, TestSize.Level1)
{
    EXPECT_FLOAT_EQ(2.0f, floorf(2.2f));
}

/**
 * @tc.name: floorl_001
 * @tc.desc: When the parameter of floorl is valid, test the return value of the function.
 * @tc.type: FUNC
 */
HWTEST_F(MathFloorTest, floorl_001, TestSize.Level1)
{
    EXPECT_DOUBLE_EQ(2.0L, floorl(2.2L));
}