#include <fcntl.h>
#include <gtest/gtest.h>
#include <string.h>
#include <unistd.h>

using namespace testing::ext;

class StdioGetwcharTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: getwchar_001
 * @tc.desc: Verify whether the read operation of this getwchar is normal.
 * @tc.type: FUNC
 * */
HWTEST_F(StdioGetwcharTest, getwchar_001, TestSize.Level1)
{
    pid_t pid;
    pid = fork();
    ASSERT_LE(0, pid);
    if (pid == 0) {
        wint_t str;
        int fd = open("/proc/version", O_RDONLY);
        int orgFd = STDIN_FILENO;
        dup2(fd, STDIN_FILENO);
        str = getwchar();
        EXPECT_TRUE(str);
        dup2(orgFd, STDIN_FILENO);
        close(fd);
        _exit(0);
    } else {
        waitpid(pid, nullptr, 0);
    }
}