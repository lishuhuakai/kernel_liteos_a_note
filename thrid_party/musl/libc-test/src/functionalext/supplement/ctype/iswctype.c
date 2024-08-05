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
#include <ctype.h>
#include <locale.h>
#include <wchar.h>
#include "test.h"

void iswctype_test(wchar_t ch, wctype_t wt, char *func_name, int want)
{
    int result = iswctype(ch, wt);
    if (result != want) {
        t_error("%s iswctype get result is %d are not want %d", func_name, result, want);
    }
}

int main(int argc, char *argv[])
{
    wctype_t wt = wctype("alnum");
    /**
     * @tc.name      : iswctype_0100
     * @tc.desc      : Use the iswctype function to determine whether the incoming wide character matches the incoming
     *                type
     * @tc.level     : Level 0
     */
    iswctype_test(L'1', wt, "iswctype_0100", 1);
    wt = wctype("alpha");
    /**
     * @tc.name      : iswdigit_0200
     * @tc.desc      : Determine letter type wide characters
     * @tc.level     : Level 1
     */
    iswctype_test(L'a', wt, "iswctype_0200", 1);
    wt = wctype("blank");
    /**
     * @tc.name      : iswctype_0300
     * @tc.desc      : Determine whitespace type wide character
     * @tc.level     : Level 1
     */
    iswctype_test(L' ', wt, "iswctype_0300", 1);
    wt = wctype("cntrl");
    /**
     * @tc.name      : iswctype_0400
     * @tc.desc      : Determine the control type wide character
     * @tc.level     : Level 1
     */
    iswctype_test(L'\n', wt, "iswctype_0400", 1);
    wt = wctype("digit");
    /**
     * @tc.name      : iswctype_0500
     * @tc.desc      : Determine decimal type wide characters
     * @tc.level     : Level 1
     */
    iswctype_test(L'9', wt, "iswctype_0500", 1);
    wt = wctype("lower");
    /**
     * @tc.name      : iswctype_0600
     * @tc.desc      : Determining lowercase type wide characters
     * @tc.level     : Level 1
     */
    iswctype_test(L'a', wt, "iswctype_0600", 1);
    wt = wctype("print");
    /**
     * @tc.name      : iswctype_0700
     * @tc.desc      : Determine the printable type wide character
     * @tc.level     : Level 1
     */
    iswctype_test(L'u', wt, "iswctype_0700", 1);
    wt = wctype("punct");
    /**
     * @tc.name      : iswctype_0800
     * @tc.desc      : Determine the punctuation type wide character
     * @tc.level     : Level 1
     */
    iswctype_test(L',', wt, "iswctype_0800", 1);
    wt = wctype("upper");
    /**
     * @tc.name      : iswctype_0900
     * @tc.desc      : Determine uppercase type wide characters
     * @tc.level     : Level 1
     */
    iswctype_test(L'G', wt, "iswctype_0900", 1);
    return t_status;
}