#include <unistd.h>
#include "libc.h"
#include "syscall.h"
#ifdef __LITEOS_A__
#include "errno.h"
#endif

int setegid(gid_t egid)
{
#ifdef __LITEOS_A__
	if (egid == -1) {
		errno = EINVAL;
		return -1;
	}
#endif
	return __setxid(SYS_setresgid, -1, egid, -1);
}
