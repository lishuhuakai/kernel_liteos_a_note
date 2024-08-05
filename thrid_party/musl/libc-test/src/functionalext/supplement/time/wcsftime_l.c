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
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>
#include "test.h"

/**
 * @tc.name      : wcsftime_l_0100
 * @tc.desc      : Format system time
 * @tc.level     : Level 0
 */
void wcsftime_l_0100(void)
{
    time_t rtime;
    struct tm *timeinfo;
    wchar_t buffer[80];
    time(&rtime);
    timeinfo = localtime(&rtime);
    locale_t newlocale_ = newlocale(LC_ALL_MASK, "en_US", NULL);
    size_t result = wcsftime_l(buffer, 80, L"%I:%M%p", timeinfo, newlocale_);
    if (!result) {
        t_error("%s wcsftime_l failed\n", __func__);
    }
    if (newlocale_) {
        freelocale(newlocale_);
        newlocale_ = NULL;
    }
}

/**
 * @tc.name      : wcsftime_l_0200
 * @tc.desc      : Format the given tm time
 * @tc.level     : Level 1
 */
void wcsftime_l_0200(void)
{
    wchar_t buff[40];
    struct tm mtime = {
        .tm_year = 122,
        .tm_mon = 5,
        .tm_mday = 6,
        .tm_hour = 8,
        .tm_min = 10,
        .tm_sec = 20,
    };
    locale_t newlocale_ = newlocale(LC_ALL_MASK, "en_US", NULL);
    size_t result = wcsftime_l(buff, sizeof buff, L"%A %c", &mtime, newlocale_);
    if (!result) {
        t_error("%s wcsftime_l failed\n", __func__);
    }
    if (wcscmp(buff, L"Sunday Sun Jun  6 08:10:20 2022")) {
        t_error("%s wrong format result is %ls\n", __func__, buff);
    }
    if (newlocale_) {
        freelocale(newlocale_);
        newlocale_ = NULL;
    }
}

int main(int argc, char *argv[])
{
    wcsftime_l_0100();
    wcsftime_l_0200();
    return t_status;
}