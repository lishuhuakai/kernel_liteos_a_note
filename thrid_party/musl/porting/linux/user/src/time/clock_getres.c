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

#ifdef VDSO_CGR_SYM

static void *volatile vdso_cgr;

static int cgr_init(clockid_t clk, struct timespec *ts)
{
	__get_vdso_info();
	void *p = __get_vdso_addr(VDSO_CGR_VER, VDSO_CGR_SYM);
	int (*f)(clockid_t, struct timespec *) =
		(int (*)(clockid_t, struct timespec *))p;
	a_cas_p(&vdso_cgr, (void *)cgr_init, p);
	return f ? f(clk, ts) : -ENOSYS;
}

static void *volatile vdso_cgr = (void *)cgr_init;

#endif

int clock_getres(clockid_t clk, struct timespec *ts)
{
	int r;
#ifdef VDSO_CGR_SYM
	int (*f)(clockid_t, struct timespec *) =
		(int (*)(clockid_t, struct timespec *))vdso_cgr;
	if (f) {
		r = f(clk, ts);
		if (!r) return r;
		if (r == -EINVAL) return __syscall_ret(r);
	}
#endif

#ifdef SYS_clock_getres_time64
	/* On a 32-bit arch, use the old syscall if it exists. */
	if (SYS_clock_getres != SYS_clock_getres_time64) {
		long ts32[2];
		r = __syscall(SYS_clock_getres, clk, ts32);
		if (!r && ts) {
			ts->tv_sec = ts32[0];
			ts->tv_nsec = ts32[1];
		}
		return __syscall_ret(r);
	}
#endif
	/* If reaching this point, it's a 64-bit arch or time64-only
	 * 32-bit arch and we can get result directly into timespec. */
	return syscall(SYS_clock_getres, clk, ts);
}
