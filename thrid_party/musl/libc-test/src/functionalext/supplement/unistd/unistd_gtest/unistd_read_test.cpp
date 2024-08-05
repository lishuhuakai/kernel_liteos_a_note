#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

constexpr size_t N_BITES = 2;
constexpr size_t READ_COUNT = 5;
constexpr size_t READ_SIZE = 6;

class UnistdReadTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: read_001
 * @tc.desc: checks whether the read function behaves correctly when reading data from a file opened in read-only
 *           mode and when attempting to read data from a file opened in write-only mode.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdReadTest, read_001, TestSize.Level1)
{
    char buf[1];
    int fd = open("/dev/null", O_RDONLY);
    ASSERT_NE(-1, fd);
    EXPECT_EQ(0, read(fd, buf, N_BITES));
    close(fd);
    fd = open("/dev/null", O_WRONLY);
    ASSERT_NE(-1, fd);
    EXPECT_EQ(-1, read(fd, buf, N_BITES));
    close(fd);
}

/**
 * @tc.name: read_002
 * @tc.desc: Verify whether the read content matches the actual situation.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdReadTest, read_002, TestSize.Level1)
{
    int fd = open("/proc/version", O_RDONLY);
    ASSERT_NE(-1, fd);
    char buf[READ_SIZE];
    EXPECT_EQ(READ_COUNT, read(fd, buf, READ_COUNT));
    EXPECT_STREQ("Linux", buf);
    close(fd);
}

/**
 * @tc.name: read_003
 * @tc.desc: Validation function error returned
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdReadTest, read_003, TestSize.Level1)
{
    char buf[1];
    EXPECT_EQ(-1, read(-1, buf, sizeof(buf)));
}