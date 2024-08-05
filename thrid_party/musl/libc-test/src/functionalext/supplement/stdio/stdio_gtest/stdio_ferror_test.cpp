#include <gtest/gtest.h>
#include <stdio.h>
using namespace testing::ext;

class StdioFerrorTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: ferror_001
 * @tc.desc: Ensure that the ferror() function correctly detects any errors that occurred during file operations.
 * @tc.type: FUNC
 **/
HWTEST_F(StdioFerrorTest, ferror_001, TestSize.Level1)
{
    FILE* file = fopen("/proc/version", "r");
    ASSERT_TRUE(file);
    EXPECT_EQ(0, ferror(file));
    fclose(file);
}