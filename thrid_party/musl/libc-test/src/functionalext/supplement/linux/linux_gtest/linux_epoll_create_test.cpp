#include <gtest/gtest.h>
#include <sys/epoll.h>

using namespace testing::ext;

class LinuxEpollcreateTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: epoll_create_001
 * @tc.desc: This test verifies epoll_create function correctly create an epoll instance, if the test passes,
 *           it indicates that epoll the function of the create1 function is normal.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxEpollcreateTest, epoll_create_001, TestSize.Level1)
{
    int epollFd = epoll_create(1);
    ASSERT_NE(epollFd, -1);
    close(epollFd);
}
