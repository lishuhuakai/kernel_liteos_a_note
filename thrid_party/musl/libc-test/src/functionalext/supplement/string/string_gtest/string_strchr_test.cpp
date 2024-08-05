#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

constexpr size_t BUF_SIZE = 10;
constexpr size_t STR_SIZE = 128;

class StringStrchrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strchr_001
 * @tc.desc: Verify that when the null character '\0' is found in the given string, the strchr function can return a
 * *         pointer pointing to that null character.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrchrTest, strchr_001, TestSize.Level1)
{
    char bufChar[BUF_SIZE];
    const char* s = "56789";
    memcpy(bufChar, s, strlen(s) + 1);
    EXPECT_EQ(strchr(bufChar, '\0'), (bufChar + strlen(s)));
}

/**
 * @tc.name: strchr_002
 * @tc.desc: Verify that when searching for the character 'b' in the given string, the strchr function can return a
 * *         pointer to the first matching character 'b'.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrchrTest, strchr_002, TestSize.Level1)
{
    char strChar[STR_SIZE];
    memset(strChar, 'b', sizeof(strChar) - 1);
    strChar[sizeof(strChar) - 1] = '\0';
    for (size_t i = 0; i < sizeof(strChar) - 1; i++) {
        EXPECT_TRUE(strchr(strChar, 'b') == &strChar[i]);
        strChar[i] = 'c';
    }
}