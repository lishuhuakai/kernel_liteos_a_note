#include <spawn.h>
#include <sched.h>
#include <errno.h>
#include <unsupported_api.h>

int posix_spawnattr_getschedparam(const posix_spawnattr_t *restrict attr,
	struct sched_param *restrict schedparam)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return ENOSYS;
}

int posix_spawnattr_setschedparam(posix_spawnattr_t *restrict attr,
	const struct sched_param *restrict schedparam)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return ENOSYS;
}

int posix_spawnattr_getschedpolicy(const posix_spawnattr_t *restrict attr, int *restrict policy)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return ENOSYS;
}

int posix_spawnattr_setschedpolicy(posix_spawnattr_t *attr, int policy)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return ENOSYS;
}
