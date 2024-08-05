#include <errno.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/vfs.h>
#include <unistd.h>

using namespace testing::ext;

class FcntlFcntlTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: fcntl_001
 * @tc.desc: Verify the correctness of the fcntl interface in retrieving file descriptor flags and checks if the file
 *           descriptor has the FD_CLOEXEC flag set.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_001, TestSize.Level1)
{
    int fd = open("test_fcntl_001.txt", O_RDONLY | O_CREAT, 0644);
    ASSERT_NE(fd, -1);
    int fdFlags = fcntl(fd, F_GETFD);
    ASSERT_NE(fdFlags, -1);
    EXPECT_EQ(fdFlags & FD_CLOEXEC, 0);
    close(fd);
    EXPECT_EQ(0, remove("test_fcntl_001.txt"));
}

/**
 * @tc.name: fcntl_002
 * @tc.desc: Verify that the fcntl interface, when used with the F_DUPFD parameter, correctly creates a new file
 *           descriptor that points to the same file table entry as the original file descriptor.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_002, TestSize.Level1)
{
    int fd = open("test_fcntl_002.txt", O_RDONLY | O_CREAT, 0644);
    ASSERT_NE(fd, -1);
    int newFd = fcntl(fd, F_DUPFD, 0);
    EXPECT_GE(newFd, 0);
    close(fd);
    close(newFd);
    EXPECT_EQ(0, remove("test_fcntl_002.txt"));
}

/**
 * @tc.name: fcntl_003
 * @tc.desc: Verify that the fcntl interface, when used with the F_GETLK parameter, retrieves the lock information of
 *           the file associated with the file descriptor, and the expected result is that the file is not locked.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_003, TestSize.Level1)
{
    int fd = open("test_fcntl_003.txt", O_RDWR | O_CREAT, 0644);
    ASSERT_NE(fd, -1);
    struct flock fileLock;
    fileLock.l_type = F_WRLCK;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_start = 0;
    fileLock.l_len = 0;
    int lockResult = fcntl(fd, F_GETLK, &fileLock);
    ASSERT_NE(lockResult, -1);
    close(fd);
    EXPECT_EQ(0, remove("test_fcntl_003.txt"));
}

/**
 * @tc.name: fcntl_004
 * @tc.desc: Verify that the fcntl interface, when used with the F_GETLK64 parameter, retrieves the lock information of
 *           the file associated with the file descriptor, and the expected result is that the file is not locked.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_004, TestSize.Level1)
{
    int fd = open("test_fcntl_004.txt", O_RDWR | O_CREAT, 0644);
    ASSERT_NE(fd, -1);
    struct flock64 fileLock;
    fileLock.l_type = F_WRLCK;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_start = 0;
    fileLock.l_len = 0;
    int lockResult = fcntl(fd, F_GETLK64, &fileLock);
    ASSERT_NE(lockResult, -1);
    close(fd);
    EXPECT_EQ(0, remove("test_fcntl_004.txt"));
}

/**
 * @tc.name: fcntl_005
 * @tc.desc: Verify that the fcntl interface, when used with the F_GETFL parameter, retrieves the flag information of
 *           the file descriptor, and the expected result is that the file descriptor does not have the O_APPEND flag
 *           set, but has the O_RDWR flag set.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_005, TestSize.Level1)
{
    int fd = open("test_fcntl_005.txt", O_RDWR | O_CREAT, 0666);
    ASSERT_NE(-1, fd);
    int fdFlags = fcntl(fd, F_GETFL);
    ASSERT_NE(fdFlags, -1);
    EXPECT_FALSE(fdFlags & O_APPEND);
    EXPECT_EQ(fdFlags & O_RDWR, O_RDWR);
    close(fd);
    EXPECT_EQ(0, remove("test_fcntl_005.txt"));
}

/**
 * @tc.name: fcntl_006
 * @tc.desc: Verify that the fcntl interface, when used with the F_SETOWN and F_GETOWN parameters, can set and retrieve
 *           the process ownership of the file descriptor, and the expected result is that the set process ID matches
 *           the retrieved process ID.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_006, TestSize.Level1)
{
    int fd = open("test_fcntl_006.txt", O_RDWR | O_CREAT, 0666);
    ASSERT_NE(-1, fd);
    pid_t pid = getpid();
    int setPidResult = fcntl(fd, F_SETOWN, pid);
    ASSERT_NE(setPidResult, -1);
    int id = fcntl(fd, F_GETOWN);
    ASSERT_NE(id, -1);
    EXPECT_EQ(pid, id);
    close(fd);
    EXPECT_EQ(0, remove("test_fcntl_006.txt"));
}

/**
 * @tc.name: fcntl_007
 * @tc.desc: Verify that the fcntl interface, when used with the F_SETFD and F_GETFD parameters, can set and retrieve
 *           the close-on-exec flag of the file descriptor, and the expected result is that the set flag matches the
 *           retrieved flag.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_007, TestSize.Level1)
{
    int fd = open("test_fcntl_007.txt", O_CREAT | O_RDWR, S_IRWXU);
    ASSERT_NE(-1, fd);
    int setFlagResult = fcntl(fd, F_SETFD, FD_CLOEXEC);
    ASSERT_NE(setFlagResult, -1);
    int getFlagResult = fcntl(fd, F_GETFD);
    ASSERT_NE(getFlagResult, -1);
    EXPECT_EQ(getFlagResult & FD_CLOEXEC, FD_CLOEXEC);
    close(fd);
    EXPECT_EQ(0, remove("test_fcntl_007.txt"));
}

/**
 * @tc.name: fcntl_008
 * @tc.desc: Verify that the fcntl interface, when used with the F_GETFL and F_SETFL parameters, can retrieve and set
 *           the flag information of the file descriptor, and the expected result is to add the O_NONBLOCK flag on top
 *           of the original flags.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_008, TestSize.Level1)
{
    int fd = open("test_fcntl_008.txt", O_RDONLY | O_CREAT, 0644);
    ASSERT_NE(fd, -1);
    int fdFlags = fcntl(fd, F_GETFL);
    ASSERT_NE(fdFlags, -1);
    EXPECT_EQ(fdFlags & O_NONBLOCK, 0);
    int newFlags = fdFlags | O_NONBLOCK;
    int setResult = fcntl(fd, F_SETFL, newFlags);
    ASSERT_NE(setResult, -1);
    int updatedFlags = fcntl(fd, F_GETFL);
    ASSERT_NE(updatedFlags, -1);
    EXPECT_EQ(updatedFlags, newFlags);
    close(fd);
    EXPECT_EQ(0, remove("test_fcntl_008.txt"));
}

/**
 * @tc.name: fcntl_009
 * @tc.desc: Verify that the fcntl interface, when used with the F_SETLK parameter, can lock the file descriptor for
 *           writing, and the expected result is a successful setting of the write lock.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_009, TestSize.Level1)
{
    int fd = open("test_fcntl_009.txt", O_RDWR | O_CREAT, 0644);
    ASSERT_NE(fd, -1);
    struct flock fileLock;
    fileLock.l_type = F_WRLCK;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_start = 0;
    fileLock.l_len = 0;
    int setResult = fcntl(fd, F_SETLK, &fileLock);
    ASSERT_NE(setResult, -1);
    close(fd);
    EXPECT_EQ(0, remove("test_fcntl_009.txt"));
}

/**
 * @tc.name: fcntl_010
 * @tc.desc: Verify that the fcntl interface, when used with the F_SETLKW parameter, can lock the file descriptor for
 *           writing and wait until the lock is released, and the expected result is a successful setting and waiting
 *           for the write lock.
 * @tc.type: FUNC
 */
HWTEST_F(FcntlFcntlTest, fcntl_010, TestSize.Level1)
{
    int fd = open("test_fcntl_010.txt", O_RDWR | O_CREAT, 0644);
    ASSERT_NE(fd, -1);
    struct flock fileLock;
    fileLock.l_type = F_WRLCK;
    fileLock.l_whence = SEEK_SET;
    fileLock.l_start = 0;
    fileLock.l_len = 0;
    int setResult = fcntl(fd, F_SETLKW, &fileLock);
    ASSERT_NE(setResult, -1);
    close(fd);
    EXPECT_EQ(0, remove("test_fcntl_010.txt"));
}
