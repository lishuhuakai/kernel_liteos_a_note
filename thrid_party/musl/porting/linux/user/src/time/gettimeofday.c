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
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include "syscall.h"
#include "atomic.h"

#ifdef VDSO_GTD_SYM

static void *volatile vdso_gtd;

static int gtd_init(struct timeval *tv, void *tz)
{
	__get_vdso_info();
	void *p = __get_vdso_addr(VDSO_GTD_VER, VDSO_GTD_SYM);
	int (*f)(struct timeval *, void *) =
		(int (*)(struct timval *, void *))p;
	a_cas_p(&vdso_gtd, (void *)gtd_init, p);
	return f ? f(tv, tz) : -ENOSYS;
}

static void *volatile vdso_gtd = (void *)gtd_init;

#endif

int gettimeofday(struct timeval *restrict tv, void *restrict tz)
{
#ifdef VDSO_GTD_SYM
    int r;
	int (*f)(struct timeval *, void *) =
	(int (*)(struct timeval *, void *))vdso_gtd;
    if (f) {
		r = f(tv, tz);
		if (!r) return r;
		if (r == -EINVAL) return __syscall_ret(r);
	}
#endif
	
	struct timespec ts;
	if (!tv) return 0;
	clock_gettime(CLOCK_REALTIME, &ts);
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = (int)ts.tv_nsec / 1000;
	return 0;
}
