#include <gtest/gtest.h>
#include <stdio.h>

using namespace testing::ext;

class StdioFeofunlockedTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: feof_unlocked_001
 * @tc.desc: Test the usage of feof_unlocked to check the end-of-file indicator on a file stream, ensuring that it
 *           accurately reflects whether there are more characters to be read from the file.
 * @tc.type: FUNC
 **/
HWTEST_F(StdioFeofunlockedTest, feof_unlocked_001, TestSize.Level1)
{
    FILE* file;
    char ch;
    file = fopen("/proc/version", "r");
    ASSERT_NE(nullptr, file);
    EXPECT_TRUE(!feof_unlocked(file));
    ch = fgetc(file);
    fclose(file);
}