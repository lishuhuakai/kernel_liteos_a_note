#include <ftw.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

using namespace testing::ext;

#define TEST_FD_LIMIT 128
#define TEST_FLAG_SIZE 4
#define TEST_DIGIT_TWO 2

class MiscNftw64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

static int NftwCallback(const char* pathName, const struct stat* sb, int flag, struct FTW* ftw)
{
    EXPECT_TRUE(pathName != nullptr);
    EXPECT_TRUE(sb != nullptr);
    if (flag == FTW_NS) {
        struct stat st;
        EXPECT_EQ(-1, stat(pathName, &st));
        return 0;
    }
    if (S_ISDIR(sb->st_mode)) {
        if (access(pathName, R_OK) == 0) {
            EXPECT_TRUE(flag == FTW_D || flag == FTW_DP);
        } else {
            EXPECT_EQ(flag, FTW_DNR);
        }
    } else if (S_ISLNK(sb->st_mode)) {
        EXPECT_EQ(flag, FTW_SL);
    } else {
        EXPECT_EQ(flag, FTW_F);
    }
    return 0;
}

static int CheckNftw64(const char* fpath, const struct stat64* sb, int tflag, FTW* ftwbuf)
{
    NftwCallback(fpath, reinterpret_cast<const struct stat*>(sb), tflag, ftwbuf);
    return 0;
}

static int NullCallback(const char* fpath, const struct stat64* sb, int, FTW* ftwbuf)
{
    return 0;
}

/**
 * @tc.name: nftw64_001
 * @tc.desc: Traverse directory /data.
 * @tc.type: FUNC
 * */
HWTEST_F(MiscNftw64Test, nftw64_001, TestSize.Level1)
{
    int flag[TEST_FLAG_SIZE] = { FTW_PHYS, FTW_MOUNT, FTW_CHDIR, FTW_DEPTH };
    const char* path = "/data";

    for (int i = 0; i < TEST_FLAG_SIZE; i++) {
        int ret = nftw64(path, CheckNftw64, TEST_FD_LIMIT, flag[i]);
        EXPECT_EQ(0, ret);
    }
}

/**
 * @tc.name: nftw64_002
 * @tc.desc: Verify if the nftw64 function handles non-existent paths as expected.
 * @tc.type: FUNC
 * */
HWTEST_F(MiscNftw64Test, nftw64_002, TestSize.Level1)
{
    errno = 0;
    int result = nftw64("/does/not/exist", NullCallback, TEST_FD_LIMIT, FTW_PHYS);
    EXPECT_EQ(-1, result);
    EXPECT_EQ(ENOENT, errno);
}

/**
 * @tc.name: nftw64_003
 * @tc.desc: Check if the behavior of the nftw64 function matches expectations when given an empty path.
 * @tc.type: FUNC
 * */
HWTEST_F(MiscNftw64Test, nftw64_003, TestSize.Level1)
{
    errno = 0;
    int result = nftw64("", NullCallback, TEST_FD_LIMIT, FTW_PHYS);
    EXPECT_EQ(-1, result);
    EXPECT_EQ(ENOENT, errno);
}