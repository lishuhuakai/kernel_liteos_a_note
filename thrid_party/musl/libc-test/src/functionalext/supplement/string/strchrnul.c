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

#include <string.h>
#include "test.h"

/**
 * @tc.name      : strchrnul_0100
 * @tc.desc      : Find the specified character in a string
 * @tc.level     : Level 0
 */
void strchrnul_0100(void)
{
    char *str = "Hello world";
    char *tmp = NULL;

    tmp = strchrnul(str, 'w');
    if (tmp == NULL) {
        t_error("%s tmp is NULL", __func__);
    }

    if (strcmp(tmp, "world")) {
        t_error("%s tmp is %s, not world", __func__, tmp);
    }
}

/**
 * @tc.name      : strchrnul_0200
 * @tc.desc      : Find \0 Character in String
 * @tc.level     : Level 1
 */
void strchrnul_0200(void)
{
    char *str = "Hello world";
    char *tmp = NULL;

    tmp = strchrnul(str, '\0');
    if (tmp == NULL) {
        t_error("%s tmp is NULL", __func__);
    }

    if (strcmp(tmp, "")) {
        t_error("%s tmp is %s, not 0", __func__, tmp);
    }
}

/**
 * @tc.name      : strchrnul_0300
 * @tc.desc      : Find \0 Character in String
 * @tc.level     : Level 2
 */
void strchrnul_0300(void)
{
    char *str = "Hello world";
    char *tmp = NULL;

    tmp = strchrnul(str, 'a');
    if (tmp == NULL) {
        t_error("%s tmp is NULL", __func__);
    }

    if (strcmp(tmp, "")) {
        t_error("%s tmp is %s, not NULL", __func__, tmp);
    }
}

int main(int argc, char *argv[])
{
    strchrnul_0100();
    strchrnul_0200();
    strchrnul_0300();
    return t_status;
}
