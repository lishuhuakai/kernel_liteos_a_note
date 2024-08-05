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

#include "functionalext.h"

uintptr_t __gnu_Unwind_Find_exidx(uintptr_t pc, int *pcount);

const int32_t INIT_LEN = 0;
const int32_t COUNT = 32;

/**
 * @tc.name:      a_gnu_Unwind_Find_exidx_0100
 * @tc.desc:      Verify gnu Unwind Find exidx process success.
 * @tc.level:     level 2.
 */
void a_gnu_Unwind_Find_exidx_0100(void)
{
    uintptr_t pc = INIT_LEN;
    int32_t pcount = COUNT;
    int32_t ret = __gnu_Unwind_Find_exidx(pc, &pcount);
    EXPECT_EQ("a_gnu_Unwind_Find_exidx_0100", ret, INIT_LEN);
}

int main(void)
{
    a_gnu_Unwind_Find_exidx_0100();
    return t_status;
}
