#include <gtest/gtest.h>
#include <string.h>

constexpr int BUF_SIZE = 20;

extern "C" void* __mempcpy_chk(void* dest, const void* src, size_t count, size_t dst_len);

using namespace testing::ext;

class FortifyMempcpychkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __mempcpy_chk_001
 * @tc.desc: Verify if the function is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyMempcpychkTest, __mempcpy_chk_001, TestSize.Level1)
{
    const char* src = "Hello, World!";
    char dest[BUF_SIZE];

    EXPECT_TRUE(__mempcpy_chk(dest, src, strlen(src), sizeof(dest)));
    EXPECT_STREQ(src, dest);
}

/**
 * @tc.name: __mempcpy_chk_002
 * @tc.desc: We will check the length of the target buffer to avoid the risk of buffer overflow.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyMempcpychkTest, __mempcpy_chk_002, TestSize.Level1)
{
    const char* src = "Hello, World!";
    char dest[BUF_SIZE];

    EXPECT_TRUE(__mempcpy_chk(dest, src, strlen(src), static_cast<size_t>(-1)));
    EXPECT_STREQ(src, dest);
}