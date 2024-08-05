#include <bits/alltypes.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <unistd.h>
using namespace testing::ext;
constexpr int NUM = 9;
class UnistdDup2Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: dup2_001
 * @tc.desc: Verify the functionality of the dup2() function by duplicating a file descriptor onto another,
 *           performing I/O operations, and checking for successful duplication and I/O.
 * @tc.type: FUNC
 **/
HWTEST_F(UnistdDup2Test, dup2_001, TestSize.Level1)
{
    int fd = open("/dev/null", O_RDONLY);
    ASSERT_NE(-1, fd);
    FILE* fp = tmpfile();
    EXPECT_TRUE(fp != nullptr);
    setbuf(fp, nullptr);
    int result1 = fprintf(fp, "musl_test");
    EXPECT_EQ(NUM, result1);
    int result2 = dup2(fd, fileno(fp));
    EXPECT_NE(-1, result2);
    fclose(fp);
    close(fd);
}

/**
 * @tc.name: dup2_002
 * @tc.desc: Verify the error handling of the dup2() function when invalid file descriptors are provided as arguments.
 *           It checks if the appropriate error code (-1) is returned.
 * @tc.type: FUNC
 **/
HWTEST_F(UnistdDup2Test, dup2_002, TestSize.Level1)
{
    int result = dup2(-1, -1);
    EXPECT_EQ(-1, result);
}