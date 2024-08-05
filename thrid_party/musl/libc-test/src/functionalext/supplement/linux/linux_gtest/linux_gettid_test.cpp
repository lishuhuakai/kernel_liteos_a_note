#include <gtest/gtest.h>
#include <sys/types.h>
#include <unistd.h>

using namespace testing::ext;

class LinuxGettidTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: gettid_001
 * @tc.desc: This test verifies whether the thread ID returned by the gettid function is greater than 0 to ensure that
 *           it can successfully obtain a valid thread ID.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxGettidTest, gettid_001, TestSize.Level1)
{
    pid_t tid = gettid();
    EXPECT_GT(tid, 0);
}