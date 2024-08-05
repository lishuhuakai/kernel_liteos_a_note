#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

using namespace testing::ext;

class StatFstatTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fstat_001
 * @tc.desc: Verify that the "fstat" function correctly retrieves file-related information such as file size, file
 *           permissions, timestamps, and other attributes associated with the specified file descriptor.
 * @tc.type: FUNC
 **/
HWTEST_F(StatFstatTest, fstat_001, TestSize.Level1)
{
    int fd = open("test_fstat", O_RDWR | O_CREAT, 0755);
    EXPECT_GE(fd, 0);
    struct stat fileStat;
    EXPECT_EQ(0, fstat(fd, &fileStat));
    close(fd);
    remove("test_fstat");
}

/**
 * @tc.name: fstat_002
 * @tc.desc: Verify the error handling of the fstat() function when attempting to retrieve the status of a n
 *           on-existent file, and it checks if the appropriate error code (-1) is returned.
 * @tc.type: FUNC
 **/
HWTEST_F(StatFstatTest, fstat_002, TestSize.Level1)
{
    const char* filename = "not_exist.txt";
    int fd = open(filename, O_RDONLY);
    struct stat fileStat;
    EXPECT_EQ(-1, fstat(fd, &fileStat));
    close(fd);
}