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

#include <string.h>
#include <stdio.h>
#include "network_conf_function.h"

char *g_fixedServices[] = {
#define PORT_DESC(a) a
#include "services.h"
#undef PORT_DESC(a)
};

#define FIXED_SERVICES_COUNT (sizeof(g_fixedServices) / sizeof(char*))

int get_services_str(char *line, FILE *f, int *indexPtr)
{
	if (f) {
		return fgets(line, sizeof line, f);
	}
	if (*indexPtr < FIXED_SERVICES_COUNT) {
		memcpy(line, g_fixedServices[*indexPtr], strlen(g_fixedServices[*indexPtr]));
		(*indexPtr)++;
		return 1;
	}
	return NULL;
}
