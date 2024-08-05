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

#include <errno.h>
#include "stdarg.h"
#include "stdbool.h"
#include "pthread_impl.h"

struct user_param {
	unsigned long user_area;
	unsigned long user_sp;
	unsigned long map_base;
	unsigned int map_size;
};

int __thread_clone(int (*func)(void *), int flags, struct pthread *thread, unsigned char *sp)
{
	int ret;
	bool join_flag = false;
	struct user_param param;

	if (thread->detach_state == DT_JOINABLE) {
		join_flag = true;
	}

	param.user_area = TP_ADJ(thread);
	param.user_sp = sp;
	param.map_base = thread->map_base;
	param.map_size = thread->map_size;
	ret = __syscall(SYS_create_user_thread, func, &param, join_flag);
	if (ret < 0) {
		return ret;
	}

	thread->tid = (unsigned long)ret;
	return 0;
}
