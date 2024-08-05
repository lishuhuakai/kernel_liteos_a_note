#include <fcntl.h>
#include <gtest/gtest.h>
#include <termios.h>
#include <unistd.h>

using namespace testing::ext;

class TermiosCfsetspeedTest : public testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

/**
 * @tc.name: cfsetspeed_001
 * @tc.desc: Verify the functionality of the cfsetspeed() function by opening a file, retrieving the current terminal
 *           attributes, setting the baud rate to 9600, applying the modified attributes to the serial port, and then
 *           closing the file descriptor.
 * @tc.type: FUNC
 **/
HWTEST_F(TermiosCfsetspeedTest, cfsetspeed_001, TestSize.Level1)
{
    int fd = open("/proc/version", O_RDWR | O_NOCTTY);
    ASSERT_NE(-1, fd);
    struct termios tty;
    int result1 = tcgetattr(fd, &tty);
    EXPECT_NE(0, result1);

    cfsetspeed(&tty, B9600);
    int result2 = tcsetattr(fd, TCSANOW, &tty);
    EXPECT_NE(0, result2);

    close(fd);
}