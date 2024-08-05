#include <gtest/gtest.h>
#include <sys/timerfd.h>

using namespace testing::ext;

class LinuxTimerfdcreateTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: timerfd_create_001
 * @tc.desc: This test verifies timerfd the create function is using CLOCK During MONOTONIC, the timer file descriptor
 *           can be successfully created and correctly closed. If the creation fails, the corresponding error message
 *           will be output.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxTimerfdcreateTest, timerfd_create_001, TestSize.Level1)
{
    int timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
    EXPECT_GE(timerfd, 0);
    close(timerfd);
}