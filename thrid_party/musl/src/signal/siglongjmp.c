#include <setjmp.h>
#include <signal.h>
#include "syscall.h"
#include "pthread_impl.h"
#include <unsupported_api.h>

_Noreturn void siglongjmp(sigjmp_buf buf, int ret)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	longjmp(buf, ret);
}
