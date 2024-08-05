#include <poll.h>
#include <fortify/poll.h>
#include <gtest/gtest.h>

using namespace testing::ext;
constexpr int TIME_OUT = 1000;

class FortifyPollchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

class FortifyPollchkDeathTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __ppoll_chk_001
 * @tc.desc: Verify if the __poll_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyPollchkTest, __poll_chk_001, TestSize.Level1)
{
    struct pollfd fds[1];
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    int ret = __poll_chk(fds, 0, 1, sizeof(fds));
    EXPECT_GE(ret, 0);
}

/**
 * @tc.name: __poll_chk_002
 * @tc.desc: Pass an incorrect buffer size to the function.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyPollchkDeathTest, __poll_chk_002, TestSize.Level1)
{
    struct pollfd fds[1];
    fds[0].fd = 1;
    fds[0].events = POLLIN;
    nfds_t nfds = 1;
    size_t fdsLen = sizeof(struct pollfd) * (nfds - 1);
    EXPECT_DEATH(__poll_chk(fds, nfds, TIME_OUT, fdsLen), "");
}