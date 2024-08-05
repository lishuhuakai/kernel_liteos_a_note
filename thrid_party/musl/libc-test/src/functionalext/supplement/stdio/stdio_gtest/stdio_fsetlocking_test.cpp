#include <gtest/gtest.h>
#include <stdio.h>
#include <stdio_ext.h>

using namespace testing::ext;

class StdioFsetlockingTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};
#define _FSETLOCKING_INTERNAL 0

/**
 * @tc.name: __fsetlocking_001
 * @tc.desc: Verify that the __fsetlocking() function can successfully set the locking mode of a file stream to
 *           internal, and it checks if the function returns the expected success value of 0.
 * @tc.type: FUNC
 */
HWTEST_F(StdioFsetlockingTest, __fsetlocking_001, TestSize.Level1)
{
    FILE* file = fopen("/proc/version", "r");
    ASSERT_TRUE(file);
    int result = __fsetlocking(file, _FSETLOCKING_INTERNAL);
    EXPECT_EQ(0, result);
    fclose(file);
}