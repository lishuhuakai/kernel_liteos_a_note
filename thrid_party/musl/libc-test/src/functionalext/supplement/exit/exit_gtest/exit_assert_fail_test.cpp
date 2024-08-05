#include <assert.h>
#include <gtest/gtest.h>
#include <signal.h>
#include <stdio.h>
using namespace testing::ext;

class ExitAssertfailTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __assert_fail_001
 * @tc.desc: Verify that the __assert_fail function properly triggers an assertion failure within a child process,
 *           leading to termination with the SIGABRT signal.
 * @tc.type: FUNC
 **/
HWTEST_F(ExitAssertfailTest, __assert_fail_001, TestSize.Level1)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error forking process");
    } else if (pid == 0) {
        __assert_fail(nullptr, nullptr, 0, nullptr);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sigNum = WTERMSIG(status);
            EXPECT_EQ(SIGABRT, sigNum);
        }
    }
}
