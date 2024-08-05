#include <sys/msg.h>
#include <unsupported_api.h>
#include "syscall.h"
#include "ipc.h"

ssize_t msgrcv(int q, void *m, size_t len, long type, int flag)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
#ifndef SYS_ipc
	return syscall_cp(SYS_msgrcv, q, m, len, type, flag);
#else
	return syscall_cp(SYS_ipc, IPCOP_msgrcv, q, len, flag, ((long[]){ (long)m, type }));
#endif
}
