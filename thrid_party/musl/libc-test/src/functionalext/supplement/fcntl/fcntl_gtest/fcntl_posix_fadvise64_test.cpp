#include <fcntl.h>
#include <gtest/gtest.h>

using namespace testing::ext;

#define TEST_LEN 10

class FcntlPosixfadvise64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: posix_fadvise64_001
 * @tc.desc: Correct parameters, set file preload size.
 * @tc.type: FUNC
 * */
HWTEST_F(FcntlPosixfadvise64Test, posix_fadvise64_001, TestSize.Level1)
{
    FILE* fp = tmpfile();
    if (!fp) {
        return;
    }
    int fd = fileno(fp);
    if (fd == -1) {
        fclose(fp);
        return;
    }
    int result1 = posix_fadvise64(fd, 0, TEST_LEN, POSIX_FADV_NORMAL);
    int result2 = posix_fadvise64(fd, 0, TEST_LEN, POSIX_FADV_SEQUENTIAL);
    int result3 = posix_fadvise64(fd, 0, TEST_LEN, POSIX_FADV_RANDOM);
    int result4 = posix_fadvise64(fd, 0, TEST_LEN, POSIX_FADV_NOREUSE);
    int result5 = posix_fadvise64(fd, 0, TEST_LEN, POSIX_FADV_WILLNEED);
    int result6 = posix_fadvise64(fd, 0, TEST_LEN, POSIX_FADV_DONTNEED);
    EXPECT_EQ(0, result1);
    EXPECT_EQ(0, result2);
    EXPECT_EQ(0, result3);
    EXPECT_EQ(0, result4);
    EXPECT_EQ(0, result5);
    EXPECT_EQ(0, result6);
    fclose(fp);
}