#include <fcntl.h>
#include <gtest/gtest.h>
#define _LARGEFILE64_SOURCE
#include <fortify/fcntl.h>

using namespace testing::ext;

class FortifyOpen64chkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __open64_chk_001
 * @tc.desc: Verify if the __open64_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyOpen64chkTest, __open64_chk_001, TestSize.Level1)
{
    int fd = __open64_chk("/proc/version", O_RDWR);
    EXPECT_NE(-1, fd);
    fd = open64("/proc/version", O_RDWR | O_CREAT, 0777);
    EXPECT_NE(-1, fd);
    close(fd);
}