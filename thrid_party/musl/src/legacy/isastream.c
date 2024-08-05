#include <stropts.h>
#include <fcntl.h>
#include <unsupported_api.h>

int isastream(int fd)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return fcntl(fd, F_GETFD) < 0 ? -1 : 0;
}
