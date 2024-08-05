/* Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include <string.h>
#include "test.h"

extern void *__aeabi_memset(void *, int, size_t);
extern void *__aeabi_memset4(void *, int, size_t);
extern void *__aeabi_memset8(void *, int, size_t);

/**
 * @tc.name      : memset_0100
 * @tc.desc      : Test __aeabi_memset method
 * @tc.level     : Level 1
 */
void memset_0100(void)
{
    int dest[] = {1, 2, 3, 4, 5, 6, 7};
    int src = 7;
    int *result = __aeabi_memset(dest, src, 16);
    if (result < 0) {
        t_error("%s __aeabi_memset error get result is %d\n", __func__, result);
    }
}

/**
 * @tc.name      : memset_0200
 * @tc.desc      : Test __aeabi_memset4 method
 * @tc.level     : Level 1
 */
void memset_0200(void)
{
    int dest[] = {1, 2, 3, 4, 5, 6, 7};
    int src = 7;
    int *result = __aeabi_memset4(dest, src, 16);
    if (result < 0) {
        t_error("%s __aeabi_memset4 error get result is %d\n", __func__, result);
    }
}

/**
 * @tc.name      : memset_0300
 * @tc.desc      : Test __aeabi_memset8 method
 * @tc.level     : Level 1
 */
void memset_0300(void)
{
    int dest[] = {1, 2, 3, 4, 5, 6, 7};
    int src = 7;
    int *result = __aeabi_memset8(dest, src, 16);
    if (result < 0) {
        t_error("%s __aeabi_memset8 error get result is %d\n", __func__, result);
    }
}

int main()
{
    memset_0100();
    memset_0200();
    memset_0300();
    return t_status;
}