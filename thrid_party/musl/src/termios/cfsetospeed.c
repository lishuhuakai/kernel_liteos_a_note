#define _BSD_SOURCE
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unsupported_api.h>

int cfsetospeed(struct termios *tio, speed_t speed)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	if (speed & ~CBAUD) {
		errno = EINVAL;
		return -1;
	}
	tio->c_cflag &= ~CBAUD;
	tio->c_cflag |= speed;
	return 0;
}

int cfsetispeed(struct termios *tio, speed_t speed)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return speed ? cfsetospeed(tio, speed) : 0;
}

weak_alias(cfsetospeed, cfsetspeed);
