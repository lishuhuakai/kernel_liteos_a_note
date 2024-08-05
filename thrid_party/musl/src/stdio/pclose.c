#include "stdio_impl.h"
#include <errno.h>
#include <unistd.h>
#ifndef __LITEOS__
#include "param_check.h"
#endif

int pclose(FILE *f)
{
#ifndef __LITEOS__
	PARAM_CHECK(f);
#endif
	int status, r;
	pid_t pid = f->pipe_pid;
	fclose(f);
	while ((r=__sys_wait4(pid, &status, 0, 0)) == -EINTR);
	if (r<0) return __syscall_ret(r);
	return status;
}
