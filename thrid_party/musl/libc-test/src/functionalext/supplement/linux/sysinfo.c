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

#include <errno.h>
#include <string.h>
#include <sys/sysinfo.h>

#include "test.h"

/**
 * @tc.name      : sysinfo_0100
 * @tc.desc      : return system information
 * @tc.level     : Level 0
 */
void sysinfo_0100(void)
{
    struct sysinfo info;
    memset(&info, 0, sizeof(info));

    int result = sysinfo(&info);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    if (info.uptime <= 0) {
        t_error("%s failed: info.uptime = %ld\n", __func__, info.uptime);
        return;
    }

    if (info.totalram < 0) {
        t_error("%s failed: info.totalram = %ld\n", __func__, info.totalram);
        return;
    }

    if (info.totalram < info.freeram) {
        t_error("%s failed: info.totalram = %ld, info.freeram = %ld\n", __func__, info.totalram, info.freeram);
        return;
    }

    if (info.totalswap < 0) {
        t_error("%s failed: info.totalswap = %ld\n", __func__, info.totalswap);
        return;
    }

    if (info.totalswap < info.freeswap) {
        t_error("%s failed: info.totalswap = %ld, info.freeswap = %ld\n", __func__, info.totalswap, info.freeswap);
        return;
    }
}

/**
 * @tc.name      : sysinfo_0200
 * @tc.desc      : return system information with NULL
 * @tc.level     : Level 2
 */
void sysinfo_0200(void)
{
    int result = sysinfo(NULL);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    if (errno != EFAULT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    sysinfo_0100();
    sysinfo_0200();

    return t_status;
}
