#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

using namespace testing::ext;

class StatLstatTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: lstat_001
 * @tc.desc: Verify that the "lstat" function correctly retrieves file-related information such as file size,
 *           file permissions, timestamps, and other attributes associated with the specified file path.
 * @tc.type: FUNC
 **/
HWTEST_F(StatLstatTest, lstat_001, TestSize.Level1)
{
    const char* dirname = "test_lstat";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;

    ASSERT_NE(-1, mkdir(dirname, mode));

    const char* path = "test_lstat";
    struct stat fileStat;
    EXPECT_NE(-1, lstat(path, &fileStat));
    remove("test_lstat");
}

/**
 * @tc.name: lstat_002
 * @tc.desc: Verify the functionality of the lstat() function by checking if it correctly returns an error when
 *           attempting to retrieve information about a non-existing file.
 * @tc.type: FUNC
 **/
HWTEST_F(StatLstatTest, lstat_002, TestSize.Level1)
{
    const char* filename = "not_exist.txt";
    struct stat fileStat;
    EXPECT_EQ(-1, lstat(filename, &fileStat));
}