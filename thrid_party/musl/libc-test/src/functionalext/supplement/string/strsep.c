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
 * @tc.name      : strsep_0100
 * @tc.desc      : Test calling strsep function to intercept string
 * @tc.level     : Level 0
 */
void strsep_0100(void)
{
    char str[12] = {0};
    strcpy(str, "helloworld");
    char *p = (char *)str;
    char *sep = "w";
    char *result = strsep(&p, sep);
    if (strcmp(result, "hello") != 0) {
        t_error("%s strsep get result is '%s' are not 'hello'\n", __func__, result);
    }
    if (strcmp(str, "hello") != 0) {
        t_error("%s strsep get str is '%s' are not 'hello'\n", __func__, str);
    }
}

/**
 * @tc.name      : strsep_0200
 * @tc.desc      : Test strsep result when marker character appears multiple times in truncated string
 * @tc.level     : Level 1
 */
void strsep_0200(void)
{
    char str[12] = {0};
    strcpy(str, "helloworld");
    char *p = (char *)str;
    char *sep = "l";
    char *result = strsep(&p, sep);
    if (strcmp(result, "he") != 0) {
        t_error("%s strsep get result is '%s' are not 'he'\n", __func__, result);
    }
    if (strcmp(str, "he") != 0) {
        t_error("%s strsep get result is '%s' are not 'he'\n", __func__, str);
    }
}

/**
 * @tc.name      : strsep_0300
 * @tc.desc      : test strsep result when marker character does not appear in truncated string
 * @tc.level     : Level 1
 */
void strsep_0300(void)
{
    char str[12] = {0};
    strcpy(str, "helloworld");
    char *p = (char *)str;
    char *sep = "a";
    char *result = strsep(&p, sep);
    if (strcmp(result, "helloworld") != 0) {
        t_error("%s strsep get result is '%s' are not 'helloworld'\n", __func__, result);
    }
    if (strcmp(str, "helloworld") != 0) {
        t_error("%s strsep get result is '%s' are not 'helloworld'\n", __func__, str);
    }
}

int main(int argc, char *argv[])
{
    strsep_0100();
    strsep_0200();
    strsep_0300();
    return t_status;
}