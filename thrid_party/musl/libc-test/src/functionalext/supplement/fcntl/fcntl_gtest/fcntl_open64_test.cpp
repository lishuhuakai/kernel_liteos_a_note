#include <fcntl.h>
#include <gtest/gtest.h>

using namespace testing::ext;

class FcntlOpen64Test : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: open64_001
 * @tc.desc: Verify whether the "/proc/version" file could be successfully opened through open and open64 functions.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlOpen64Test, open64_001, TestSize.Level1)
{
    int file = open("/proc/version", O_RDONLY);
    ASSERT_NE(file, -1);
    close(file);
    file = open64("/proc/version", O_RDONLY);
    ASSERT_NE(file, -1);
    close(file);
}

/**
 * @tc.name: open64_001
 * @tc.desc: Verify that the open64 interface has the same functionality and behavior as the open interface.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlOpen64Test, open64_002, TestSize.Level1)
{
    int file = open("/proc/version", O_WRONLY);
    ASSERT_NE(file, -1);
    close(file);
    file = open64("/proc/version", O_WRONLY);
    ASSERT_NE(file, -1);
    close(file);
}

/**
 * @tc.name: open64_001
 * @tc.desc: Verify that the open64 interface has the same functionality and behavior as the open interface when opening
 *           files in read-write mode.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlOpen64Test, open64_003, TestSize.Level1)
{
    int file = open("/proc/version", O_RDWR);
    ASSERT_NE(file, -1);
    close(file);
    file = open64("/proc/version", O_RDWR);
    ASSERT_NE(file, -1);
    close(file);
}