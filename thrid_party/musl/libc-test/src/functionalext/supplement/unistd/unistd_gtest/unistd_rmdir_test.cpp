#include <fcntl.h>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <sys/auxv.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace testing::ext;

class UnistdRmdirTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: rmdir_001
 * @tc.desc: Test that the function is functioning properly.
 * @tc.type: FUNC
 * */
HWTEST_F(UnistdRmdirTest, rmdir_001, TestSize.Level1)
{
    const char* path = "/data/testrmdir";
    if (access(path, F_OK) != 0) {
        EXPECT_EQ(0, mkdir(path, 0777));
        EXPECT_EQ(0, rmdir(path));
    } else {
        remove(path);
        EXPECT_EQ(0, mkdir(path, 0777));
        EXPECT_EQ(0, rmdir(path));
    }
    path = nullptr;
}