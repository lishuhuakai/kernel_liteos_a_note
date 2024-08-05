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

#define _GNU_SOURCE 1
#include <sched.h>
#include <malloc.h>

cpu_set_t* __sched_cpualloc(size_t count)
{
    // The static analyzer complains that CPU_ALLOC_SIZE eventually expands to
    // N * sizeof(unsigned long), which is incompatible with cpu_set_t. This is
    // on purpose.
    return (cpu_set_t*) malloc(CPU_ALLOC_SIZE(count)); // NOLINT
}
