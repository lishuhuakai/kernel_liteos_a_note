#include <gtest/gtest.h>
#include <sched.h>

using namespace testing::ext;

class LinuxUnshareTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};
constexpr int NUM = -10;
/**
 * @tc.name: unshare_001
 * @tc.desc: Verify that the unshare function can successfully create a new PID namespace by using the
 *           CLONE_NEWPID flag.
 * @tc.type: FUNC
 **/
HWTEST_F(LinuxUnshareTest, unshare_001, TestSize.Level1)
{
    pid_t pid = fork();
    ASSERT_NE(-1, pid);

    if (pid == 0) {
        int result = unshare(CLONE_NEWPID);
        EXPECT_EQ(0, result);
        exit(0);
    } else {
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
        }
    }
}

/**
 * @tc.name: unshare_002
 * @tc.desc: Verify the behavior of the unshare() function when an invalid flag is provided as an argument,
 *           expecting the function call to fail and return -1.
 * @tc.type: FUNC
 **/
HWTEST_F(LinuxUnshareTest, unshare_002, TestSize.Level1)
{
    int result = unshare(NUM);
    EXPECT_EQ(-1, result);
}