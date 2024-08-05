#include <errno.h>
#include <gtest/gtest.h>
#include <sys/epoll.h>

using namespace testing::ext;

class LinuxEpollctlTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: epoll_ctl_001
 * @tc.desc: This test verifies whether successfully adding a file descriptor (fds [0]) to the created epoll instance
 *           will return a successful result.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxEpollctlTest, epoll_ctl_001, TestSize.Level1)
{
    int epollFd = epoll_create(1);
    ASSERT_NE(epollFd, -1);
    int fds[2];
    epoll_event ev;
    EXPECT_TRUE(epoll_ctl(epollFd, EPOLL_CTL_ADD, fds[0], &ev) != -1);
    close(epollFd);
}

/**
 * @tc.name: epoll_ctl_002
 * @tc.desc: This test verifies the call to epoll_ When using the ctl function, use an invalid operation parameter
 *           EPOLL_CTL_MOD (value 3) will cause the function to return -1.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxEpollctlTest, epoll_ctl_002, TestSize.Level1)
{
    int epollFd = epoll_create(1);
    ASSERT_NE(epollFd, -1);
    int fds[2];
    epoll_event ev;
    EXPECT_TRUE(epoll_ctl(epollFd, EPOLL_CTL_MOD, fds[0], &ev) == -1);
    close(epollFd);
}

/**
 * @tc.name: epoll_ctl_003
 * @tc.desc: This test verifies the call to epoll_ When using the ctl function, use EPOLL_CTL_DEL operation parameter
 *           deleting an unregistered file descriptor will result in the function returning -1.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxEpollctlTest, epoll_ctl_003, TestSize.Level1)
{
    int epollFd = epoll_create(1);
    ASSERT_NE(epollFd, -1);
    int fds[2];
    epoll_event ev;
    EXPECT_TRUE(epoll_ctl(epollFd, EPOLL_CTL_DEL, fds[0], &ev) == -1);
    close(epollFd);
}
