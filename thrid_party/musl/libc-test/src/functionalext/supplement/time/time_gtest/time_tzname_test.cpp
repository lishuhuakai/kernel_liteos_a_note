#include <gtest/gtest.h>
#include <time.h>
using namespace testing::ext;

class TimeTimezoneTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: tzname_001
 * @tc.desc: Test the tzname array, which should contain the names of the local time zone and the
 *           alternate time zone (if available).
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTimezoneTest, tzname_001, TestSize.Level1)
{
    time_t currTime;
    struct tm* timeInfo;

    time(&currTime);
    timeInfo = localtime(&currTime);

    EXPECT_TRUE(tzname[0]);
    EXPECT_TRUE(tzname[1]);
}
