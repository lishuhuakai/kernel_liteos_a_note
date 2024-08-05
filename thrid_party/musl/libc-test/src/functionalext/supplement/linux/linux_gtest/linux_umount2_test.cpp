#include <errno.h>
#include <gtest/gtest.h>
#include <sys/mount.h>
using namespace testing::ext;

class LinuxUmount2Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: umount2_001
 * @tc.desc: Validate the behavior of the umount2() function under specific conditions, including error handling
 *           and the functionality of forced unmounting.
 * @tc.type: FUNC
 **/
HWTEST_F(LinuxUmount2Test, umount2_001, TestSize.Level1)
{
    const char* target = nullptr;
    errno = 0;
    int result = umount2(target, MNT_FORCE);
    EXPECT_EQ(-1, result);
    EXPECT_EQ(EFAULT, errno);
}