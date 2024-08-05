#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdFsyncTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fsync_001
 * @tc.desc: Verify whether the fsync function behaves as expected under different file descriptor states.
 * @tc.type: FUNC
 */
HWTEST_F(UnistdFsyncTest, fsync_001, TestSize.Level1)
{
    int fd;

    EXPECT_NE(-1, fd = open("/data/local/tmp", O_RDONLY));
    EXPECT_EQ(0, fsync(fd));
    close(fd);

    EXPECT_NE(0, fd = open("/data/local/tmp", O_RDWR));
    EXPECT_EQ(-1, fsync(fd));
    close(fd);
}