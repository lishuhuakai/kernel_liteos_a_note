#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

constexpr int SLEEP_TIME = 1000000;

class UnistdUsleepTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: usleep_001
 * @tc.desc: Test that the function is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdUsleepTest, usleep_001, TestSize.Level1)
{
    struct timespec oldTime;
    clock_gettime(CLOCK_MONOTONIC, &oldTime);
    EXPECT_EQ(0, usleep(SLEEP_TIME));
    struct timespec newTime;
    clock_gettime(CLOCK_MONOTONIC, &newTime);
    ASSERT_GE(newTime.tv_sec - oldTime.tv_sec, 1);
}