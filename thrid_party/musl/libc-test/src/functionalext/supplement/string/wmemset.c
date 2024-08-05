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
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wmemset_0100
 * @tc.desc      : Copies the specified number of wide characters from the source to the destination
 * @tc.level     : Level 0
 */
void wmemset_0100(void)
{
    const int index = 20;
    int count = 10;
    wchar_t ch = L'C';
    wchar_t des[index];
    wmemset(des, ch, count);
    for (int i = 0; i < count; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is not C\n", __func__, i);
        }
    }
}

/**
 * @tc.name      : wmemset_0200
 * @tc.desc      : Test wmemset when count is 0
 * @tc.level     : Level 1
 */
void wmemset_0200(void)
{
    const int index = 10;
    int count = 0;
    wchar_t ch = L'C';
    wchar_t des[index];
    wmemset(des, ch, count);
    for (int i = 0; i < index; i++) {
        if (des[i] == ch) {
            t_error("%s des[%d] is not want C\n", __func__, i);
        }
    }
}

/**
 * @tc.name      : wmemset_0300
 * @tc.desc      : Test wmemset when count is equal to the number characters in dest string
 * @tc.level     : Level 1
 */
void wmemset_0300(void)
{
    const int index = 10;
    int count = 10;
    wchar_t ch = L'C';
    wchar_t des[index];
    wmemset(des, ch, count);
    for (int i = 0; i < index; i++) {
        if (des[i] != ch) {
            t_error("%s des[%d] is not want C\n", __func__, i);
        }
    }
}

int main(int argc, char *argv[])
{
    wmemset_0100();
    wmemset_0200();
    wmemset_0300();
    return t_status;
}