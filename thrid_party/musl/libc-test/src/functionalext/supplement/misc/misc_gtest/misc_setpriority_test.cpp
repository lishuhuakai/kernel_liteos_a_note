#include <gtest/gtest.h>
#include <sys/resource.h>

using namespace testing::ext;

class MiscSetpriorityTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: setpriority_001
 * @tc.desc: This test verifies whether the setpriority function can correctly set the scheduling priority of the
 *           current process to 1 and return 0 when set successfully.
 * @tc.type: FUNC
 */
HWTEST_F(MiscSetpriorityTest, setpriority_001, TestSize.Level1)
{
    int setResult = setpriority(PRIO_PROCESS, getpid(), 1);
    EXPECT_EQ(0, setResult);
}