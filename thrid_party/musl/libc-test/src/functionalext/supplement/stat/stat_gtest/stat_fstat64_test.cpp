#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

using namespace testing::ext;

class StatFstat64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fstat64_001
 * @tc.desc: Verify the functionality of the fstat64() function by creating a directory, opening a file within that
 *           directory, retrieving the file status information using fstat64(), and performing checks to ensure
 *           successful execution of each step.
 * @tc.type: FUNC
 **/
HWTEST_F(StatFstat64Test, fstat64_001, TestSize.Level1)
{
    const char* dirName = "test_fstat64.txt";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int result1 = mkdir(dirName, mode);
    ASSERT_NE(-1, result1);

    int fd = open("test_fstat64.txt", O_RDONLY);
    ASSERT_NE(-1, fd);
    struct stat64 statBuf;
    int result2 = fstat64(fd, &statBuf);
    EXPECT_NE(-1, result2);
    close(fd);
    remove("test_fstat64.txt");
}