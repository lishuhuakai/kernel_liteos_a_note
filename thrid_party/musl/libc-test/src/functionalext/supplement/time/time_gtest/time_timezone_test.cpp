#include <gtest/gtest.h>
#include <sys/timeb.h>
#include <time.h>
using namespace testing::ext;

class TimeTimezoneTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: timezone_001
 * @tc.desc: Test the functionality of the clock_gettime function and the timezone field of the timeb structure.
 * @tc.type: FUNC
 **/
HWTEST_F(TimeTimezoneTest, timezone_001, TestSize.Level1)
{
    struct timeb tp;
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    tp.timezone = tp.dstflag;
    EXPECT_EQ(0, tp.timezone);
}