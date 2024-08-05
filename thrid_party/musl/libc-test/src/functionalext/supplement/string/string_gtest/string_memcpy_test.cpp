#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

constexpr size_t SRC_SIZE = 1024;

class StringMemcpyTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: memcpy_001
 * @tc.desc: Verify that memcpy can accurately copy an entire string including the null terminator.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemcpyTest, memcpy_001, TestSize.Level1)
{
    char dst[] = "Hello, World!";
    char src[SRC_SIZE];
    memcpy(src, dst, sizeof(dst));
    EXPECT_STREQ(src, dst);
}

/**
 * @tc.name: memcpy_002
 * @tc.desc: Check if memcpy does not alter the source string when copying zero bytes.
 * @tc.type: FUNC
 */
HWTEST_F(StringMemcpyTest, memcpy_002, TestSize.Level1)
{
    const char* dst = "Hello, World!";
    char src[SRC_SIZE] = {};
    memcpy(src, dst, 0);
    EXPECT_STREQ(src, "");
}