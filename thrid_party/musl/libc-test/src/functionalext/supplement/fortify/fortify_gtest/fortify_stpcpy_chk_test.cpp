#include <gtest/gtest.h>
#include <string.h>

constexpr int BUF_SIZE = 10;

using namespace testing::ext;

class FortifyStpcpychkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" char* __stpcpy_chk(char* dest, const char* src, size_t dst_len);

/**
 * @tc.name: __strcpy_chk_001
 * @tc.desc: Verify whether the __stpcpy_chk can correctly process the result and avoid buffer overflow if the specified
 *           buffer size is an invalid value (-1) when connecting strings.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyStpcpychkTest, __stpcpy_chk_001, TestSize.Level1)
{
    char buf[BUF_SIZE];
    memset(buf, 'A', sizeof(buf));

    const char* src = "Hello";
    char* dest = buf;
    __stpcpy_chk(dest, src, static_cast<size_t>(-1));
    EXPECT_STREQ(src, dest);
}