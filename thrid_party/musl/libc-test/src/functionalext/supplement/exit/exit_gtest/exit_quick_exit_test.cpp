#include <errno.h>
#include <gtest/gtest.h>
#include <stdlib.h>

using namespace testing::ext;

class ExitQuickExitTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: quick_exit_001
 * @tc.desc: Verify that the parent process can correctly wait for the child process to exit and retrieve its exit
 *           status after the child process calls quick_exit.
 * @tc.type: FUNC
 */
HWTEST_F(ExitQuickExitTest, quick_exit_001, TestSize.Level1)
{
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        sleep(1);
        quick_exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        while ((pid = wait(&status)) == -1) {
            if (errno != EINTR) {
                exit(EXIT_FAILURE);
            }
        }
        if (WIFEXITED(status)) {
            EXPECT_EQ(WEXITSTATUS(status), EXIT_SUCCESS);
        }
    } else {
        exit(EXIT_FAILURE);
    }
}