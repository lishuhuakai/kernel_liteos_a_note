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

#ifndef INT_SYSCALL_HOOKS_H
#define INT_SYSCALL_HOOKS_H

#include <features.h>
#include <stddef.h>

extern hidden volatile const char *g_syscall_hooks_table __attribute__((aligned(8)));
extern hidden volatile void *g_syscall_hooks_entry __attribute__((aligned(8)));

static inline int is_syscall_hooked(long n)
{
	return g_syscall_hooks_table != NULL && g_syscall_hooks_table[n] != 0;
}

static inline long __syscall_hooks_entry0(long n)
{
	return ((long (*)(long))g_syscall_hooks_entry)(n);
}

static inline long __syscall_hooks_entry1(long n, long a)
{
	return ((long (*)(long, long))g_syscall_hooks_entry)(n, a);
}

static inline long __syscall_hooks_entry2(long n, long a, long b)
{
	return ((long (*)(long, long, long))g_syscall_hooks_entry)(n, a, b);
}

static inline long __syscall_hooks_entry3(long n, long a, long b, long c)
{
	return ((long (*)(long, long, long, long))g_syscall_hooks_entry)(n, a, b, c);
}

static inline long __syscall_hooks_entry4(long n, long a, long b, long c, long d)
{
	return ((long (*)(long, long, long, long, long))g_syscall_hooks_entry)(n, a, b, c, d);
}

static inline long __syscall_hooks_entry5(long n, long a, long b, long c, long d, long e)
{
	return ((long (*)(long, long, long, long, long, long))g_syscall_hooks_entry)(n, a, b, c, d, e);
}

static inline long __syscall_hooks_entry6(long n, long a, long b, long c, long d, long e, long f)
{
	return ((long (*)(long, long, long, long, long, long, long))g_syscall_hooks_entry)(n, a, b, c, d, e, f);
}
#endif