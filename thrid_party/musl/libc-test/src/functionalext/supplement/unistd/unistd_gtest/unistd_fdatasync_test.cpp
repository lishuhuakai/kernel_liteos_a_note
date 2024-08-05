#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdFdatasyncTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fdatasync_001
 * @tc.desc: Verify whether the fdatasync function behaves as expected under different file descriptor states.
 * @tc.type: FUNC
 */
HWTEST_F(UnistdFdatasyncTest, fdatasync_001, TestSize.Level1)
{
    int fd;

    EXPECT_NE(-1, fd = open("/data/local/tmp", O_RDONLY));
    EXPECT_EQ(0, fdatasync(fd));
    close(fd);

    EXPECT_NE(0, fd = open("/data/local/tmp", O_RDWR));
    EXPECT_EQ(-1, fdatasync(fd));
    close(fd);
}