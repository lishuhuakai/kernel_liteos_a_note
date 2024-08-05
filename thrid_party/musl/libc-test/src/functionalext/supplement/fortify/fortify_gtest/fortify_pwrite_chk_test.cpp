#include <fcntl.h>
#include <gtest/gtest.h>
#include <string.h>
#define __FORTIFY_COMPILATION
#include <fortify/unistd.h>

constexpr int BUF_SIZE = 2;

using namespace testing::ext;

class FortifyPwritechkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __pwrite_chk_001
 * @tc.desc: Check whether the pwrite64 function behaves correctly when writing data to a file opened in write-only
 *           mode and when attempting to write data to a file opened in read-only mode.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyPwritechkTest, __pwrite_chk_001, TestSize.Level1)
{
    char buf[1];
    int fd = open("/data/pwrite_chk.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    EXPECT_EQ(BUF_SIZE, __pwrite_chk(fd, buf, BUF_SIZE, 0, BUF_SIZE));
    fd = open("/data/pwrite_chk.txt", O_RDONLY);
    int ret = __pwrite_chk(fd, buf, BUF_SIZE, 0, BUF_SIZE);
    EXPECT_EQ(-1, ret);
    close(fd);
    remove("/data/pwrite_chk.txt");
}
