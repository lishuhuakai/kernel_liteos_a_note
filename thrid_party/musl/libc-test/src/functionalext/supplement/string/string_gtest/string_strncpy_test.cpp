#include <gtest/gtest.h>
#include <string.h>

constexpr int STR_SIZE_ONE = 10;
constexpr int STR_SIZE_TWO = 14;
constexpr int STR_SIZE_THREE = 15;

using namespace testing::ext;

class StringStrncpyTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strncpy_001
 * @tc.desc: Verify that it functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrncpyTest, strncpy_001, TestSize.Level1)
{
    const char* srcChar = "012345678";
    char dstChar[STR_SIZE_ONE];

    strncpy(dstChar, srcChar, STR_SIZE_ONE);
    EXPECT_STREQ("012345678", dstChar);

    char dstChar2[STR_SIZE_THREE];

    strncpy(dstChar2, srcChar, STR_SIZE_THREE);
    EXPECT_STREQ("012345678", dstChar2);
    EXPECT_EQ('\0', dstChar2[STR_SIZE_TWO]);

    srcChar = nullptr;
}