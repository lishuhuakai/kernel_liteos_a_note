#include <errno.h>
#include <gtest/gtest.h>
#include <signal.h>
using namespace testing::ext;

class SignalSigaddsetTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

static void SignalHandler(int signum) {}

/**
 * @tc.name: sigaddset_001
 * @tc.desc: Ensure that the "sigaddset" function correctly adds signals to a signal set, and that the "sigaction"
 *           function can be used to set up signal handlers for the specified signals.
 * @tc.type: FUNC
 **/
HWTEST_F(SignalSigaddsetTest, sigaddset_001, TestSize.Level1)
{
    sigset_t set;
    sigemptyset(&set);

    EXPECT_EQ(0, sigaddset(&set, SIGINT));
    EXPECT_EQ(0, sigaddset(&set, SIGTERM));
    struct sigaction sa;
    sa.sa_handler = SignalHandler;
    sa.sa_mask = set;
    sa.sa_flags = 0;
    EXPECT_EQ(sigaction(SIGINT, &sa, nullptr), 0);
    EXPECT_EQ(sigaction(SIGTERM, &sa, nullptr), 0);
}

/**
 * @tc.name: sigaddset_002
 * @tc.desc: Verify that sigaddset correctly adds a signal to a signal set and returns the expected results
 *           when given invalid input.
 * @tc.type: FUNC
 **/
HWTEST_F(SignalSigaddsetTest, sigaddset_002, TestSize.Level1)
{
    sigset_t set;
    sigemptyset(&set);

    errno = 0;
    EXPECT_EQ(-1, sigaddset(&set, 0));
    EXPECT_EQ(EINVAL, errno);

    errno = 0;
    EXPECT_EQ(-1, sigaddset(&set, NSIG + 1));
    EXPECT_EQ(EINVAL, errno);

    errno = 0;
    EXPECT_EQ(0, sigaddset(&set, SIGUSR1));
    EXPECT_EQ(0, errno);
}