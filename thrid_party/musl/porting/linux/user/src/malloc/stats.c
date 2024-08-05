/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdarg.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include "pthread_impl.h"

int malloc_info(int options, FILE* fp)
{
	return 0;
}

#ifdef USE_JEMALLOC_DFX_INTF
extern struct mallinfo je_mallinfo();
extern void je_malloc_stats_print(void (*write_cb)(void *, const char *), void *cbopaque,
	const char *opts);
#endif

void malloc_stats_print(void (*write_cb) (void *, const char *), void *cbopaque, const char *opts)
{
#ifdef USE_JEMALLOC_DFX_INTF
	je_malloc_stats_print(write_cb, cbopaque, opts);
#endif
}

struct mallinfo2 mallinfo2(void)
{
#ifdef USE_JEMALLOC_DFX_INTF
	struct mallinfo info = je_mallinfo();
	struct mallinfo2 res = {
		.hblks = info.hblks,
		.hblkhd = info.hblkhd,
		.usmblks = info.usmblks,
		.uordblks = info.uordblks,
		.fordblks = info.fordblks,
	};
	return res;
#endif
	return (struct mallinfo2){};
}

struct mallinfo mallinfo(void)
{
	struct mallinfo2 mallinfo2_res = mallinfo2();
	return (struct mallinfo) {
		.hblks = mallinfo2_res.hblks,
		.hblkhd = mallinfo2_res.hblkhd,
		.uordblks = mallinfo2_res.uordblks,
		.fordblks = mallinfo2_res.fordblks,
	};
}