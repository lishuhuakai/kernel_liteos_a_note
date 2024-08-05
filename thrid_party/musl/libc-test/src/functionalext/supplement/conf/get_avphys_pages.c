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
 * @tc.name      : get_avphys_page_0100
 * @tc.desc      : Verify the total number of pages available for acquisition
 * @tc.level     : Level 0
 */
void get_avphys_page_0100(void)
{
    long avail_phys_pages = get_avphys_pages();
    if (avail_phys_pages <= 0) {
        t_error("%s get_avphys_pages failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    get_avphys_page_0100();
    return t_status;
}