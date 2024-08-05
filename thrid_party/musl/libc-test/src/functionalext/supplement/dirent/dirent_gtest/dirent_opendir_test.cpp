#include <dirent.h>
#include <errno.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class DirentOpendirTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: opendir_001
 * @tc.desc: Verify that after using the opendir function to open a directory, the readdir function can read the list of
 *           files in that directory, and the closedir function can close the directory pointer.
 * @tc.type: FUNC
 */
HWTEST_F(DirentOpendirTest, opendir_001, TestSize.Level1)
{
    DIR* dir = opendir("/proc/self");
    ASSERT_NE(dir, nullptr);
    dirent* dir2 = readdir(dir);
    EXPECT_EQ(strcmp(dir2->d_name, "."), 0);
    EXPECT_TRUE(closedir(dir) == 0);
}

/**
 * @tc.name: opendir_002
 * @tc.desc: Verify the correctness of opendir in handling error paths.
 * @tc.type: FUNC
 */
HWTEST_F(DirentOpendirTest, opendir_002, TestSize.Level1)
{
    EXPECT_EQ(opendir("/fake/dir"), nullptr);
    EXPECT_EQ(ENOENT, errno);
    EXPECT_EQ(opendir("/dev/null"), nullptr);
    EXPECT_EQ(ENOTDIR, errno);
}