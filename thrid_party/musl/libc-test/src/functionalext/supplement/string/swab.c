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
#include "test.h"

/**
 * @tc.name      : swab_0100
 * @tc.desc      : Swap bytes from source and destination area
 * @tc.level     : Level 0
 */
void swab_0100(void)
{
    char src[20] = "swab";
    char dest[20];
    swab(src, dest, strlen(src));
    if (strcmp(dest, "wsba")) {
        t_error("%s swab failed", __func__);
    }
}

/**
 * @tc.name      : swab_0200
 * @tc.desc      : Swap bytes from source and destination area, and dest is initialized
 * @tc.level     : Level 1
 */
void swab_0200(void)
{
    char src[20] = "swab";
    char dest[20] = "swab";
    swab(src, dest, strlen(src));
    if (strcmp(dest, "wsba")) {
        t_error("%s swab failed", __func__);
    }
}

int main(int argc, char *argv[])
{
    swab_0100();
    swab_0200();
    return t_status;
}