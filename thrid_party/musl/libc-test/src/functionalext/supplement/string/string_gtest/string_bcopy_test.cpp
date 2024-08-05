#include <gtest/gtest.h>
#include <string.h>

constexpr int MAX_SIZE = 1024;
constexpr int SUB_NUM = 1;
constexpr int DIV_NUM = 2;

using namespace testing::ext;

class StringStrdupTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: bcopy_001
 * @tc.desc: Verify bcopy functionality is normal
 * @tc.type: FUNC
 * */
HWTEST_F(StringStrdupTest, bcopy_001, TestSize.Level1)
{
    char srcChar[MAX_SIZE + SUB_NUM], dstChar[MAX_SIZE + SUB_NUM];
    for (int i = 0; i < MAX_SIZE; i++) {
        memset(srcChar, 'A', MAX_SIZE / DIV_NUM);
        memset(srcChar + MAX_SIZE / DIV_NUM, 'a', MAX_SIZE / DIV_NUM);
        memcpy(dstChar, srcChar, MAX_SIZE);

        size_t obj = random() % MAX_SIZE;
        size_t pos = random() % (MAX_SIZE - obj);
        memcpy(dstChar + pos, srcChar, obj);
        bcopy(srcChar, srcChar + pos, obj);
        int result = memcmp(srcChar, dstChar, MAX_SIZE);
        EXPECT_EQ(0, result);
    }
}