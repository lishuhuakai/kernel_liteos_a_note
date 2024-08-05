#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class DirentScandirTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: scandir_001
 * @tc.desc: Verify whether the scandir function can return the expected error code when scanning a non-existent
 *           directory.
 * @tc.type: FUNC
 */
HWTEST_F(DirentScandirTest, scandir_001, TestSize.Level1)
{
    dirent** result = nullptr;
    errno = 0;
    EXPECT_TRUE(scandir("/fake-dir", &result, nullptr, nullptr) == -1);
    EXPECT_TRUE(ENOENT == errno);
    EXPECT_EQ(result, nullptr);
}