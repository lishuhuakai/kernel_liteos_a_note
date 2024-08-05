/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include "gwp_asan.h"
#include "test.h"

#define NUM_OF_MALLOC_FOR_OOM 10000
#define SIZE_OF_SINGLE_MALLOC 20

void test_gwp_asan_oom()
{
    if (libc_gwp_asan_has_free_mem()) {
        for (size_t i = 0; i < NUM_OF_MALLOC_FOR_OOM; i++) {
            void *ptr = malloc(SIZE_OF_SINGLE_MALLOC);
            if (!ptr) {
                t_error("FAIL malloc failed!\n");
            }
        }
    } else {
        t_error("FAIL can't use gwp_asan!\n");
    }
    if (!libc_gwp_asan_has_free_mem()) {
        // test we can call default malloc when gwp_asan doesn't have free mem.
        void *ptr = malloc(SIZE_OF_SINGLE_MALLOC);
        if (!ptr) {
            t_error("FAIL malloc failed!\n");
        }
        if (libc_gwp_asan_ptr_is_mine(ptr)) {
            t_error("FAIL this addr(%p) shouldn't in gwp_asan area!\n", ptr);
        } else {
            printf("It's ok call default malloc\n");
        }
    } else {
        t_error("FAIL gwp_asan doesn't oom!\n");
    }
}

int main()
{
    void *ptr = malloc(2);
    if (!ptr) {
        return 0;
    }
    int size = malloc_usable_size(ptr);
    void *new_addr = realloc(ptr, 40);
    if (!new_addr) {
        return 0;
    }
    char c = *(char *)new_addr;
    printf("c:%c size:%d.\n", c, size);

    void *calloc_ptr = calloc(5, 4);
    if (!calloc_ptr) {
        return 0;
    }
    int value = *(int *)calloc_ptr;
    if (value != 0) {
        t_error("FAIL the memory value of gwp_asan calloc isn't 0 get %d", value);
    }

    free(new_addr);
    test_gwp_asan_oom();
    return 0;
}