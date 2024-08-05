#include <gtest/gtest.h>
#include <string.h>

using namespace testing::ext;

constexpr size_t SRC_SIZE = 1024;
constexpr size_t DST_SIZE = 1024;
constexpr size_t RAND_LEN = 512;

class StringStrcatTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: strcat_001
 * @tc.desc: Verify the correctness of the strcat function for string concatenation, including validation of the lengths
 *           of the destination and source strings, handling of null terminators, and correctness of the concatenated
 *           string.
 * @tc.type: FUNC
 */
HWTEST_F(StringStrcatTest, strcat_001, TestSize.Level1)
{
    char dstChar[DST_SIZE], srcChar[SRC_SIZE];
    memset(dstChar, 'A', DST_SIZE);
    dstChar[DST_SIZE - 1] = '\0';
    memset(srcChar, 'M', RAND_LEN);
    srcChar[random() % RAND_LEN] = '\0';
    srcChar[RAND_LEN - 1] = '\0';
    EXPECT_TRUE(strcat(dstChar, srcChar) == dstChar);
}