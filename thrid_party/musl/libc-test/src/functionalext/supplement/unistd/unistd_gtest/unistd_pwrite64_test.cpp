#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

constexpr size_t BUF_SIZE = 2;

class UnistdPwrite64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: pwrite64_001
 * @tc.desc: checks whether the pwrite64 function behaves correctly when writing data to a file opened in write-only
 *           mode and when attempting to write data to a file opened in read-only mode.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdPwrite64Test, pwrite64_001, TestSize.Level1)
{
    char buf[1];
    int fd = open("/data/pwrite64.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    ASSERT_NE(-1, fd);
    EXPECT_EQ(BUF_SIZE, pwrite64(fd, buf, BUF_SIZE, 0));
    close(fd);
    fd = open("/data/pwrite64.txt", O_RDONLY);
    ASSERT_NE(-1, fd);
    int ret = pwrite64(fd, buf, BUF_SIZE, 0);
    EXPECT_EQ(-1, ret);
    close(fd);
    remove("/data/pwrite64.txt");
}