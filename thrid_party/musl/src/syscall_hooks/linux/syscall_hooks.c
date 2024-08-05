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

#include <stdio.h>
#include <syscall.h>
#include <assert.h>
#include <syscall_hooks.h>

#include "libc.h"
#include "pthread_impl.h"

#define HIJACK_SYSCALL_MAX  1024

volatile const char *g_syscall_hooks_table __attribute__((aligned(8))) = NULL;
volatile void *g_syscall_hooks_entry __attribute__((aligned(8))) = NULL;

/*
 * @brief
 *     reset == 0: g_syscall_hooks_table and g_syscall_hooks_entry will be the function
 *                 input parameters hooks_table and hooks_entry.
 *     reset != 0: g_syscall_hooks_table and g_syscall_hooks_entry will be default values
 *
 * @param
 *     hooks_table: pointer to a syscall table
 *     hooks_entry: pointer to svc0_entry
 *
 * @return
 *     -EINVAL: input parameters are invalid
 *     -EPERM: operation not allowed
 *     0: set_syscall_hooks success
*/

int set_syscall_hooks(const char *hooks_table, int table_len, void *hooks_entry, int reset, int **tid_addr)
{
	int ret = 0;

	if (reset == 0) {
		if (hooks_table == NULL || hooks_entry == NULL || tid_addr == NULL ||
			table_len < HIJACK_SYSCALL_MAX) {
				ret = -EINVAL;
			}
	}

	if (ret == 0) {
		__tl_lock();
		if (!libc.threads_minus_1) {
			if (reset) {
				g_syscall_hooks_entry = NULL;
				g_syscall_hooks_table = NULL;
			} else {
				g_syscall_hooks_entry = hooks_entry;
				g_syscall_hooks_table = hooks_table;
			}
			*tid_addr = &__thread_list_lock;
		} else {
			ret = -EPERM;
		}
		__tl_unlock();
	}

	return ret;
}