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

#include <sys/mman.h>
#include <stdio.h>
#include <wchar.h>

#include "functionalext.h"

/**
 * @tc.name      : mbrlen_0100
 * @tc.desc      : Verify that the obtained character length is 1 byte
 * @tc.level     : Level 0
 */
void mbrlen_0100(void)
{
    char test[] = {'m', 'u', 's', 'l'};
    const int num = sizeof(test) / sizeof(test[0]);
    size_t limitsize = sizeof(test[0]);
    for (unsigned int i = 0; i < num; i++) {
        EXPECT_EQ("mbrlen_0100", mbrlen(test + i, limitsize, NULL), 1);
    }
}

/**
 * @tc.name      : mbrlen_0200
 * @tc.desc      : Verify that the length return value of the incoming exception is not an illegal value
 * @tc.level     : Level 2
 */
void mbrlen_0200(void)
{
    char test[] = {'m', 'u', 's', 'l', '\0'};
    size_t index = 1;
    size_t ret = 0;
    const int num = sizeof(test) / sizeof(test[0]) - 1;
    for (unsigned int i = 0; i < num; i++) {
        ret = mbrlen(&test[i], 0, NULL);
        EXPECT_NE("mbrlen_0200", ret, index);
    }
    if (test[num] == '\0') {
        ret = mbrlen(&test[num], 1, NULL);
        EXPECT_EQ("mbrlen_0200", ret, CMPFLAG);
    }
}

int main(void)
{
    mbrlen_0100();
    mbrlen_0200();

    return t_status;
}