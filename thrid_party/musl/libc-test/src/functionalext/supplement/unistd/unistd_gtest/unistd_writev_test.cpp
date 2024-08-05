#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdWritevTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: writev_001
 * @tc.desc: Writes iovcnt buffers of data described by iov to the file associated with fd.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdWritevTest, writev_001, TestSize.Level1)
{
    const char* str0 = "test ";
    const char* str1 = "writev\n";
    struct iovec iov[2];

    iov[0].iov_base = reinterpret_cast<void*>(const_cast<char*>(str0));
    iov[0].iov_len = strlen(str0) + 1;
    iov[1].iov_base = reinterpret_cast<void*>(const_cast<char*>(str1));
    iov[1].iov_len = strlen(str1) + 1;

    EXPECT_EQ(iov[0].iov_len + iov[1].iov_len, writev(STDOUT_FILENO, iov, 2));
}