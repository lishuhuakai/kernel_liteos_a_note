#include <gtest/gtest.h>
#include <string.h>

constexpr int STR_SIZE_ONE = 1;
constexpr int STR_SIZE_TWO = 2;
constexpr int STR_SIZE_THREE = 9;
constexpr int STR_SIZE_FOUR = 10;

using namespace testing::ext;

class StringStrstrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strstr_001
 * @tc.desc: Verify that it functions normally
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrstrTest, strstr_001, TestSize.Level1)
{
    const char* srcChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    EXPECT_EQ(srcChar, strstr(srcChar, ""));

    EXPECT_EQ(srcChar, strstr(srcChar, "A"));
    EXPECT_EQ(srcChar + STR_SIZE_ONE, strstr(srcChar, "B"));
    EXPECT_EQ(srcChar + STR_SIZE_TWO, strstr(srcChar, "C"));

    EXPECT_EQ(srcChar + STR_SIZE_THREE, strstr(srcChar, "J"));
    EXPECT_EQ(srcChar + STR_SIZE_FOUR, strstr(srcChar, "K"));

    EXPECT_EQ(srcChar + STR_SIZE_THREE, strstr(srcChar, "JKL"));

    srcChar = nullptr;
}