#include <errno.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <sys/xattr.h>

using namespace testing::ext;

class LinuxSetxattrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: setxattr_001
 * @tc.desc: This test verifies whether the setxattr function can handle invalid parameters correctly and whether the
 *           errno value can be set correctly.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxSetxattrTest, setxattr_001, TestSize.Level1)
{
    errno = 0;
    int result = setxattr(nullptr, nullptr, nullptr, -1, -1);
    EXPECT_NE(result, 0);
    EXPECT_TRUE(errno == EFAULT);
}

/**
 * @tc.name: setxattr_002
 * @tc.desc: This test verifies whether the setxattr function can handle invalid parameters correctly and whether the
 *           errno value can be set correctly.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxSetxattrTest, setxattr_002, TestSize.Level1)
{
    const char* filePath = "setxattr.txt";
    int fileDescriptor = open(filePath, O_RDWR | O_CREAT, 0666);
    char data[] = "musl";
    write(fileDescriptor, data, strlen(data));
    close(fileDescriptor);
    const char* attrName = "user.test";
    const char* attrValue = "new_data";
    int setResult = setxattr(filePath, attrName, attrValue, strlen(attrValue), XATTR_CREATE);
    EXPECT_EQ(setResult, 0);
    int removeResult = remove(filePath);
    EXPECT_TRUE(removeResult == 0);
}