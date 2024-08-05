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
#include "test.h"

/**
 * @tc.name      : tolower_0100
 * @tc.desc      : Convert uppercase to lowercase by tolower
 * @tc.level     : Level 0
 */
void tolower_0100(void)
{
    char ch = 'A';
    char result = tolower(ch);
    if (result != 'a') {
        t_error("%s tolower get result is %c but want get value a ", __func__, result);
    }
}

/**
 * @tc.name      : tolower_0200
 * @tc.desc      : Test the result obtained by passing a number to tolower
 * @tc.level     : Level 1
 */
void tolower_0200(void)
{
    char ch = 2;
    char result = tolower(ch);
    if (result != ch) {
        t_error("%s tolower get result is %c but want get value %c ", __func__, result, ch);
    }
}

/**
 * @tc.name      : tolower_0300
 * @tc.desc      : Test the result of passing lowercase letters to tolower
 * @tc.level     : Level 1
 */
void tolower_0300(void)
{
    char ch = 'a';
    char result = tolower(ch);
    if (result != ch) {
        t_error("%s tolower get result is %c but want get value %c ", __func__, result, ch);
    }
}

/**
 * @tc.name      : tolower_0400
 * @tc.desc      : Test the result of passing special characters into tolower
 * @tc.level     : Level 2
 */
void tolower_0400(void)
{
    char ch = '$';
    char result = tolower(ch);
    if (result != ch) {
        t_error("%s tolower get result is %c but want get value %c ", __func__, result, ch);
    }
}

int main(int argc, char *argv[])
{
    tolower_0100();
    tolower_0200();
    tolower_0300();
    tolower_0400();
    return t_status;
}