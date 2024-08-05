#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

class StringBcmpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: bcmp_001
 * @tc.desc: Verify that the bcmp function returns the expected result of 0 when comparing two strings with identical
 *           content.
 * @tc.type: FUNC
 */
HWTEST_F(StringBcmpTest, bcmp_001, TestSize.Level1)
{
    const char* str1 = "Hello";
    const char* str2 = "Hello";
    EXPECT_EQ(0, bcmp(str1, str2, strlen(str1)));
}

/**
 * @tc.name: bcmp_002
 * @tc.desc: Verify that if two strings are not equal within the given length, the return value of bcmp function is not
 * *         equal to the length of the first string.
 * @tc.type: FUNC
 */
HWTEST_F(StringBcmpTest, bcmp_002, TestSize.Level1)
{
    const char* str1 = "hello";
    const char* str2 = "world";
    EXPECT_NE(bcmp(str1, str2, strlen(str1)), strlen(str1));
}