#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include <stdlib.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include "musl_log.h"
#include "musl_fdtrack_hook.h"

static char *__fdtrack_hook_shared_lib = "libfdleak_tracker.so";

int FDTRACK_START_HOOK(int fd_value)
{
	int fd = fd_value;
	if (fd != -1 && __predict_false(__fdtrack_enabled) && __predict_false(__fdtrack_hook)) {
		struct fdtrack_event event;
		event.type = FDTRACK_EVENT_TYPE_CREATE;
		atomic_load(&__fdtrack_hook)(&event);
	}
	return fd;
}

__attribute__((constructor())) static void __musl_fdtrack_initialize()
{
	void* shared_library_handle = NULL;
	shared_library_handle = dlopen(__fdtrack_hook_shared_lib, RTLD_NOW | RTLD_LOCAL);
	if (shared_library_handle == NULL) {
		MUSL_LOGI("FdTrack, Unable to open shared library %s: %s.\n", __fdtrack_hook_shared_lib, dlerror());
		return;
	}
	MUSL_LOGI("FdTrack, load_fdtrack_hook_shared_library success.");
}

#endif
