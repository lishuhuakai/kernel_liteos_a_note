#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

constexpr size_t BUF_SIZE = 10;

class StringStrcpyTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strcpy_001
 * @tc.desc: Verify the behavior of the strcpy interface under normal conditions. It asserts that when the strcpy
 *           function successfully copies the source string to the destination string, the last character of the
 *           destination string should be the null character \0, and all the characters after it should remain unchanged
 *           from the original destination string.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrcpyTest, strcpy_001, TestSize.Level1)
{
    char buffer[BUF_SIZE];
    char* ptr = strdup("12345");
    memset(buffer, 'B', sizeof(buffer));
    EXPECT_TRUE(strcpy(buffer, ptr) == buffer);
    EXPECT_TRUE(buffer[9] == 'B');
    EXPECT_TRUE(buffer[8] == 'B');
    EXPECT_TRUE(buffer[7] == 'B');
    EXPECT_TRUE(buffer[6] == 'B');
    free(ptr);
}

/**
 * @tc.name: strcpy_002
 * @tc.desc: Verify the behavior of the strcpy interface when the source string is an empty string. It asserts that when
 *           the source string is an empty string, the strcpy function should set the destination string to be the null
 *           character \0, and the returned pointer should point to the starting address of the destination string.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrcpyTest, strcpy_002, TestSize.Level1)
{
    char buffer[BUF_SIZE];
    char* ptr = strdup("");
    EXPECT_TRUE(buffer[0] == '\0');
    EXPECT_TRUE(strcpy(buffer, ptr) == buffer);
    free(ptr);
}

/**
 * @tc.name: strcpy_003
 * @tc.desc: Test the behavior with a non-empty source string and verifies that the function can correctly copy the
 *           source string to the destination string and return the correct pointer.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrcpyTest, strcpy_003, TestSize.Level1)
{
    char buffer[BUF_SIZE];
    char* ptr = strdup("147852369");
    memset(buffer, 'B', sizeof(buffer));
    EXPECT_TRUE(strcpy(buffer, ptr) == buffer);
    free(ptr);
}