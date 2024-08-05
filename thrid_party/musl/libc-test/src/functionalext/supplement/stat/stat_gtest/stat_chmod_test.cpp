#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>
using namespace testing::ext;

class StatChmodTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: chmod_001
 * @tc.desc: Verify that the "chmod" function successfully changes the permission bits of a file without
 *           encountering any errors or exceptions.
 * @tc.type: FUNC
 **/
HWTEST_F(StatChmodTest, chmod_001, TestSize.Level1)
{
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int fd = open("test_chmod", O_RDWR | O_CREAT, 0755);
    EXPECT_GE(fd, 0);
    EXPECT_EQ(chmod("test_chmod", mode), 0);
    remove("test_chmod");
}

/**
 * @tc.name: chmod_002
 * @tc.desc: Assist in the verification and validation of software that relies on the "chmod" function for file
 *           permission management.
 * @tc.type: FUNC
 **/
HWTEST_F(StatChmodTest, chmod_002, TestSize.Level1)
{
    EXPECT_EQ(-1, chmod(nullptr, 0));
}