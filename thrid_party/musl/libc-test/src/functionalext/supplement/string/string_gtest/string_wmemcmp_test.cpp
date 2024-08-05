#include <gtest/gtest.h>
#include <string.h>
#include <wchar.h>

using namespace testing::ext;

class StringWmemcmpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: wmemcmp_001
 * @tc.desc: Verify that it functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringWmemcmpTest, wmemcmp_001, TestSize.Level1)
{
    wchar_t str[] = L"ABCDEFG";
    wchar_t tar[] = L"ABCDEFF";
    EXPECT_EQ(1, wmemcmp(str, tar, wcslen(str)));
    EXPECT_EQ(-1, wmemcmp(tar, str, wcslen(str)));
    EXPECT_EQ(0, wmemcmp(str, str, wcslen(str)));
    EXPECT_EQ(0, wmemcmp(tar, str, 0));
}