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

#include <string.h>
#include "functionalext.h"

/**
 * @tc.name      : memmem_0100
 * @tc.desc      : Normal value test, assert whether the address value of the return value is
 *                 the created buffer address
 * @tc.level     : Level 0
 */
void memmem_0100(void)
{
    const char buffer[] = "musl test";
    const char cmp[] = "musl";
    char *ptr = NULL;

    for (int i = 0; i < sizeof(cmp) / sizeof(cmp[0]); i++) {
        ptr = memmem(buffer, sizeof(buffer), cmp, i);
        if (ptr == NULL) {
            EXPECT_PTRNE("memmem_0100", ptr, NULL);
            return;
        }
        EXPECT_PTREQ("memmem_0100", ptr, buffer);
    }
}

/**
 * @tc.name      : memmem_0200
 * @tc.desc      : Assert whether the returned value of an invalid parameter is null
 * @tc.level     : Level 2
 */
void memmem_0200(void)
{
    const char buffer[] = "musl test";
    size_t buflen = sizeof(buffer) / sizeof(buffer[0]);
    size_t errlen = buflen + 1;
    char *ptr = NULL;

    ptr = memmem(buffer, sizeof(buffer), "musl", errlen);
    EXPECT_PTREQ("memmem_0200", ptr, NULL);

    ptr = memmem(buffer, sizeof(buffer), "muls", 3);
    EXPECT_PTREQ("memmem_0200", ptr, NULL);
}

int main(void)
{
    memmem_0100();
    memmem_0200();

    return t_status;
}