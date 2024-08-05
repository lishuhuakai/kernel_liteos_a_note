#include <gtest/gtest.h>
#include <string.h>

constexpr int STR_SIZE_ONE = 4;
constexpr int STR_SIZE_TWO = 12;

using namespace testing::ext;

class StringStrlenTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strlen_001
 * @tc.desc: Verify that it meets \0 and functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrlenTest, strlen_001, TestSize.Level1)
{
    char srcChar[] = "test\0 strlen";
    int srcLen = sizeof(srcChar) / sizeof(srcChar[0]) - 1;
    EXPECT_EQ(STR_SIZE_ONE, strlen(srcChar));
    EXPECT_EQ(STR_SIZE_TWO, srcLen);
}