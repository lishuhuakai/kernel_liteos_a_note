#include <gtest/gtest.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace testing::ext;

constexpr int BUF_SIZE = 128;

class UnistdExitTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: _Exit_001
 * @tc.desc: Ensure that the basic operations of the file can function properly.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdExitTest, _Exit_001, TestSize.Level1)
{
    int pipefd[2];
    ASSERT_NE(-1, pipe(pipefd));
    pid_t pid = fork();
    if (pid == 0) {
        close(pipefd[0]);
        write(pipefd[1], "before exit", sizeof("before exit"));
        close(pipefd[1]);
        _Exit(0);
        write(pipefd[1], "after exit", sizeof("after exit"));
    } else {
        close(pipefd[1]);
        char buffer[BUF_SIZE];
        read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);
        EXPECT_STREQ("before exit", buffer);
    }
}