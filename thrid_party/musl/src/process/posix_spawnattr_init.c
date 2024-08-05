#include <spawn.h>
#include <unsupported_api.h>

int posix_spawnattr_init(posix_spawnattr_t *attr)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	*attr = (posix_spawnattr_t){ 0 };
	return 0;
}
