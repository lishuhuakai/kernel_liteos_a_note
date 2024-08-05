#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include <stdatomic.h>
#include "musl_fdtrack.h"

_Atomic(fdtrack_hook) __fdtrack_hook;
bool __fdtrack_enabled = false;

void set_fdtrack_enabled(bool newValue)
{
	__fdtrack_enabled = newValue;
}

bool fdtrack_cas_hook(fdtrack_hook* expected, fdtrack_hook value)
{
	return atomic_compare_exchange_strong(&__fdtrack_hook, expected, value);
}

#endif