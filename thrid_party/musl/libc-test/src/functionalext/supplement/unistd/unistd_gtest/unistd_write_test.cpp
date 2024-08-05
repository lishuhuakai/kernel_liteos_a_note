#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdWriteTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr size_t BUF_SIZE = 2;

/**
 * @tc.name: write_001
 * @tc.desc: checks whether the write function behaves correctly when writing data to a file opened in write-only
 *           mode and when attempting to write data to a file opened in read-only mode.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdWriteTest, write_001, TestSize.Level1)
{
    char buf[1];
    int fd = open("/data/write.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    ASSERT_NE(-1, fd);
    EXPECT_EQ(BUF_SIZE, write(fd, buf, BUF_SIZE));
    close(fd);
    fd = open("/data/write.txt", O_RDONLY);
    ASSERT_NE(-1, fd);
    int ret = write(fd, buf, BUF_SIZE);
    EXPECT_EQ(-1, ret);
    close(fd);
    remove("/data/write.txt");
}