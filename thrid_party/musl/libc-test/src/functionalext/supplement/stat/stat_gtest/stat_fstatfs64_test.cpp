#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/statfs.h>

using namespace testing::ext;

class StatFstatfs64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fstatfs64_001
 * @tc.desc: This test verifies whether the file system information was successfully obtained and the result was 0 when
 *           opening the/proc directory and calling the fstatfs64 function.
 * @tc.type: FUNC
 */
HWTEST_F(StatFstatfs64Test, fstatfs64_001, TestSize.Level1)
{
    int fileDescriptor = open("/proc", O_RDONLY);
    EXPECT_TRUE(fileDescriptor >= 0);
    struct statfs64 stat;
    int result = fstatfs64(fileDescriptor, &stat);
    EXPECT_EQ(0, result);
    close(fileDescriptor);
}