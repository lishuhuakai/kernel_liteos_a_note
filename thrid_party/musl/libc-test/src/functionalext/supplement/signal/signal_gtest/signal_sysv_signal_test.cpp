#include <gtest/gtest.h>
#include <signal.h>
using namespace testing::ext;

class SignalSysvSignalTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

static void SignalHandler(int signum) {}
/**
 * @tc.name: __sysv_signal_001
 * @tc.desc: ensures that the __sysv_signal function is capable of setting and restoring signal handlers correctly
 *           for the SIGINT signal.
 * @tc.type: FUNC
 **/
HWTEST_F(SignalSysvSignalTest, __sysv_signal_001, TestSize.Level1)
{
    sighandler_t oldHandler = signal(SIGINT, SignalHandler);
    EXPECT_NE(oldHandler, SIG_ERR);

    sighandler_t restoredHandler = signal(SIGINT, oldHandler);
    EXPECT_EQ(restoredHandler, SignalHandler);
}
