#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

#define TEST_LEN 10

using namespace testing::ext;

class FcntlPosixfallocate64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fcntl_posix_fallocate64_001
 * @tc.desc: Verify whether the specified size of space can be successfully allocated when using the fcntl function for
 *           file allocation, and determine whether the allocation result is correct through assertion.
 * @tc.type: FUNC
 * */
HWTEST_F(FcntlPosixfallocate64Test, fcntl_posix_fallocate64_001, TestSize.Level1)
{
    struct stat sb;
    FILE* fp = tmpfile();
    if (!fp) {
        return;
    }
    int fd = fileno(fp);
    if (fd == -1) {
        fclose(fp);
        return;
    }
    int result = posix_fallocate64(fd, 0, TEST_LEN);
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, fstat(fd, &sb));
    EXPECT_EQ(sb.st_size, TEST_LEN);
    fclose(fp);
}