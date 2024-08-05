#include <gtest/gtest.h>
#include <string.h>

constexpr int BUF_SIZE = 10;

using namespace testing::ext;

class FortifyStrncpychkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" void __strncpy_chk(char* dest, const char* src, size_t len, size_t dst_len);

/**
 * @tc.name: __strncpy_chk_001
 * @tc.desc: Verify if the function is functioning properly.
 * @tc.type: FUNC
 * */

HWTEST_F(FortifyStrncpychkTest, __strncpy_chk_001, TestSize.Level1)
{
    const char* srcChar = "012345678";
    char dstChar[BUF_SIZE];
    __strncpy_chk(dstChar, srcChar, BUF_SIZE, BUF_SIZE);
    EXPECT_STREQ("012345678", dstChar);
    srcChar = nullptr;
}

/**
 * @tc.name: __strncpy_chk_002
 * @tc.desc: Verify whether the __strncpy_chk can correctly process the result and avoid buffer overflow if the
 *           specified buffer size is an invalid value (-1) when connecting strings.
 * @tc.type: FUNC
 * */

HWTEST_F(FortifyStrncpychkTest, __strncpy_chk_002, TestSize.Level1)
{
    const char* srcChar = "012345678";
    char dstChar[BUF_SIZE];
    __strncpy_chk(dstChar, srcChar, BUF_SIZE, static_cast<size_t>(-1));
    EXPECT_STREQ("012345678", dstChar);
    srcChar = nullptr;
}