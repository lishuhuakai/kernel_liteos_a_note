#include <poll.h>
#include <fortify/poll.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class FortifyPpollchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __ppoll_chk_001
 * @tc.desc: Verify if the __ppoll_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyPpollchkTest, __ppoll_chk_001, TestSize.Level1)
{
    struct pollfd fds[1];
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    struct timespec ts;
    ts.tv_nsec = 1000;
    int ret = __ppoll_chk(fds, 1, &ts, nullptr, sizeof(fds));
    EXPECT_GE(ret, 0);
}

/**
 * @tc.name: __ppoll_chk_002
 * @tc.desc: Set the fds parameter to nullptr and verify if the function will return -1.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyPpollchkTest, __ppoll_chk_002, TestSize.Level1)
{
    int nfds = 1;
    struct pollfd* fds = nullptr;
    const struct timespec* timeout = nullptr;
    const sigset_t* sigMask = nullptr;
    int ret = __ppoll_chk(fds, nfds, timeout, sigMask, sizeof(struct pollfd));
    EXPECT_EQ(-1, ret);
}