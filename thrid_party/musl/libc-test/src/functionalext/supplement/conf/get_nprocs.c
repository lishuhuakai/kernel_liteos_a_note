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

#include <sys/sysinfo.h>
#include "test.h"

/**
 * @tc.name      : get_nprocs_0100
 * @tc.desc      : Verify the number of CPUs being used
 * @tc.level     : Level 0
 */
void get_nprocs_0100(void)
{
    int nprocs = get_nprocs();
    if (nprocs <= 0) {
        t_error("%s get_nprocs failed\n", __func__);
    }
    if (sysconf(_SC_NPROCESSORS_ONLN) != nprocs) {
        t_error("%s get_nprocs invalid\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    get_nprocs_0100();
    return t_status;
}