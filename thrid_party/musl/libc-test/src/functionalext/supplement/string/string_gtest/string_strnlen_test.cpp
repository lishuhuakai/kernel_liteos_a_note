#include <gtest/gtest.h>
#include <string.h>

constexpr int STR_SIZE_ONE = 4;

using namespace testing::ext;

class StringStrnlenTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strnlen_001
 * @tc.desc: Verify that it functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrnlenTest, strnlen_001, TestSize.Level1)
{
    char srcChar[] = "test\0 strlen";
    int srcLen = sizeof(srcChar) / sizeof(srcChar[0]) - 1;
    EXPECT_EQ(STR_SIZE_ONE, strnlen(srcChar, srcLen));

    char dstChar[] = "test strlen";
    srcLen = sizeof(dstChar) / sizeof(dstChar[0]) - 1;
    EXPECT_EQ(srcLen, strnlen(dstChar, srcLen));
}