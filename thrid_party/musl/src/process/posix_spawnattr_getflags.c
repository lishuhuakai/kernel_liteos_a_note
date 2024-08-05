#include <spawn.h>
#include <unsupported_api.h>

int posix_spawnattr_getflags(const posix_spawnattr_t *restrict attr, short *restrict flags)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	*flags = attr->__flags;
	return 0;
}
