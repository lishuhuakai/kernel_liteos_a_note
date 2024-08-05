#include <errno.h>
#include <gtest/gtest.h>
#include <syslog.h>
using namespace testing::ext;

class MiscCloselogTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: closelog_001
 * @tc.desc: Ensure that the closelog() function is correctly closing the logging system after the program has
 *           finished logging messages.
 * @tc.type: FUNC
 **/
HWTEST_F(MiscCloselogTest, closelog_001, TestSize.Level1)
{
    openlog("my_program", LOG_PID, LOG_USER);
    syslog(LOG_INFO, "This is a test log message.");
    errno = 0;
    closelog();
    EXPECT_EQ(0, errno);
}