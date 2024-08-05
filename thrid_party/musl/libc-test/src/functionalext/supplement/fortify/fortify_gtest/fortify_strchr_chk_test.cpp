#include <gtest/gtest.h>
#include <string.h>
#define __FORTIFY_COMPILATION
#include <fortify/string.h>

#define MAX_SIZE 1024
constexpr int CANCEL_NUMBER = 100;

using namespace testing::ext;

class FortifyStrchrchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" char* __strchr_chk(const char* s, int c, size_t s_len);

/**
 * @tc.name: __strchr_chk_001
 * @tc.desc: Verify if the function is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyStrchrchkTest, __strchr_chk_001, TestSize.Level1)
{
    char srcChar[MAX_SIZE];
    int pos, obj;
    char* excepted;
    for (int i = 0; i < CANCEL_NUMBER; i++) {
        int dest = 'A';
        memset(srcChar, ~dest, MAX_SIZE);
        pos = random() % (MAX_SIZE - 1);
        obj = random() % (MAX_SIZE - 1);
        if (pos > obj) {
            excepted = nullptr;
        } else {
            srcChar[pos] = dest;
            excepted = srcChar + pos;
        }
        char* result = __strchr_chk(srcChar, dest, obj);
        EXPECT_STREQ(excepted, result);
    }
}