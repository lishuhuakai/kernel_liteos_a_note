#define _GNU_SOURCE
#include <unistd.h>
#include "syscall.h"
#include "pthread_impl.h"

#define ZERO (0)
#define NEGATIVE_ONE (-1)

pid_t gettid(void)
{
	pid_t tid = __pthread_self()->tid;
	if (tid == ZERO || tid == NEGATIVE_ONE) {
		return __syscall(SYS_gettid);
	}
	return tid;
}
