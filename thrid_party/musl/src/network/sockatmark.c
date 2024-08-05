#include <sys/socket.h>
#include <sys/ioctl.h>
#include <unsupported_api.h>

int sockatmark(int s)
{
	int ret;
	UNSUPPORTED_API_VOID(LITEOS_A);
	if (ioctl(s, SIOCATMARK, &ret) < 0)
		return -1;
	return ret;
}
