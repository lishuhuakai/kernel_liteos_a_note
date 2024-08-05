#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdDupTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: dup_001
 * @tc.desc: Can the dup function successfully copy standard file descriptors.
 * @tc.type: FUNC
 */
HWTEST_F(UnistdDupTest, dup_001, TestSize.Level1)
{
    int fd = STDIN_FILENO;
    int dupFd = dup(fd);
    EXPECT_NE(fd, dupFd);
}