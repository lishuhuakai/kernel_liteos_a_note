#include <gtest/gtest.h>
#include <sys/timerfd.h>

using namespace testing::ext;

class LinuxTimerfdsettimeTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: timerfd_settime_001
 * @tc.desc: This test verifies that for timerfd_create, timerfd_settime and timerfd_gettime functions create a
 *           timer file descriptor, set the timer time value, and obtain the current time value to check if their
 *           functions are normal, if any of these operations fail, the corresponding error message will be output.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxTimerfdsettimeTest, timerfd_settime_001, TestSize.Level1)
{
    struct itimerspec its = { { 0, 0 }, { 2, 0 } };
    struct itimerspec val;
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    EXPECT_GE(fd, 0);
    int setResult = timerfd_settime(fd, 0, &its, nullptr);
    EXPECT_EQ(setResult, 0);
    int getResult = timerfd_gettime(fd, &val);
    EXPECT_EQ(getResult, 0);
    close(fd);
}