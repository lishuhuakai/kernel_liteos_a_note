#include <errno.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/epoll.h>

using namespace testing::ext;

class LinuxEpollcreate1Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: epoll_create1_001
 * @tc.desc: This test verifies epoll can the create1 function correctly create an epoll instance? if the test passes,
 *           it indicates that epoll the function of the create1 function is normal.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxEpollcreate1Test, epoll_create1_001, TestSize.Level1)
{
    int fd = epoll_create1(0);
    ASSERT_NE(fd, -1);
    close(fd);
}