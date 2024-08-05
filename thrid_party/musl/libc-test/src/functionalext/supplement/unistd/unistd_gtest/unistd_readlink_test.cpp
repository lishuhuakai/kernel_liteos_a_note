#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

constexpr size_t BUF_SIZE = 2;

class UnistdReadlinkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: readlink_001
 * @tc.desc: checking the behavior of the readlink function.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdReadlinkTest, readlink_001, TestSize.Level1)
{
    char buf[1];
    EXPECT_EQ(-1, readlink("/dev/null", buf, BUF_SIZE));
}

/**
 * @tc.name: readlink_002
 * @tc.desc: Verify the correctness of the readlink function for symbolic link files in a specific path.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdReadlinkTest, readlink_002, TestSize.Level1)
{
    char path[PATH_MAX];
    size_t length = readlink("/dev/fd", path, sizeof(path));
    ASSERT_LT(0, length);
    EXPECT_EQ("/proc/self/fd", std::string(path, length));
}