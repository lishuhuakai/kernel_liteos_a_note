#include <gtest/gtest.h>
#include <stdio.h>
using namespace testing::ext;

class StdioFilenounlockedTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fileno_unlocked_001
 * @tc.desc: Ensure that the fileno_unlocked() function correctly retrieves the file descriptor associated with a file.
 * @tc.type: FUNC
 **/
HWTEST_F(StdioFilenounlockedTest, fileno_unlocked_001, TestSize.Level1)
{
    FILE* file = fopen("/proc/version", "r");
    ASSERT_NE(nullptr, file);
    int fd = fileno_unlocked(file);
    EXPECT_NE(-1, fd);
    fclose(file);
}