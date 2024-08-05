#include <mqueue.h>
#include "syscall.h"

int mq_close(mqd_t mqd)
{
#ifdef __LITEOS_A__
	return syscall(SYS_mqclose, mqd);
#else
	return syscall(SYS_close, mqd);
#endif
}
