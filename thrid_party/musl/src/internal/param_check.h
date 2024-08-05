/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PARAM_CHECK_H
#define PARAM_CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <info/fatal_message.h>

#define	SIZE (64)

#define PARAM_CHECK(p) do { \
	if (!p) { \
		char message[SIZE]; \
		snprintf(message, sizeof(message), "%s: parameter is null", __FUNCTION__); \
		set_fatal_message(message); \
		abort(); \
	} \
} while (0)
#endif
