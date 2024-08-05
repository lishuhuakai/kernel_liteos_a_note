/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
		event.fd = fd;
		atomic_load(&__fdtrack_hook)(&event);
	}
	return fd;
}

__attribute__((constructor())) static void __musl_fdtrack_initialize()
{
	if (!check_beta_develop_before()) {
		return;
	}
	void* shared_library_handle = NULL;
	shared_library_handle = dlopen(__fdtrack_hook_shared_lib, RTLD_NOW | RTLD_LOCAL);
	if (shared_library_handle == NULL) {
		MUSL_LOGI("FdTrack, Unable to open shared library %s: %s.\n", __fdtrack_hook_shared_lib, dlerror());
		return;
	}
}

#endif
