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

#include "test.h"
#include "gwp_asan.h"
#include <malloc.h>

#define NUM_OF_MALLOC_FOR_OOM 10000
#define SIZE_OF_SINGLE_MALLOC 20

// It is expected that we can go to gwp_asan in higher frequency allocation scenarios.
int main()
{
    int found = 0;
    for (size_t i = 0; i < NUM_OF_MALLOC_FOR_OOM; i++) {
        void *ptr = malloc(SIZE_OF_SINGLE_MALLOC);
        if (!ptr) {
            t_error("FAIL malloc failed!\n");
        }
        if (libc_gwp_asan_ptr_is_mine(ptr)) {
            found = 1;
        }
    }
    if (!found) {
        t_error("FAIL it didn't go to gwp_asan alloctor!\n");
    } else {
        printf("It's ok to call gwp_asan in higher frequency allocation scenarios\n");
    }
}