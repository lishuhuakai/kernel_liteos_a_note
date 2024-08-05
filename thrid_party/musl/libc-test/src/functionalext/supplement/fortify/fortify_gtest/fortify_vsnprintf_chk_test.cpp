#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>

constexpr int BUF_SIZE = 100;
constexpr int EXPECT_NUM = 3;
constexpr int NUM = 666;
using namespace testing::ext;

class FortifyVsnprintfchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" int __vsnprintf_chk(
    char* dest, size_t supplied_size, int flags, size_t dst_len_from_compiler, const char* format, va_list va);

static int VsnprintfHelper(char* str, size_t size, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int result = __vsnprintf_chk(str, size - 1, 0, size - 1, format, args);
    va_end(args);
    return result;
}

/**
 * @tc.name: __vsnprintf_chk_001
 * @tc.desc: Verify whether the specified content will be copied after the output of this function.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyVsnprintfchkTest, __vsnprintf_chk_001, TestSize.Level1)
{
    char buffer[BUF_SIZE];
    const char* format = "%d";
    int value = NUM;
    int result = VsnprintfHelper(buffer, sizeof(buffer), format, value);
    EXPECT_EQ(EXPECT_NUM, result);
    EXPECT_STREQ("666", buffer);
}