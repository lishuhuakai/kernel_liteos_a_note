#include <sys/resource.h>
#include "syscall.h"

int getpriority(int which, id_t who)
{
#ifdef __LITEOS_A__
	return syscall(SYS_getpriority, which, who);
#else
	int ret = syscall(SYS_getpriority, which, who);
	if (ret < 0) return ret;
	return 20-ret;
#endif
}
