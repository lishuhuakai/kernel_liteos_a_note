#include <cstdlib>
#include <errno.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace testing::ext;

class StdioClearerrunlockedTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: clearerr_unlocked_001
 * @tc.desc: Test the usage of clearerr_unlocked to clear error indicators on a file stream, ensuring the stream
 *           is ready for further operations.
 * @tc.type: FUNC
 **/
HWTEST_F(StdioClearerrunlockedTest, clearerr_unlocked_001, TestSize.Level1)
{
    FILE* file;
    int ch;
    errno = 0;
    file = fopen("/proc/version", "r");
    ASSERT_NE(nullptr, file);
    while ((ch = fgetc(file)) != EOF) {
    }
    clearerr_unlocked(file);
    EXPECT_EQ(0, errno);
    fclose(file);
}