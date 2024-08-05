#include <dirent.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class DirentClosedirTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: closedir_001
 * @tc.desc: Verify that the closedir function can successfully close an open directory and ensure that its return value
 *           is 0 to confirm whether it has performed the expected operation of closing the directory.
 * @tc.type: FUNC
 */
HWTEST_F(DirentClosedirTest, closedir_001, TestSize.Level1)
{
    DIR* dir = opendir("/proc/self");
    ASSERT_NE(dir, nullptr);
    EXPECT_EQ(closedir(dir), 0);
}