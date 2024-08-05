#include <gtest/gtest.h>
#include <stdio.h>
using namespace testing::ext;

class StdioFputcunlockedTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};
constexpr int NUM = 65;
/**
 * @tc.name: fputc_unlocked_001
 * @tc.desc: Ensure that the fputc_unlocked() function correctly writes a character to a file without any
 *           locking mechanisms.
 * @tc.type: FUNC
 **/
HWTEST_F(StdioFputcunlockedTest, fputc_unlocked_001, TestSize.Level1)
{
    FILE* file;
    char ch = 'A';
    file = fopen("/proc/version", "w");
    ASSERT_NE(nullptr, file);
    int result = fputc_unlocked(ch, file);
    EXPECT_EQ(NUM, result);
    fclose(file);
}