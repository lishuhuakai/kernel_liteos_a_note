#include <gtest/gtest.h>
#include <stdio.h>
#include <sys/prctl.h>

using namespace testing::ext;

class LinuxPrctlTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: prctl_001
 * @tc.desc: This test verifies that after calling the prctl function to set the thread name, the thread name is
 *           obtained by calling the prctl function and compared with the set thread name to ensure that the function of
 *           setting the thread name works properly.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxPrctlTest, prctl_001, TestSize.Level1)
{
    const char* threadName = "ThreadName";
    int setResult = prctl(PR_SET_NAME, static_cast<const void*>(threadName));
    EXPECT_EQ(0, setResult);
    char buffer[BUFSIZ];
    memset(buffer, 0, sizeof(buffer));
    int getResult = prctl(PR_GET_NAME, static_cast<void*>(buffer));
    EXPECT_EQ(0, getResult);
    EXPECT_STREQ(threadName, buffer);
}
