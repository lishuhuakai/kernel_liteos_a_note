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

/**
 * @tc.name      : memcpy_0100
 * @tc.desc      : Test __aeabi_memcpy method
 * @tc.level     : Level 1
 */

extern void *__aeabi_memcpy(void *__restrict, const void *__restrict, size_t);
extern void *__aeabi_memcpy4(void *__restrict, const void *__restrict, size_t);
extern void *__aeabi_memcpy8(void *__restrict, const void *__restrict, size_t);

void memcpy_0100(void)
{
    int dest[] = {1, 2, 3, 4, 5, 6, 7};
    int src[] = {4, 5, 6, 7};
    void *result = __aeabi_memcpy(dest, src, 16);
    if (result < 0) {
        t_error("%s __aeabi_memcpy error get result is %d\n", __func__, result);
    }
}

/**
 * @tc.name      : memcpy_0200
 * @tc.desc      : Test __aeabi_memcpy4 method
 * @tc.level     : Level 1
 */
void memcpy_0200(void)
{
    int dest[] = {1, 2, 3, 4, 5, 6, 7};
    int src[] = {4, 5, 6, 7};
    void *result = __aeabi_memcpy4(dest, src, 16);
    if (result < 0) {
        t_error("%s __aeabi_memcpy4 error get result is %d\n", __func__, result);
    }
}

/**
 * @tc.name      : memcpy_0300
 * @tc.desc      : Test __aeabi_memcpy8 method
 * @tc.level     : Level 1
 */
void memcpy_0300(void)
{
    int dest[] = {1, 2, 3, 4, 5, 6, 7};
    int src[] = {4, 5, 6, 7};
    void *result = __aeabi_memcpy8(dest, src, 16);
    if (result < 0) {
        t_error("%s __aeabi_memcpy8 error get result is %d\n", __func__, result);
    }
}

int main()
{
    memcpy_0100();
    memcpy_0200();
    memcpy_0300();
    return t_status;
}