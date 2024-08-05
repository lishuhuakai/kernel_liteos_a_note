#include <unistd.h>
#include "syscall.h"
#include "libc.h"
#ifdef __LITEOS_A__
#include "errno.h"
#endif

int seteuid(uid_t euid)
{
#ifdef __LITEOS_A__
	if (euid == -1) {
		errno = EINVAL;
		return -1;
	}
#endif
	return __setxid(SYS_setresuid, -1, euid, -1);
}
