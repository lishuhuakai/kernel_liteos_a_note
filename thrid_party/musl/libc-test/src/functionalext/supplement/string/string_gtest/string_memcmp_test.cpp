#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

class StringMemcmpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: memcmp_001
 * @tc.desc: Validate the behavior of the memcmp function when comparing two identical strings, ensuring that memcmp
 *           correctly compares the strings and returns the expected result of 0.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemcmpTest, memcmp_001, TestSize.Level1)
{
    const char* str1 = "Hello, world!";
    const char* str2 = "Hello, world!";
    int result = memcmp(str1, str2, strlen(str1));
    EXPECT_EQ(result, 0);
}

/**
 * @tc.name: memcmp_002
 * @tc.desc: Validate the behavior of the memcmp function when comparing two strings of different lengths, ensuring
 *           that memcmp correctly compares the strings and returns a result greater than 0.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemcmpTest, memcmp_002, TestSize.Level1)
{
    const char* str1 = "Hello, world!";
    const char* str2 = "Hello";
    int result = memcmp(str1, str2, strlen(str1));
    EXPECT_GT(result, 0);
}

/**
 * @tc.name: memcmp_003
 * @tc.desc: Validate the behavior of the memcmp function when comparing strings of different lengths, ensuring that it
 *           compares according to the specified length and returns the expected result of 0.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemcmpTest, memcmp_003, TestSize.Level1)
{
    const char* str1 = "Hello, world!";
    const char* str2 = "Hello";
    int result = memcmp(str1, str2, strlen(str2));
    EXPECT_EQ(result, 0);
}

/**
 * @tc.name: memcmp_004
 * @tc.desc: Validate the behavior of the memcmp function when comparing null pointers or strings with a length of 0,
 *           ensuring that memcmp correctly handles this scenario and returns the expected result of 0.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemcmpTest, memcmp_004, TestSize.Level1)
{
    const char* str1 = nullptr;
    const char* str2 = nullptr;
    int result = memcmp(str1, str2, 0);
    EXPECT_EQ(result, 0);
}