#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/ioctl.h>

using namespace testing::ext;

class MiscIoctlTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: ioctl_001
 * @tc.desc: This test shows whether the system will return -1 as expected when using an illegal ioctl command number.
 * @tc.type: FUNC
 */
HWTEST_F(MiscIoctlTest, ioctl_001, TestSize.Level1)
{
    int fd = open("/dev/null", O_RDONLY);
    ASSERT_NE(fd, -1);
    int result = ioctl(fd, 0xffff, 0);
    EXPECT_TRUE(result == -1);
    close(fd);
}