#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

class StringStrcmpTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strcmp_001
 * @tc.desc: Verify that the strcmp function should return 0 when comparing two completely identical strings.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrcmpTest, strcmp_001, TestSize.Level1)
{
    const char* str1 = "Hello";
    const char* str2 = "Hello";
    int result = strcmp(str1, str2);
    EXPECT_EQ(result, 0);
}

/**
 * @tc.name: strcmp_002
 * @tc.desc: Verify that the strcmp function should return a non-zero value when comparing two different strings.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrcmpTest, strcmp_002, TestSize.Level1)
{
    const char* str1 = "Hello";
    const char* str2 = "World";
    int result = strcmp(str1, str2);
    EXPECT_NE(result, 0);
}

/**
 * @tc.name: strcmp_003
 * @tc.desc: Verify that the strcmp function should return 0 when comparing two null pointer.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrcmpTest, strcmp_003, TestSize.Level1)
{
    const char* str1 = nullptr;
    const char* str2 = nullptr;
    int result = strcmp(str1, str2);
    EXPECT_EQ(result, 0);
}