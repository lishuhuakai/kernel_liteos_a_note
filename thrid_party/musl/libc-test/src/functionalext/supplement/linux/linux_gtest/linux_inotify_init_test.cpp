#include <gtest/gtest.h>
#include <sys/inotify.h>

using namespace testing::ext;

class LinuxInotifyinitTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: inotify_init_001
 * @tc.desc: This test verifies inotify the init function can successfully create an inotify instance and make
 *           corresponding assertion judgments.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxInotifyinitTest, inotify_init_001, TestSize.Level1)
{
    int fd = inotify_init();
    ASSERT_NE(fd, -1);
    close(fd);
}