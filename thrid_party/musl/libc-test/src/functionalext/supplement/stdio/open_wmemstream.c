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

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include "functionalext.h"

/**
 * @tc.name      : open_wmemstream_0100
 * @tc.desc      : Create and open a memory stream file(success)
 * @tc.level     : Level 0
 */
void open_wmemstream_0100(void)
{
    size_t len = -1;
    int ret = 0;
    wchar_t *ptr = NULL;

    FILE *fp = open_wmemstream(&ptr, &len);
    if (fp == NULL) {
        EXPECT_PTRNE("open_wmemstream_0100", fp, NULL);
        return;
    }
    ret = fprintf(fp, "open_wmemstream success!");
    EXPECT_PTRNE("open_wmemstream_0100", ptr, NULL);
    ret = fclose(fp);
    free(ptr);
    ptr = NULL;
}

int main(void)
{
    open_wmemstream_0100();
    return t_status;
}
