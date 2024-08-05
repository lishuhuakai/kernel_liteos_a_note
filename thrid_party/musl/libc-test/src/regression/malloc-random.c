/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "test.h"

#define MALLOC_SIZE_S 0x20
#define MALLOC_SIZE_L 0x40
#define IDX_IN_CHUNK (-3)

static int get_slotnum(uint8_t *p)
{
	return (p[IDX_IN_CHUNK] & 0x1f);
}

int main(void)
{
	void *p = malloc(MALLOC_SIZE_S);
	void *q = malloc(MALLOC_SIZE_S);
	void *r = malloc(MALLOC_SIZE_S);
	if (!p || !q || !r)
		t_error("malloc returned NULL\n");

	int slot_p_1 = get_slotnum((uint8_t *)p);
	int slot_q_1 = get_slotnum((uint8_t *)q);
	int slot_r_1 = get_slotnum((uint8_t *)r);

	free(p);
	free(q);
	free(r);

	p = malloc(MALLOC_SIZE_L);
	q = malloc(MALLOC_SIZE_L);
	r = malloc(MALLOC_SIZE_L);
	if (!p || !q || !r)
		t_error("malloc returned NULL\n");

	int slot_p_2 = get_slotnum((uint8_t *)p);
	int slot_q_2 = get_slotnum((uint8_t *)q);
	int slot_r_2 = get_slotnum((uint8_t *)r);

	free(p);
	free(q);
	free(r);

	if (((slot_p_1 + 1) == slot_q_1) && ((slot_q_1 + 1) == slot_r_1)) {
		if (((slot_p_2 + 1) == slot_q_2) && ((slot_q_2 + 1) == slot_r_2))
			t_error("malloc(0) random error\n");
	}
	return t_status;
}

