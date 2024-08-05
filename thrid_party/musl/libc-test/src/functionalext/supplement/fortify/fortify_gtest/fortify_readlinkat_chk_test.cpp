#include <fcntl.h>
#include <gtest/gtest.h>
#include <string.h>
#define __FORTIFY_COMPILATION
#include <fortify/unistd.h>

using namespace testing::ext;

class FortifyReadlinkchkTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: __readlinkat_chk_001
 * @tc.desc: Verify the correctness of the readlink function for symbolic link files in a specific path.
 * @tc.type: FUNC
 * */
HWTEST_F(FortifyReadlinkchkTest, __readlinkat_chk_001, TestSize.Level1)
{
    int fd = open("/data/readlinkat_chk", O_RDWR | O_CREAT, 0644);
    ASSERT_LE(0, fd);
    char buf[] = "test";
    write(fd, buf, sizeof(buf));
    close(fd);
    char linkpath[PATH_MAX] = "readlinkat_test";
    remove(linkpath);
    EXPECT_EQ(0, symlink("/data/readlinkat_chk", linkpath));
    char path[PATH_MAX];
    ssize_t length = __readlinkat_chk(AT_FDCWD, "readlinkat_test", path, sizeof(path), sizeof(path));
    ASSERT_LT(0, length);
    EXPECT_EQ("/data/readlinkat_chk", std::string(path, length));
    remove("/data/readlinkat_chk");
    remove("readlinkat_test");
}