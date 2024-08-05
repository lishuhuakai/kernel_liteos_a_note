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

#include <ctype.h>
#include "test.h"

/**
 * @tc.name      : toascii_0100
 * @tc.desc      : Convert a character to ASCII
 * @tc.level     : Level 0
 */
void toascii_0100(void)
{
    int c1 = 'a' + 128;
    char c2 = toascii(c1);
    if (c2 != 'a') {
        t_error("%s toascii failed. c2 id %c", __func__, c2);
    }
}

/**
 * @tc.name      : toascii_0200
 * @tc.desc      : Convert a character to ASCII special symbol
 * @tc.level     : Level 1
 */
void toascii_0200(void)
{
    int c1 = 33;
    char c2 = toascii(c1);
    if (c2 != '!') {
        t_error("%s toascii failed. c2 id %c", __func__, c2);
    }
}

/**
 * @tc.name      : toascii_0300
 * @tc.desc      : Convert a character to an ASCII number
 * @tc.level     : Level 1
 */
void toascii_0300(void)
{
    int c1 = 48;
    char c2 = toascii(c1);
    if (c2 != '0') {
        t_error("%s toascii failed. c2 id %c", __func__, c2);
    }
}

int main(int argc, char *argv[])
{
    toascii_0100();
    toascii_0200();
    toascii_0300();
    return t_status;
}