#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

using namespace testing::ext;

class StatFstatatTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fstatat_001
 * @tc.desc: Verify that the fstatat function successfully retrieves file status information, including file mode
 *           permissions, when used with appropriate parameters.
 * @tc.type: FUNC
 **/
HWTEST_F(StatFstatatTest, fstatat_001, TestSize.Level1)
{
    const char* dirname = "test_fstatat";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    ASSERT_NE(-1, mkdir(dirname, mode));

    struct stat st;
    const char* linkname = "test_fstatat";

    EXPECT_EQ(0, fstatat(AT_FDCWD, linkname, &st, AT_NO_AUTOMOUNT));
    mode_t mask = S_IRWXU | S_IRWXG | S_IRWXO;
    EXPECT_EQ(0755 & mask, static_cast<mode_t>(st.st_mode) & mask);

    remove("test_fstatat");
}

/**
 * @tc.name: fstatat_002
 * @tc.desc: Test whether the return value of the fstatat() function is 0, and check if the obtained file or
 *           directory permissions match the expected value.
 * @tc.type: FUNC
 **/
HWTEST_F(StatFstatatTest, fstatat_002, TestSize.Level1)
{
    const char* dirname = "test_fstatat";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    EXPECT_NE(-1, mkdir(dirname, mode));

    struct stat st;
    const char* linkname = "test_fstatat";

    EXPECT_EQ(0, fstatat(AT_FDCWD, linkname, &st, AT_SYMLINK_NOFOLLOW));
    mode_t mask = S_IRWXU | S_IRWXG | S_IRWXO;
    EXPECT_EQ(0755 & mask, static_cast<mode_t>(st.st_mode) & mask);
    remove("test_fstatat");
}