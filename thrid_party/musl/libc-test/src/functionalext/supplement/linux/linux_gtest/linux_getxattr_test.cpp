#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <sys/xattr.h>

using namespace testing::ext;

class LinuxGetxattrTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr int SIZE = 4;

/**
 * @tc.name: getxattr_001
 * @tc.desc: This test verifies whether the getxattr function can correctly return -1 when passing invalid parameters,
 *           that is, whether it can handle invalid attribute query requests correctly.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxGetxattrTest, getxattr_001, TestSize.Level1)
{
    int result = getxattr(nullptr, nullptr, nullptr, -1);
    EXPECT_EQ(result, -1);
}

/**
 * @tc.name: getxattr_002
 * @tc.desc: This test tests whether the getxattr function can correctly obtain the extension properties of a file.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxGetxattrTest, getxattr_002, TestSize.Level1)
{
    const char* filePath = "getxattr.txt";
    int fileDescriptor = open(filePath, O_RDWR | O_CREAT, 0666);
    char data[] = "musl";
    write(fileDescriptor, data, sizeof(data));
    close(fileDescriptor);
    const char* attrName = "user.test";
    const char* attrValue = "musl";
    int setResult = setxattr(filePath, attrName, attrValue, strlen(attrValue), XATTR_CREATE);
    EXPECT_EQ(setResult, 0);
    char buffer[BUFSIZ] = { 0 };
    int getResult = getxattr(filePath, attrName, buffer, sizeof(buffer));
    EXPECT_EQ(getResult, SIZE);
    EXPECT_STREQ(buffer, data);
    int removeResult = remove(filePath);
    EXPECT_EQ(removeResult, 0);
}