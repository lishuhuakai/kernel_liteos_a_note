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
 * @tc.name      : strndup_0100
 * @tc.desc      : Test the strndup function to copy the string according to the input parameter
 * @tc.level     : Level 0
 */
void strndup_0100(void)
{
    char *str = "helloworld";
    char *result = strndup(str, 2);
    if (strcmp(result, "he")) {
        t_error("%s strndup get result is '%s' are not 'he'\n", __func__, result);
    }
}

/**
 * @tc.name      : strndup_0200
 * @tc.desc      : Test the return value of strndup when the number of copies of the input exceeds the passed string
 * @tc.level     : Level 1
 */
void strndup_0200(void)
{
    char *str = "helloworld";
    char *result = strndup(str, 12);
    if (strcmp(result, "helloworld")) {
        t_error("%s strndup get result is '%s' are not 'he'\n", __func__, result);
    }
}

/**
 * @tc.name      : strndup_0300
 * @tc.desc      : The return value of strndup when the number of test copies is 0
 * @tc.level     : Level 1
 */
void strndup_0300(void)
{
    char *str = "helloworld";
    char *result = strndup(str, 0);
    if (strcmp(result, "") != 0) {
        t_error("%s strndup get result is '%s' are not ''\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    strndup_0100();
    strndup_0200();
    strndup_0300();
    return t_status;
}