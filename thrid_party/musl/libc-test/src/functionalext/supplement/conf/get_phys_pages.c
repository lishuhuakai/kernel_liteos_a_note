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
 * @tc.name      : get_phys_pages_0100
 * @tc.desc      : Verify the total number of pages fetched in memory
 * @tc.level     : Level 0
 */
void get_phys_pages_0100(void)
{
    long phys_pages = get_phys_pages();
    if (sysconf(_SC_PHYS_PAGES) != phys_pages) {
        t_error("%s get_phys_pages failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    get_phys_pages_0100();
    return t_status;
}