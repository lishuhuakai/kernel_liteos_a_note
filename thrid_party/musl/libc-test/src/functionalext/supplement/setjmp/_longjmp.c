/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <setjmp.h>
#include "test.h"

void FAIL(void)
{
    t_error("%s failed, this function should be unreachable\n", __func__);
}

/**
 * @tc.name      : _longjmp_0100
 * @tc.desc      : Restore the environment saved by the  most recent invocation of setjmp() in the same process, with
 *                 the corresponding jmp_buf argument
 * @tc.level     : Level 0
 */
void _longjmp_0100(void)
{
    jmp_buf jb;
    int value = setjmp(jb);
    if (value == 0) {
        _longjmp(jb, 456);
        FAIL();
    } else {
        if (value != 456) {
            t_error("%s _longjmp failed\n", __func__);
        }
    }
}

int main(int argc, char *argv[])
{
    _longjmp_0100();
    return t_status;
}