#include <fcntl.h>
#include <gtest/gtest.h>
#define __FORTIFY_COMPILATION
#include <fortify/unistd.h>

using namespace testing::ext;

class FortifyWriteChkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __write_chk_001
 * @tc.desc: Verify the basic functionality of writing data to a file using the __write_chk function, including creating
 *           a temporary file, writing data, and validating the correctness of the write results.
 * @tc.type: FUNC
 */
HWTEST_F(FortifyWriteChkTest, __write_chk_001, TestSize.Level1)
{
    const char* data = "Hello, this is a test.";
    size_t count = strlen(data);
    int fd = open("test.txt", O_RDWR | O_CREAT, 0644);
    ASSERT_NE(fd, -1);
    ssize_t result = __write_chk(fd, data, count, count);
    EXPECT_EQ(result, count);
    close(fd);
    EXPECT_EQ(0, remove("test.txt"));
}