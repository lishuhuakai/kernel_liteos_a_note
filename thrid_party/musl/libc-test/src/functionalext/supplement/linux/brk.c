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

#include <errno.h>
#include <unistd.h>
#include "test.h"

void *get_brk()
{
    return sbrk(0);
}

/**
 * @tc.name      : brk_0100
 * @tc.desc      : Verify Change the location of program breakpoints
 * @tc.level     : Level 0
 */
void brk_0100(void)
{
    errno = 0;
    void *initial_break = get_brk();
    void *new_break = (void *)((uintptr_t)(initial_break) + 1);

    int result = brk(new_break);
    if (result == -1) {
        if (errno != ENOMEM) {
            t_error("%s errno should be ENOMEM\n", __func__);
        }
    } else {
        if (result != 0) {
            t_error("%s brk failed\n", __func__);
        }
        if (get_brk() < new_break) {
            t_error("%s brk invalid\n", __func__);
        }
    }
}

int main(int argc, char *argv[])
{
    brk_0100();
    return t_status;
}