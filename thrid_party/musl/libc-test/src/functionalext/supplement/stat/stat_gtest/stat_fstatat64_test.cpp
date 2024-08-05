#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

using namespace testing::ext;

class StatFstatatTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fstatat64_001
 * @tc.desc: Verify the functionality of the fstatat64() function by creating a directory, retrieving the file status
 *           information using fstatat64() with the AT_SYMLINK_NOFOLLOW flag to ensure that any symbolic links in the
 *           path are not followed, comparing the file permissions with the expected value, and performing checks to
 *           ensure successful execution of each step.
 * @tc.type: FUNC
 **/
HWTEST_F(StatFstatatTest, fstatat64_001, TestSize.Level1)
{
    const char* dirName = "test_fstatat64.txt";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int result = mkdir(dirName, mode);
    ASSERT_NE(-1, result);
    struct stat64 st;
    const char* linkName = "test_fstatat64.txt";
    EXPECT_EQ(0, fstatat64(AT_FDCWD, linkName, &st, AT_NO_AUTOMOUNT));
    mode_t mask = S_IRWXU | S_IRWXG | S_IRWXO;
    EXPECT_EQ(0755 & mask, static_cast<mode_t>(st.st_mode) & mask);
    remove("test_fstatat64.txt");
}

/**
 * @tc.name: fstatat64_002
 * @tc.desc: Verify the functionality of the fstatat64() function by creating a directory, retrieving the file status
 *           information using fstatat64() with the AT_SYMLINK_NOFOLLOW flag to ensure that any symbolic links in the
 *           path are not followed, comparing the file permissions with the expected value, and performing checks to
 *           ensure successful execution of each step.
 * @tc.type: FUNC
 **/
HWTEST_F(StatFstatatTest, fstatat64_002, TestSize.Level1)
{
    const char* dirName = "test_fstatat64.txt";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int result = mkdir(dirName, mode);
    ASSERT_NE(-1, result);
    struct stat64 st;
    const char* linkName = "test_fstatat64.txt";
    int ret = fstatat64(AT_FDCWD, linkName, &st, AT_SYMLINK_NOFOLLOW);
    EXPECT_EQ(0, ret);
    mode_t mask = S_IRWXU | S_IRWXG | S_IRWXO;
    EXPECT_EQ(0755 & mask, static_cast<mode_t>(st.st_mode) & mask);
    remove("test_fstatat64.txt");
}

/**
 * @tc.name: fstatat64_003
 * @tc.desc: Verify the functionality of the fstatat64() function by creating a directory, retrieving the file status
 *           information using fstatat64() with the AT_EMPTY_PATH flag to specify that the path refers to the current
 *           directory, comparing the file permissions with the expected value, and performing checks to ensure
 *           successful execution of each step.
 * @tc.type: FUNC
 **/
HWTEST_F(StatFstatatTest, fstatat64_003, TestSize.Level1)
{
    const char* dirName = "test_fstatat64.txt";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    int result = mkdir(dirName, mode);
    ASSERT_NE(-1, result);
    struct stat64 st;
    const char* linkName = "test_fstatat64.txt";
    int ret = fstatat64(AT_FDCWD, linkName, &st, AT_EMPTY_PATH);
    EXPECT_EQ(0, ret);
    mode_t mask = S_IRWXU | S_IRWXG | S_IRWXO;
    EXPECT_EQ(0755 & mask, static_cast<mode_t>(st.st_mode) & mask);
    remove("test_fstatat64.txt");
}