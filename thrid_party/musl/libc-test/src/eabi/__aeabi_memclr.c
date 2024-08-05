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
 * @tc.name      : memclr_0100
 * @tc.desc      : Test __aeabi_memclr method
 * @tc.level     : Level 1
 */
extern int __aeabi_memclr(void *dest, size_t n);
extern int __aeabi_memclr4(void *dest, size_t n);
extern int __aeabi_memclr8(void *dest, size_t n);

void memclr_0100(void)
{
    char dest[] = "__aeabi_memclr";
    if (__aeabi_memclr(dest, sizeof(dest)) == 0) {
        t_error("%s __aeabi_memclr", __func__);
    }
}

/**
 * @tc.name      : memclr_0200S
 * @tc.desc      : Test __aeabi_memclr4 method
 * @tc.level     : Level 1
 */
void memclr_0200(void)
{
    char dest[] = "__aeabi_memclr";
    if (__aeabi_memclr4(dest, sizeof(dest)) == 0) {
        t_error("%s __aeabi_memclr4", __func__);
    }
}

/**
 * @tc.name      : memclr_0300
 * @tc.desc      : Test __aeabi_memclr8 method
 * @tc.level     : Level 1
 */
void memclr_0300(void)
{
    char dest[] = "__aeabi_memclr";
    if (__aeabi_memclr8(dest, sizeof(dest)) == 0) {
        t_error("%s __aeabi_memclr8", __func__);
    }
}

int main()
{
    memclr_0100();
    memclr_0200();
    memclr_0300();
    return t_status;
}