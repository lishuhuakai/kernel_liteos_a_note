#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
#define _LARGEFILE64_SOURCE
#include <fortify/fcntl.h>

using namespace testing::ext;

class FortifyOpenatchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __openat_chk_001
 * @tc.desc: Verify if the __openat_chk is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyOpenatchkTest, __openat_chk_001, TestSize.Level1)
{
    int result1 = __openat_chk(0, "/dev/null", O_WRONLY);
    int result2 = __openat_chk(0, "/dev/null", O_RDONLY);
    int result3 = __openat_chk(0, "openat_test", O_WRONLY);
    int result4 = __openat_chk(0, "openat_test", O_RDONLY);
    EXPECT_LE(0, result1);
    EXPECT_LE(0, result2);
    EXPECT_EQ(-1, result3);
    EXPECT_EQ(-1, result4);
}