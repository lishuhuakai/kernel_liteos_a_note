#include <chrono>
#include <errno.h>
#include <gtest/gtest.h>
#include <signal.h>
using namespace testing::ext;

class SignalSigemptySetTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

static void SignalHandler(int signum) {}

/**
 * @tc.name: sigtimedwait_001
 * @tc.desc: Verify whether the sigtimedwait() function waits for a signal from the set set within the specified
 *           ts and returns -1 if no signal is received.
 * @tc.type: FUNC
 **/
HWTEST_F(SignalSigemptySetTest, sigtimedwait_001, TestSize.Level1)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    struct sigaction sa;
    sa.sa_handler = SignalHandler;
    sa.sa_mask = set;
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, nullptr);

    siginfo_t si;
    struct timespec ts = { .tv_sec = 1, .tv_nsec = 0 };
    EXPECT_EQ(-1, sigtimedwait(&set, &si, &ts));
}