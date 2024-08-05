#include <errno.h>
#include "pthread_impl.h"
#include "prt_sys_external.h"
#include "prt_task_external.h"
static int _isr_errno;

int *__errno_location(void)
{
	if (OS_INT_INACTIVE && (RUNNING_TASK != NULL)) {
		return &(RUNNING_TASK->lastErr);
	} else {
		return &_isr_errno;
	}
}

weak_alias(__errno_location, ___errno_location);
