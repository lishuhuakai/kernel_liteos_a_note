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

extern void *__aeabi_read_tp();
/**
 * @tc.name      : memset_0100
 * @tc.desc      : Test __aeabi_read_tp method
 * @tc.level     : Level 1
 */
void aeabi_read_tp_0100(void)
{
    void *result = __aeabi_read_tp();
    if (result < 0) {
        t_error("%s __aeabi_read_tp error get result is %d\n", __func__, __aeabi_read_tp);
    }
}

int main()
{
    aeabi_read_tp_0100();
    return t_status;
}