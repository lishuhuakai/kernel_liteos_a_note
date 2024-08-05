#include <gtest/gtest.h>
#include <fortify/string.h>

#define MAX_SIZE 1024
constexpr int CANCEL_NUMBER = 100;

using namespace testing::ext;

class FortifyStrrchrchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" char* __strrchr_chk(const char* s, int c, size_t s_len);

/**
 * @tc.name: __strrchr_chk_001
 * @tc.desc: Verify if the __strrchr_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyStrrchrchkTest, __strrchr_chk_001, TestSize.Level1)
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
        char* result = __strrchr_chk(srcChar, dest, obj);
        EXPECT_STREQ(excepted, result);
    }
}
