#include <spawn.h>
#include <unsupported_api.h>

int posix_spawnattr_setsigdefault(posix_spawnattr_t *restrict attr, const sigset_t *restrict def)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	attr->__def = *def;
	return 0;
}
