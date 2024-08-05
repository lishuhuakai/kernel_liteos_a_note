#include <spawn.h>
#include <unsupported_api.h>

int posix_spawnattr_getsigmask(const posix_spawnattr_t *restrict attr, sigset_t *restrict mask)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	*mask = attr->__mask;
	return 0;
}
