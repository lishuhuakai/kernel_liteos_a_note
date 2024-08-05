#include <errno.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
using namespace testing::ext;

class StatStatTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: stat_001
 * @tc.desc: Aiming to confirm that the stat function can successfully retrieve information about the "test.txt" file
 *           and that no error occurs during this process.
 * @tc.type: FUNC
 **/
HWTEST_F(StatStatTest, stat_001, TestSize.Level1)
{
    const char* dirname = "test_stat";
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
    ASSERT_NE(-1, mkdir(dirname, mode));
    const char* path = "test_stat";
    struct stat fileStat;
    EXPECT_EQ(0, stat(path, &fileStat));
    remove("test_stat");
}

/**
 * @tc.name: stat_002
 * @tc.desc: Verify that the stat function correctly handles the case of a non-existent file and sets the
 *           appropriate error code.
 * @tc.type: FUNC
 **/
HWTEST_F(StatStatTest, stat_002, TestSize.Level1)
{
    const char* path = "not_exist.txt";
    struct stat fileStat;
    errno = 0;
    EXPECT_EQ(-1, stat(path, &fileStat));
    EXPECT_EQ(ENOENT, errno);
}