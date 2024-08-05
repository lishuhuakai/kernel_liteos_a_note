#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

class StringStrcasecmpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strcasecmp_001
 * @tc.desc: Verify the correctness of the strcasecmp function for case-insensitive comparison of strings, including
 *           comparisons for equality, less than, and greater than scenarios.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrcasecmpTest, strcasecmp_001, TestSize.Level1)
{
    EXPECT_TRUE(strcasecmp("world", "WORLD") == 0);
    EXPECT_TRUE(strcasecmp("world1", "world2") < 0);
    EXPECT_TRUE(strcasecmp("world2", "world1") > 0);
}