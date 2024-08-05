#include <gtest/gtest.h>
#include <signal.h>
#include <unistd.h>
using namespace testing::ext;

class ExitAbortTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: abort_001
 * @tc.desc: Verify that calling the abort function in a child process leads to expected termination with the
 *           SIGABRT signal.
 * @tc.type: FUNC
 **/
HWTEST_F(ExitAbortTest, abort_001, TestSize.Level1)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error forking process");
    } else if (pid == 0) {
        abort();
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sigNum = WTERMSIG(status);
            EXPECT_EQ(SIGABRT, sigNum);
        }
    }
}
