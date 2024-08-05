#include <gtest/gtest.h>
#include <time.h>
using namespace testing::ext;

class TimeDaylightTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: daylight_001
 * @tc.desc: Test daylight exists
 * @tc.type: FUNC
 **/
HWTEST_F(TimeDaylightTest, daylight_001, TestSize.Level1)
{
    EXPECT_GE(0, daylight);
}