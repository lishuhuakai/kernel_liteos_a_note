#include <fcntl.h>
#include <gtest/gtest.h>
#define __FORTIFY_COMPILATION
#include <fortify/unistd.h>

using namespace testing::ext;

constexpr int BUF_SIZE = 256;
constexpr size_t COUNT = 100;

class FortifyPreadChkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __pread_chk_001
 * @tc.desc: Verify that the file can be opened, data can be pre-read using the __pread_chk function, and the file can
 *           be closed properly.
 * @tc.type: FUNC
 */
HWTEST_F(FortifyPreadChkTest, __pread_chk_001, TestSize.Level1)
{
    char buf[BUF_SIZE];
    off_t offset = 0;
    int file = open("test.txt", O_RDONLY | O_CREAT, 0644);
    ASSERT_NE(file, -1);
    size_t bufSize = sizeof(buf);
    ssize_t result = __pread_chk(file, buf, COUNT, offset, bufSize);
    close(file);
    EXPECT_GE(result, 0);
    EXPECT_EQ(0, remove("test.txt"));
}