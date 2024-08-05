/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include <errno.h>
#include <sched.h>
#include "test.h"

/*
 * @tc.name      : sched_getcpu_0100
 * @tc.desc      : Determine CPU on which the calling thread is running
 * @tc.level     : Level 0
 */
void sched_getcpu_0100(void)
{
    errno = 0;
    int result = sched_getcpu();
    if (result == -1) {
        t_error("%s sched_getcpu failed\n", __func__);
    }
    if (errno != 0) {
        t_error("%s errno is %d\n", errno);
    }
}

int main(int argc, char *argv[])
{
    sched_getcpu_0100();
    return t_status;
}