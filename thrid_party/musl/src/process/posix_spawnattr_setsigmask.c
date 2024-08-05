#include <spawn.h>
#include <unsupported_api.h>

int posix_spawnattr_setsigmask(posix_spawnattr_t *restrict attr, const sigset_t *restrict mask)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	attr->__mask = *mask;
	return 0;
}
