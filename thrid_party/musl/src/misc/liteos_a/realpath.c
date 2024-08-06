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

#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "syscall.h"

char *realpath(const char *restrict filename, char *restrict resolved)
{
	int ret;
	char tmp[PATH_MAX];

	if (!filename) {
		errno = EINVAL;
		return 0;
	}

	ret = syscall(SYS_realpath, filename, tmp);
	if (ret < 0) return 0;

	return resolved ? strcpy(resolved, tmp) : strdup(tmp);
}
