#include <gtest/gtest.h>
#include <string.h>

constexpr int BUF_SIZE = 10;
constexpr int BUF_SIZE_TWO = 14;
constexpr int BUF_SIZE_THREE = 15;

using namespace testing::ext;

class FortifyStrcpychkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" char* __strcpy_chk(char* dest, const char* src, size_t dst_len);

/**
 * @tc.name: __strcpy_chk_001
 * @tc.desc: Verify if the __strcpy_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyStrcpychkTest, __strcpy_chk_001, TestSize.Level1)
{
    const char* srcChar = "012345678";
    char dstChar[BUF_SIZE];
    __strcpy_chk(dstChar, srcChar, BUF_SIZE);
    EXPECT_STREQ("012345678", dstChar);
    char dstChar2[BUF_SIZE_THREE];
    __strcpy_chk(dstChar2, srcChar, BUF_SIZE_THREE);
    EXPECT_STREQ("012345678", dstChar2);
    EXPECT_EQ('\0', dstChar2[BUF_SIZE_TWO]);
    srcChar = nullptr;
}