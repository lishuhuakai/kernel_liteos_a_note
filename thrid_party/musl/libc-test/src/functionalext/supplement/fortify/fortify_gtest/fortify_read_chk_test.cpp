#include <fcntl.h>
#include <gtest/gtest.h>
#define __FORTIFY_COMPILATION
#include <fortify/unistd.h>

using namespace testing::ext;

constexpr size_t BUF_SIZE = 5;

class FortifyReadChkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __read_chk_001
 * @tc.desc: Verify that the file can be opened, data can be read using the __read_chk function, and the correctness of
 *           the read results can be validated.
 * @tc.type: FUNC
 */
HWTEST_F(FortifyReadChkTest, __read_chk_001, TestSize.Level1)
{
    int fd = open("/proc/version", O_RDONLY);
    ASSERT_NE(fd, -1);
    char buf[BUF_SIZE];
    ssize_t result = __read_chk(fd, buf, BUF_SIZE, sizeof(buf));
    EXPECT_EQ(result, BUF_SIZE);
    EXPECT_EQ(buf[0], 'L');
    EXPECT_EQ(buf[1], 'i');
    EXPECT_EQ(buf[2], 'n');
    EXPECT_EQ(buf[3], 'u');
    EXPECT_EQ(buf[4], 'x');
    close(fd);
}