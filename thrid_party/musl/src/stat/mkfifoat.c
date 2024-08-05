#include <sys/stat.h>
#include <unsupported_api.h>

int mkfifoat(int fd, const char *path, mode_t mode)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return mknodat(fd, path, mode | S_IFIFO, 0);
}
