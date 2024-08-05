#include <fcntl.h>
#include <gtest/gtest.h>
#include <fortify/string.h>
#include <sys/stat.h>

constexpr int BUF_SIZE = 10;
constexpr int BUF_SIZE_TWO = 14;
constexpr int BUF_SIZE_THREE = 15;

using namespace testing::ext;

class FortifyOpenat64chkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __strlcpy_chk_001
 * @tc.desc: Verify if the __strlcpy_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyOpenat64chkTest, __strlcpy_chk_001, TestSize.Level1)
{
    const char* srcChar = "012345678";
    char dstChar[BUF_SIZE];
    __strlcpy_chk(dstChar, srcChar, BUF_SIZE, sizeof(dstChar));
    EXPECT_STREQ("012345678", dstChar);
    char dstChar2[BUF_SIZE_THREE];
    __strlcpy_chk(dstChar2, srcChar, BUF_SIZE_THREE, sizeof(dstChar2));
    EXPECT_STREQ("012345678", dstChar2);
    EXPECT_EQ('\0', dstChar2[BUF_SIZE_TWO]);
    srcChar = nullptr;
}
