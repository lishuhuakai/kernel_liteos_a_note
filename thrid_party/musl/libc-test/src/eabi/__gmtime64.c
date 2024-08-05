/* Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include <time.h>
#include "test.h"

extern struct tm *gmtime(const time_t *t);

/**
 * @tc.name      : memset_0100
 * @tc.desc      : Test __gmtime64 method
 * @tc.level     : Level 1
 */
#define BST (+1)
#define CCT (+8)
 
void gmtime64_0100(void)
{
    time_t rawtime;
    struct tm *info;

    time(&rawtime);
    info = gmtime(&rawtime);

    if (info < 0) {
        t_error("%s __gmtime64 error get result is %2d\n", __func__, (info->tm_hour + CCT) % 24);
    }
}

int main()
{
    gmtime64_0100();
    return t_status;
}