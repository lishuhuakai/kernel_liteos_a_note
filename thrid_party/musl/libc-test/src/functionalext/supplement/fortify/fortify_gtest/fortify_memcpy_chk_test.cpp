#include <gtest/gtest.h>
#include <string.h>

constexpr int BUF_SIZE = 20;

using namespace testing::ext;

extern "C" void* __memcpy_chk(void* dest, const void* src, size_t count, size_t dst_len);

class FortifyMemcpychkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __memcpy_chk_001
 * @tc.desc: Verify whether the __memcpy_chk can correctly process the result and avoid buffer overflow if the specified
 *           buffer size is an invalid value (-1) when connecting strings.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyMemcpychkTest, __memcpy_chk_001, TestSize.Level1)
{
    const char* src = "Hello, World!";
    char dest[BUF_SIZE];

    EXPECT_TRUE(__memcpy_chk(dest, src, strlen(src), static_cast<size_t>(-1)));
    EXPECT_STREQ(src, dest);
}

/**
 * @tc.name: __memcpy_chk_002
 * @tc.desc: Verify that the __memcpy_chk function performs copy operations correctly given parameters and prevents
             buffer overflows.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyMemcpychkTest, __memcpy_chk_002, TestSize.Level1)
{
    const char* src = "Hello, World!";
    char dest[BUF_SIZE];

    EXPECT_TRUE(__memcpy_chk(dest, src, strlen(src), sizeof(dest)));
    EXPECT_STREQ(src, dest);
}