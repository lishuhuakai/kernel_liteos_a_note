#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

constexpr size_t N_BITES = 2;

class UnistdPread64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: pread64_001
 * @tc.desc: checks whether the pread64 function behaves correctly when reading data from a file opened in read-only
 *           mode and when attempting to read data from a file opened in write-only mode.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdPread64Test, pread64_001, TestSize.Level1)
{
    char buf[1];
    int fd = open("/dev/null", O_RDONLY);
    ASSERT_NE(-1, fd);
    EXPECT_EQ(0, pread64(fd, buf, N_BITES, 0));
    close(fd);
    fd = open("/dev/null", O_WRONLY);
    ASSERT_NE(-1, fd);
    EXPECT_EQ(-1, pread64(fd, buf, N_BITES, 0));
    close(fd);
}