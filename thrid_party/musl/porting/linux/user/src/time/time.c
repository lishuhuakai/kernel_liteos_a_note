/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <time.h>
#include <errno.h>
#include <stdint.h>
#include "syscall.h"
#include "atomic.h"

#ifdef VDSO_TIME_SYM

static void *volatile vdso_time;

static time_t time_init(time_t *t)
{
	__get_vdso_info();
	void *p = __get_vdso_addr(VDSO_TIME_VER, VDSO_TIME_SYM);
	time_t (*f)(time_t *) =
		(time_t (*)(time_t *))p;
	a_cas_p(&vdso_time, (void *)time_init, p);
	return f ? f(t) : -ENOSYS;
}

static void *volatile vdso_time = (void *)time_init;

#endif

time_t time(time_t *t)
{
#ifdef VDSO_TIME_SYM
	time_t (*f)(time_t *) =
	(time_t (*)(time_t *))vdso_time;
    if (f) {
		return f(t);
	}
#endif

	struct timespec ts;
	__clock_gettime(CLOCK_REALTIME, &ts);
	if (t) *t = ts.tv_sec;
	return ts.tv_sec;
}
