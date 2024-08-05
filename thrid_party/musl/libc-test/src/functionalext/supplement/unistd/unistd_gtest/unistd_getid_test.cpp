#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdGetidTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: getuid_001
 * @tc.desc: checks whether the getuid functions behave as expected and return the correct
 *           user and group IDs.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdGetidTest, getuid_001, TestSize.Level1)
{
    EXPECT_EQ(getuid(), getauxval(AT_UID));
}

/**
 * @tc.name: geteuid_001
 * @tc.desc: checks whether the geteuid functions behave as expected and return the correct
 *           user and group IDs.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdGetidTest, geteuid_001, TestSize.Level1)
{
    EXPECT_EQ(geteuid(), getauxval(AT_EUID));
}

/**
 * @tc.name: getgid_001
 * @tc.desc: checks whether getgid functions behave as expected and return the correct
 *           user and group IDs.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdGetidTest, getgid_001, TestSize.Level1)
{
    EXPECT_EQ(getgid(), getauxval(AT_GID));
}

/**
 * @tc.name: getegid_001
 * @tc.desc: checks whether getegid functions behave as expected and return the correct
 *           user and group IDs.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdGetidTest, getegid_001, TestSize.Level1)
{
    EXPECT_EQ(getegid(), getauxval(AT_EGID));
}