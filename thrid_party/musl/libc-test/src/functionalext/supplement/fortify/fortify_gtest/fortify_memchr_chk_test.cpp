#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#define __FORTIFY_COMPILATION
#include <fortify/string.h>

constexpr int MAX_SIZE = 1024;
constexpr int FOR_SIZE = 100;

using namespace testing::ext;

class FortifyMemchrchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __memchr_chk_001
 * @tc.desc: Verify if the __memchr_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyMemchrchkTest, __memchr_chk_001, TestSize.Level1)
{
    char srcChar[MAX_SIZE];
    int seekChar = 'A';
    char* expected;
    int pos, obj;
    for (int i = 0; i < FOR_SIZE; i++) {
        memset(srcChar, ~seekChar, MAX_SIZE);
        pos = random() % MAX_SIZE;
        obj = random() % MAX_SIZE;
        if (pos >= obj) {
            expected = nullptr;
        } else {
            srcChar[pos] = seekChar;
            expected = srcChar + pos;
        }
        EXPECT_TRUE(__memchr_chk(srcChar, seekChar, obj, sizeof(srcChar)) == expected);
    }
}