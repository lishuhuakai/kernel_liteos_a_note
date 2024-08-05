#include <gtest/gtest.h>
#include <sigchain.h>
using namespace testing::ext;

class SigchainAddSpecialSignalHandlerTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};
constexpr int NUM = 2;
static int g_count = 0;

static bool SigchainSigint1(int signo, siginfo_t* siginfo, void* contextRaw)
{
    g_count++;
    EXPECT_EQ(signo, SIGINT);
    return false;
}
static bool SigchainSigint2(int signo, siginfo_t* siginfo, void* contextRaw)
{
    g_count++;
    EXPECT_EQ(signo, SIGINT);
    return true;
}

/**
 * @tc.name: add_special_signal_handler_001
 * @tc.desc: Verify that the add_special_signal_handler function successfully registers and invokes the specified
 *           special signal handlers when a SIGINT signal is raised.
 * @tc.type: FUNC
 **/
HWTEST_F(SigchainAddSpecialSignalHandlerTest, add_special_signal_handler_001, TestSize.Level1)
{
    struct signal_chain_action sigint1 = {
        .sca_sigaction = SigchainSigint1,
        .sca_mask = {},
        .sca_flags = 0,
    };
    add_special_signal_handler(SIGINT, &sigint1);

    struct signal_chain_action sigint2 = {
        .sca_sigaction = SigchainSigint2,
        .sca_mask = {},
        .sca_flags = SIGCHAIN_ALLOW_NORETURN,
    };
    add_special_signal_handler(SIGINT, &sigint2);

    raise(SIGINT);
    EXPECT_EQ(g_count, NUM);
}