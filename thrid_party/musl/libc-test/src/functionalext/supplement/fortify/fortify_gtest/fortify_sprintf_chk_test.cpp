#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>

constexpr int BUF_SIZE = 1024;

using namespace testing::ext;

class FortifySprintfchkchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" int __sprintf_chk(char* dest, int flags, size_t dst_len_from_compiler, const char* format, ...);

/**
 * @tc.name: __sprintf_chk_001
 * @tc.desc: Verify __sprintf_chk function handle different formatted strings and parameters as expected
 *           to ensure the correctness and security of the function in actual use.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifySprintfchkchkTest, __sprintf_chk_001, TestSize.Level1)
{
    char str[BUF_SIZE];
    int ret = 0;

    ret = __sprintf_chk(str, 0, BUF_SIZE, "test");
    EXPECT_NE(0, ret);
    EXPECT_STREQ(str, "test");

    ret = __sprintf_chk(str, 0, BUF_SIZE, "%c", '1');
    EXPECT_NE(0, ret);
    EXPECT_STREQ(str, "1");

    ret = __sprintf_chk(str, 0, BUF_SIZE, "%s,%s", "123", "456");
    EXPECT_NE(0, ret);
    EXPECT_STREQ(str, "123,456");

    ret = __sprintf_chk(str, 0, BUF_SIZE, "%%test");
    EXPECT_NE(0, ret);
    EXPECT_STREQ(str, "%test");

    ret = __sprintf_chk(str, 0, BUF_SIZE, "%3$d,%2$s,%1$c", 'a', "bc", 0);
    EXPECT_NE(0, ret);
    EXPECT_STREQ(str, "0,bc,a");
}