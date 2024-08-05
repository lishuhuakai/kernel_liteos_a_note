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

#ifndef _MUSL_MALLOC_DISPATCH_TABLE_H
#define _MUSL_MALLOC_DISPATCH_TABLE_H

#include "musl_malloc_dispatch.h"
#include <stdatomic.h>

struct musl_libc_globals {
	volatile atomic_llong current_dispatch_table;
	volatile atomic_llong so_dispatch_table;
	volatile atomic_llong memleak_tracker_so_dispatch_table;
	struct MallocDispatchType malloc_dispatch_table;
	struct MallocDispatchType memleak_tracker_malloc_dispatch_table;
};

#endif
