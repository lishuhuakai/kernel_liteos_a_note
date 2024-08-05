#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>

constexpr int BUF_SIZE = 100;
constexpr int EXPECT_NUM = 3;

using namespace testing::ext;

class FortifyVsprintfchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" int __vsprintf_chk(char* dest, int flags, size_t dst_len_from_compiler, const char* format, va_list va);

static int VsprintfHelper(char* str, size_t size, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = __vsprintf_chk(str, 0, size - 1, format, args);
    va_end(args);
    return result;
}

/**
 * @tc.name: __strcat_chk_001
 * @tc.desc: Verify whether the specified content will be copied after the output of this function.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyVsprintfchkTest, __vsprintf_chk_001, TestSize.Level1)
{
    char buffer[BUF_SIZE];
    const char* format = "%d";
    int value = 666;

    int result = VsprintfHelper(buffer, sizeof(buffer), format, value);
    EXPECT_EQ(EXPECT_NUM, result);
    EXPECT_STREQ("666", buffer);
}