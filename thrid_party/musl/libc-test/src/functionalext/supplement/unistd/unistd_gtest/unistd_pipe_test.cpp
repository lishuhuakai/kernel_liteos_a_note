#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

constexpr int BUF_SIZE = 256;

class UnistdPipeTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: pipe_001
 * @tc.desc: checks whether the pipe function creates a pipe successfully and whether data can be written to and read
 *           from the pipe correctly in a child process and a parent process.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdPipeTest, pipe_001, TestSize.Level1)
{
    int fd[2];
    char buffer[BUF_SIZE];
    EXPECT_EQ(0, pipe(fd));
    pid_t pid = fork();
    ASSERT_LE(0, pid);
    if (pid == 0) {
        const char* message = "test pipe";
        EXPECT_EQ(0, close(fd[0]));
        write(fd[1], message, strlen(message));
        EXPECT_EQ(0, close(fd[1]));
        _exit(0);
    } else {
        EXPECT_EQ(0, close(fd[1]));
        read(fd[0], buffer, sizeof(buffer));
        EXPECT_STREQ("test pipe", buffer);
        EXPECT_EQ(0, close(fd[0]));
    }
}