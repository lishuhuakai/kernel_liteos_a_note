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

#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include "functionalext.h"

/**
 * @tc.name      : putc_unlocked_0100
 * @tc.desc      : write data the content to the stdout file (success)
 * @tc.level     : Level 0
 */
void putc_unlocked_0100(void)
{
    int ret;
    wchar_t ch[] = L"this is putc_unlocked_0100 test, std output success!\n";

    int length = sizeof(ch) / sizeof(wchar_t);
    for (int i = 0; i < length; i++) {
        ret = putc_unlocked(ch[i], stdout);
        EXPECT_EQ("putc_unlocked_0100", ret, ch[i]);
        EXPECT_NE("putc_unlocked_0100", ret, EOF);
    }
}

int main(void)
{
    putc_unlocked_0100();
    return t_status;
}
