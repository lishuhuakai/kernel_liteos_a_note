#include <gtest/gtest.h>
#include <stdio.h>
using namespace testing::ext;

class StdioFpurgeTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

extern "C" int fpurge(FILE*);
/**
 * @tc.name: fpurge_001
 * @tc.desc: Ensure that the fpurge() function correctly discards any buffered data in the file stream.
 * @tc.type: FUNC
 **/
HWTEST_F(StdioFpurgeTest, fpurge_001, TestSize.Level1)
{
    FILE* file = fopen("test_fpurge.txt", "w+");
    ASSERT_NE(nullptr, file);
    fputs("Hello, World!", file);
    int result = fpurge(file);
    EXPECT_EQ(0, result);
    fclose(file);
}