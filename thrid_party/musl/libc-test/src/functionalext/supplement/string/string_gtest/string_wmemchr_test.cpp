#include <gtest/gtest.h>
#include <wchar.h>

using namespace testing::ext;

class StringWmemchrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: wmemchr_001
 * @tc.desc: Verify that it functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringWmemchrTest, wmemchr_001, TestSize.Level1)
{
    wchar_t str[] = L"ABCDEFG";
    wchar_t target = L'C';
    EXPECT_EQ(L'C', *wmemchr(str, target, wcslen(str)));

    EXPECT_EQ(nullptr, wmemchr(str, target, 0));

    wchar_t target2 = L'H';
    EXPECT_EQ(nullptr, wmemchr(str, target2, wcslen(str)));
}