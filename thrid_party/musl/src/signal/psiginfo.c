#include <signal.h>
#include <unsupported_api.h>

void psiginfo(const siginfo_t *si, const char *msg)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	psignal(si->si_signo, msg);
}
