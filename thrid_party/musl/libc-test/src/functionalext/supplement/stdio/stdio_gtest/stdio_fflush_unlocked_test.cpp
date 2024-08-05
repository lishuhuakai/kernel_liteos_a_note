#include <gtest/gtest.h>
#include <stdio.h>
using namespace testing::ext;

class StdioFflushunlockedTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fflush_unlocked_001
 * @tc.desc: Ensure that the fflush_unlocked() function correctly flushes any buffered output to the file.
 * @tc.type: FUNC
 **/
HWTEST_F(StdioFflushunlockedTest, fflush_unlocked_001, TestSize.Level1)
{
    FILE* file = fopen("test_fflush_unlocked", "w");
    ASSERT_NE(nullptr, file);
    fprintf(file, "This is a test message\n");
    int result = fflush_unlocked(file);
    EXPECT_EQ(0, result);
    fclose(file);
}