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
#include "musl_fdtrack.h"
#include "stdatomic_impl.h"
#include "sys_param.h"

enum EnumBetaDevelopMode {
	NOT_CHECK,
	ENABLE,
	DISABLE,
};

_Atomic(fdtrack_hook) __fdtrack_hook;
bool __fdtrack_enabled = false;

static char *__is_beta_version = "beta";
static char *__key_version_type = "const.logsystem.versiontype";
static char *__is_develop_mode = "enable";
static char *__key_develop_mode = "persist.hiview.leak_detector";
static enum EnumBetaDevelopMode __beta_develop_mode = NOT_CHECK;
static int TIME_IN_MILLISECONDS = 1000000;

void set_fdtrack_enabled(bool newValue)
{
	__fdtrack_enabled = newValue;
}

bool fdtrack_cas_hook(fdtrack_hook* expected, fdtrack_hook value)
{
	return atomic_compare_exchange_strong(&__fdtrack_hook, expected, value);
}

bool normal_flow_control(struct timeval prevTime, int interval)
{
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	int timeDifferenceSeconds = ((currentTime.tv_sec * TIME_IN_MILLISECONDS + currentTime.tv_usec) -
								(prevTime.tv_sec * TIME_IN_MILLISECONDS + prevTime.tv_usec)) / TIME_IN_MILLISECONDS;
	return timeDifferenceSeconds > interval;
}

bool check_open_func(const char*expected, const char* key)
{
	CachedHandle handle = CachedParameterCreate(key, "unknown");
	const char *value = CachedParameterGet(handle);
	return (value != NULL && strncmp(value, expected, strlen(expected)) == 0);
}

bool check_beta_develop_before()
{
	if (__beta_develop_mode == NOT_CHECK) {
		if (check_open_func(__is_beta_version, __key_version_type) ||
			check_open_func(__is_develop_mode, __key_develop_mode)) {
			__beta_develop_mode = ENABLE;
		} else {
			__beta_develop_mode = DISABLE;
		}
	}
	return __beta_develop_mode == ENABLE;
}

bool check_before_memory_allocate(struct timeval prevTime, int interval)
{
	if (!check_beta_develop_before()) {
		return false;
	}
	if (!(prevTime.tv_sec == 0 || normal_flow_control(prevTime, interval))) {
		return false;
	}
	return true;
}

#endif