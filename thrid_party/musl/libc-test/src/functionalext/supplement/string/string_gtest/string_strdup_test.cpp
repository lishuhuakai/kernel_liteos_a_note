#include <gtest/gtest.h>
#include <string.h>
#include <wchar.h>

using namespace testing::ext;

class StringStrdupTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strdup_001
 * @tc.desc: Verify its functionality is normal
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrdupTest, strdup_001, TestSize.Level1)
{
    EXPECT_STREQ("strduptest", strdup("strduptest"));
}