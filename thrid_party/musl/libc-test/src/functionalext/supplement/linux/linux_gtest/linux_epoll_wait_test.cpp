#include <errno.h>
#include <gtest/gtest.h>
#include <signal.h>
#include <sys/epoll.h>
#include <thread>

using namespace testing::ext;

class LinuxEpollwaitTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr int WRITE_SIZE = 5;
constexpr int MAX_EVENTS = 10;

/**
 * @tc.name: epoll_wait_001
 * @tc.desc: This test verifies epoll can the wait function correctly wait for events in the epoll instance and return
 *           the correct results? if the test passes, it indicates that epoll the wait function is functioning properly.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxEpollwaitTest, epoll_wait_001, TestSize.Level1)
{
    int epollFd = epoll_create(1);
    ASSERT_NE(epollFd, -1);
    int fds[2];
    pipe(fds);
    epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fds[0];
    ASSERT_EQ(epoll_ctl(epollFd, EPOLL_CTL_ADD, fds[0], &ev), 0);
    std::thread writerThread([&]() { write(fds[1], "Hello", WRITE_SIZE); });
    epoll_event events[MAX_EVENTS];
    int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, -1);
    EXPECT_GT(numEvents, 0);
    writerThread.join();
    close(epollFd);
}