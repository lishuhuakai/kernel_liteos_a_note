#include <fcntl.h>
#include <gtest/gtest.h>
#include <string.h>
#include <unistd.h>

using namespace testing::ext;

constexpr int BUF_SIZE = 1024;

class StdioGetsTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: gets_001
 * @tc.desc: Verify whether the read operation of this gets is normal.
 * @tc.type: FUNC
 * */
HWTEST_F(StdioGetsTest, gets_001, TestSize.Level1)
{
    pid_t pid;
    pid = fork();
    ASSERT_LE(0, pid);
    if (pid == 0) {
        char str[BUF_SIZE];
        int fd = open("/proc/version", O_RDONLY);
        int orgFd = dup(STDIN_FILENO);
        dup2(fd, STDIN_FILENO);
        gets(str);
        EXPECT_TRUE(str);
        dup2(orgFd, STDIN_FILENO);
        close(fd);
        _exit(0);
    } else {
        waitpid(pid, nullptr, 0);
    }
}