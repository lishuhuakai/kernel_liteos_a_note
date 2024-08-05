/**
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

#ifndef __TIME_TEST_DATA_H__
#define __TIME_TEST_DATA_H__

#include <stdlib.h>
#include <string.h>
#include <time.h>

struct time_test_string_struct {
    const char *tz;
    const char *result;
};

struct time_test_double_struct {
    const char *tz;
    const double result;
};

struct time_test_time_struct {
    const char *tz;
    const time_t result;
};

static const char *test_handle_path(const char *tz)
{
    const char *p = tz;
    if (tz) {
#ifdef TIME_ZONE_SUB_TAG
        char *str = strrchr(tz, TIME_ZONE_SUB_TAG);
        if (str) {
            p = ++str;
        }
#endif
    }
    return p;
}

#endif
