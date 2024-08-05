#include <sys/msg.h>
#include <unsupported_api.h>
#include "syscall.h"
#include "ipc.h"

int msgget(key_t k, int flag)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
#ifndef SYS_ipc
	return syscall(SYS_msgget, k, flag);
#else
	return syscall(SYS_ipc, IPCOP_msgget, k, flag);
#endif
}
