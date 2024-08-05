#include <gtest/gtest.h>
#include <string.h>

constexpr size_t TEST_SIZE = 10;
constexpr int COMPER_NUMBER = 7;
constexpr int COMPER_NUMBER_TWO = 9;

using namespace testing::ext;

class FortifyStrcatchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" char* __strcat_chk(char* dest, const char* src, size_t dst_buf_size);

/**
 * @tc.name: __strcat_chk_001
 * @tc.desc: Verify whether the __strcat_chk can correctly handle the results and avoid buffer overflow when connecting
 *           strings.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyStrcatchkTest, __strcat_chk_001, TestSize.Level1)
{
    char src[TEST_SIZE] = { 0 };
    src[0] = 'a';
    src[1] = '\0';
    char* result = __strcat_chk(src, "aaaaa", sizeof(src));
    EXPECT_STREQ("aaaaaa", result);
    EXPECT_EQ('\0', src[COMPER_NUMBER]);
}

/**
 * @tc.name: __strcat_chk_002
 * @tc.desc: Verify whether the __strcat_chk can correctly handle the results and avoid buffer overflow when connecting
 *           long strings.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyStrcatchkTest, __strcat_chk_002, TestSize.Level1)
{
    char src[TEST_SIZE] = { 0 };
    src[0] = 'a';
    src[1] = '\0';
    EXPECT_STREQ("aaaaaaaaa", __strcat_chk(src, "aaaaaaaa", sizeof(src)));
    EXPECT_EQ('\0', src[COMPER_NUMBER_TWO]);
}

/**
 * @tc.name: __strcat_chk_003
 * @tc.desc: Verify whether the __strcat_chk can correctly process the result and avoid buffer overflow if the specified
 *           buffer size is an invalid value (-1) when connecting strings.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyStrcatchkTest, __strcat_chk_003, TestSize.Level1)
{
    char src[TEST_SIZE] = { 0 };
    src[0] = 'a';
    src[1] = '\0';
    EXPECT_STREQ("aaaaaaaaa", __strcat_chk(src, "aaaaaaaa", static_cast<size_t>(-1)));
    EXPECT_EQ('\0', src[COMPER_NUMBER_TWO]);
}