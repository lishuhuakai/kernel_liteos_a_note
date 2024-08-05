#include <gtest/gtest.h>
#include <sys/syscall.h>

using namespace testing::ext;

class MiscSyscallTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: syscall_001
 * @tc.desc: This test verifies whether the results obtained by calling the getpid function and syscall (SYS_getpid) to
 *           obtain the process ID are the same.
 * @tc.type: FUNC
 */
HWTEST_F(MiscSyscallTest, syscall_001, TestSize.Level1)
{
    long result = syscall(SYS_getpid);
    EXPECT_TRUE(getpid() == result);
}