#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

constexpr size_t FOUND_LEN = 5;

class StringStrcspnTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strcspn_001
 * @tc.desc: Verify the correctness of the strcspn function in searching for a specified set of characters within a
 *           given string, including validation of the returned matching position.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrcspnTest, strcspn_001, TestSize.Level1)
{
    const char* str1 = "Hello,world!";
    const char* str2 = " ,!";
    size_t count = strcspn(str1, str2);
    EXPECT_EQ(count, FOUND_LEN);
}

/**
 * @tc.name: strcspn_002
 * @tc.desc: Verify that when no characters from the specified character set are found in the source string, the strcspn
 *           function will return the length of the source string.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrcspnTest, strcspn_002, TestSize.Level1)
{
    const char* str1 = "Hello,world!";
    const char* str2 = "xyz";
    size_t count = strcspn(str1, str2);
    EXPECT_EQ(count, strlen(str1));
}