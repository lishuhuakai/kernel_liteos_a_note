#include <gtest/gtest.h>
#include <sys/utsname.h>

using namespace testing::ext;

class MiscUnameTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: uname_001
 * @tc.desc: This test verifies that the uname function can successfully obtain operating system information, ensuring
 *           that the return value is 0, while asserting the various fields of the obtained operating system information
 *           to ensure that they are not empty or have a length of zero.
 * @tc.type: FUNC
 */
HWTEST_F(MiscUnameTest, uname_001, TestSize.Level1)
{
    struct utsname osInfo;
    int unameResult = uname(&osInfo);
    EXPECT_TRUE(unameResult == 0);
    EXPECT_EQ(0, strcmp("Linux", osInfo.sysname));
    EXPECT_TRUE(strcmp("", osInfo.machine) != 0);
    EXPECT_TRUE(strcmp("", osInfo.release) != 0);
    EXPECT_TRUE(strcmp("", osInfo.version) != 0);
    EXPECT_TRUE(strlen(osInfo.domainname) != 0);
}