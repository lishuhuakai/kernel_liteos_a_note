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
#include <locale.h>
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wctob_0100
 * @tc.desc      : Call the wctob function to convert wide characters to single bytes
 * @tc.level     : Level 0
 */
void wctob_0100(void)
{
    wchar_t c = L'A';
    int result = wctob(c);
    if (result != 'A') {
        t_error("%s wctob get result is %c are not 'A'", __func__, result);
    }
}

/**
 * @tc.name      : wctob_0200
 * @tc.desc      : characters over 0x80
 * @tc.level     : Level 1
 */
void wctob_0200(void)
{
    wchar_t c = L'\u00df';
    int result = wctob(c);
    if (result != EOF) {
        t_error("%s wctob get result is %d are not -1", __func__, result);
    }
}

/**
 * @tc.name      : wctob_0300
 * @tc.desc      : Wide characters exceed 0xff00
 * @tc.level     : Level 1
 */
void wctob_0300(void)
{
    wchar_t c = L'\uff02';
    int result = wctob(c);
    if (result != EOF) {
        t_error("%s wctob get result is %c are not EOF", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "en_US.utf8");

    wctob_0100();
    wctob_0200();
    wctob_0300();
    return t_status;
}