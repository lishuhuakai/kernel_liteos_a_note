#include <errno.h>
#include <gtest/gtest.h>
#include <poll.h>
#include <sys/wait.h>

using namespace testing::ext;

class SelectPollTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: poll_001
 * @tc.desc: This test verifies the behavior of the poll() function when passing an empty pollfd array and a timeout of
 *           1 millisecond.
 * @tc.type: FUNC
 */
HWTEST_F(SelectPollTest, poll_001, TestSize.Level1)
{
    errno = 0;
    int emptyPoll = poll(nullptr, 0, 1);
    EXPECT_TRUE(emptyPoll == 0);
    EXPECT_TRUE(errno == 0);
}