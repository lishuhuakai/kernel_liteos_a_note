#include <spawn.h>
#include <unsupported_api.h>

int posix_spawnattr_getpgroup(const posix_spawnattr_t *restrict attr, pid_t *restrict pgrp)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	*pgrp = attr->__pgrp;
	return 0;
}
