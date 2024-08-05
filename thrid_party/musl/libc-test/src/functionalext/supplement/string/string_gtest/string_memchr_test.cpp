#include <gtest/gtest.h>
#include <stdint.h>
#include <string.h>

using namespace testing::ext;

class StringMemchrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: memchr_001
 * @tc.desc: Test the memchr function to verify if it correctly handles empty string input, and if the target character
 *           does not exist in an empty string, returns nullptr.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemchrTest, memchr_001, TestSize.Level1)
{
    const char* sourceString = "";
    const void* foundChar = memchr(sourceString, 'w', strlen(sourceString));
    EXPECT_EQ(foundChar, nullptr);
}

/**
 * @tc.name: memchr_002
 * @tc.desc: Test the memchr function to verify if it correctly handles the case where the maximum number of bytes to
 *           search is 0 and returns nullptr.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemchrTest, memchr_002, TestSize.Level1)
{
    const char* sourceString = "Hello, world";
    const void* foundChar = memchr(sourceString, 'w', 0);
    EXPECT_EQ(foundChar, nullptr);
}

/**
 * @tc.name: memchr_003
 * @tc.desc: Verify memchr ability to correctly search for a specified character in a given string and return the
 *           correct result.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemchrTest, memchr_003, TestSize.Level1)
{
    const char* sourceString = "Hello, world";
    const void* foundChar = memchr(sourceString, 'w', strlen(sourceString));
    ASSERT_NE(foundChar, nullptr);
}