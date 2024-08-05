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
 * @tc.name      : toupper_0100
 * @tc.desc      : Convert lowercase to uppercase via toupper
 * @tc.level     : Level 0
 */
void toupper_0100(void)
{
    char ch = 'a';
    char result = toupper(ch);
    if (result != 'A') {
        t_error("%s toupper get result is %c but want get value A ", __func__, result);
    }
}

/**
 * @tc.name      : toupper_0200
 * @tc.desc      : Test the result of passing numbers into toupper
 * @tc.level     : Level 1
 */
void toupper_0200(void)
{
    char ch = 2;
    char result = toupper(ch);
    if (result != ch) {
        t_error("%s toupper get result is %c but want get value %c ", __func__, result, ch);
    }
}

/**
 * @tc.name      : toupper_0300
 * @tc.desc      : Test the result of passing capital letters into toupper
 * @tc.level     : Level 1
 */
void toupper_0300(void)
{
    char ch = 'A';
    char result = toupper(ch);
    if (result != ch) {
        t_error("%s toupper get result is %c but want get value %c ", __func__, result, ch);
    }
}

/**
 * @tc.name      : toupper_0400
 * @tc.desc      : Test the result of passing special characters into toupper
 * @tc.level     : Level 2
 */
void toupper_0400(void)
{
    char ch = '$';
    char result = toupper(ch);
    if (result != ch) {
        t_error("%s toupper get result is %c but want get value %c ", __func__, result, ch);
    }
}

int main(int argc, char *argv[])
{
    toupper_0100();
    toupper_0200();
    toupper_0300();
    toupper_0400();
    return t_status;
}