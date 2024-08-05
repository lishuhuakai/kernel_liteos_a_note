#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

class StringStrpbrkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strpbrk_001
 * @tc.desc: Verify that it functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrpbrkTest, strpbrk_001, TestSize.Level1)
{
    const char* srcChar = "source";
    const char* dstChar = "target";
    EXPECT_STREQ("rce", strpbrk(srcChar, dstChar));

    srcChar = "abcdefg";
    dstChar = "hijklmn";
    EXPECT_STREQ(nullptr, strpbrk(srcChar, dstChar));

    srcChar = nullptr;
    dstChar = nullptr;
}