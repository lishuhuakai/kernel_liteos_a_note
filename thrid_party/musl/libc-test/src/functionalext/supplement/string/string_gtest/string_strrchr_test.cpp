#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

class StringStrrchrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strrchr_001
 * @tc.desc: Verify that it functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrrchrTest, strrchr_001, TestSize.Level1)
{
    const char* srcChar = "source";
    EXPECT_STREQ("ce", strrchr(srcChar, 'c'));

    srcChar = "abcdefg";
    EXPECT_STREQ(NULL, strrchr(srcChar, 'h'));

    srcChar = nullptr;
}