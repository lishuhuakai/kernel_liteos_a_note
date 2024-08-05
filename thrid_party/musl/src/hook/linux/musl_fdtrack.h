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

#ifndef _MUSL_FDTRACK_H
#define _MUSL_FDTRACK_H

#include <sys/cdefs.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

enum fdtrack_type {
	FDTRACK_EVENT_TYPE_CREATE,
};

struct fdtrack_event {
	uint32_t type;
	int fd;
};

typedef void (*fdtrack_hook)(struct fdtrack_event*);
void set_fdtrack_enabled(bool newValue);
bool fdtrack_cas_hook(fdtrack_hook* expected, fdtrack_hook value);
bool normal_flow_control(struct timeval prevTime, int interval);
bool check_open_func(const char* expected, const char* key);
bool check_beta_develop_before();
bool check_before_memory_allocate(struct timeval prevTime, int interval);

#ifdef __cplusplus
}
#endif
#endif