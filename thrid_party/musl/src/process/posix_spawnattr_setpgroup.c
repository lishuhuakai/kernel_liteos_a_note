#include <spawn.h>
#include <unsupported_api.h>

int posix_spawnattr_setpgroup(posix_spawnattr_t *attr, pid_t pgrp)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	attr->__pgrp = pgrp;
	return 0;
}
