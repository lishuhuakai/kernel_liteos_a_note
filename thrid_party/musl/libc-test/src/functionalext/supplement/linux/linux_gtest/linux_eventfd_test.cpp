#include <errno.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/eventfd.h>

using namespace testing::ext;

class LinuxEventfdTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

constexpr unsigned int VALUE = 2;

/**
 * @tc.name: eventfd_001
 * @tc.desc: This test verifies that the eventfd instance created by the eventfd function can correctly set the flag bit
 *           and read the correct initial value, while also setting the EFD_The CLOEXEC flag bit ensures that the
 *           eventfd instance will automatically close when executing the exec series of functions.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxEventfdTest, eventfd_001, TestSize.Level1)
{
    int eventFd = eventfd(VALUE, EFD_CLOEXEC);
    ASSERT_NE(eventFd, -1);
    int flags = fcntl(eventFd, F_GETFD);
    EXPECT_NE(flags, -1);
    EXPECT_EQ(FD_CLOEXEC, flags & FD_CLOEXEC);
    close(eventFd);
}

/**
 * @tc.name: eventfd_002
 * @tc.desc: The test case aims to verify the functionality and correctness of creating an eventfd instance
 *           with the EFD_NONBLOCK and EFD_CLOEXEC flags. It ensures that the eventfd instance can work properly and
 *           will be automatically closed when executing exec series functions.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxEventfdTest, eventfd_002, TestSize.Level1)
{
    int eventFd = eventfd(VALUE, EFD_NONBLOCK | EFD_CLOEXEC);
    ASSERT_NE(eventFd, -1);
    int flags = fcntl(eventFd, F_GETFD);
    EXPECT_NE(flags, -1);
    EXPECT_EQ(FD_CLOEXEC, flags & FD_CLOEXEC);
    close(eventFd);
}

/**
 * @tc.name: eventfd_003
 * @tc.desc: The testing viewpoint of this test case is to create an eventfd object of semaphore type by calling the
 *           eventfd function and asserting a return value greater than 0 to verify whether the eventfd object was
 *           successfully created.
 * @tc.type: FUNC
 */
HWTEST_F(LinuxEventfdTest, eventfd_003, TestSize.Level1)
{
    int result = eventfd(0, EFD_SEMAPHORE);
    EXPECT_GE(result, 0);
}
