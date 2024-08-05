/* Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include "syscall.h"
#include "atomic.h"

#ifdef SYS_cacheflush
int _flush_cache(void *addr, int len, int op)
{
	return syscall(SYS_cacheflush, addr, len, op);
}
weak_alias(_flush_cache, cacheflush);
#endif

#ifdef SYS_cachectl
int __cachectl(void *addr, int len, int op)
{
	return syscall(SYS_cachectl, addr, len, op);
}
weak_alias(__cachectl, cachectl);
#endif

#ifdef SYS_riscv_flush_icache

#define VDSO_FLUSH_ICACHE_SYM "__vdso_flush_icache"
#define VDSO_FLUSH_ICACHE_VER "LINUX_4.5"

static void *volatile vdso_func;

static int flush_icache_init(void *start, void *end, unsigned long int flags)
{
        __get_vdso_info();
	void *p = __get_vdso_addr(VDSO_FLUSH_ICACHE_VER, VDSO_FLUSH_ICACHE_SYM);
	int (*f)(void *, void *, unsigned long int) =
		(int (*)(void *, void *, unsigned long int))p;
	a_cas_p(&vdso_func, (void *)flush_icache_init, p);
	return f ? f(start, end, flags) : -ENOSYS;
}

static void *volatile vdso_func = (void *)flush_icache_init;

int __riscv_flush_icache(void *start, void *end, unsigned long int flags) 
{
	int (*f)(void *, void *, unsigned long int) =
		(int (*)(void *, void *, unsigned long int))vdso_func;
	if (f) {
		int r = f(start, end, flags);
		if (!r) return r;
		if (r != -ENOSYS) return __syscall_ret(r);
	}
}
weak_alias(__riscv_flush_icache, riscv_flush_icache);
#endif
