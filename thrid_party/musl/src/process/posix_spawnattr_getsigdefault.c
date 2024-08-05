#include <spawn.h>
#include <unsupported_api.h>

int posix_spawnattr_getsigdefault(const posix_spawnattr_t *restrict attr, sigset_t *restrict def)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	*def = attr->__def;
	return 0;
}
