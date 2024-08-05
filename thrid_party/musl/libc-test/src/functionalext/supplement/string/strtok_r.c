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
 * @tc.name      : strtok_r_0100
 * @tc.desc      : Test strtok_r function to intercept string according to input characters
 * @tc.level     : Level 0
 */
void strtok_r_0100(void)
{
    char *compare[3] = {"ABC", "DEF", "GHI"};
    char buf[] = "ABC,DEF,GHI";
    char *result[3];
    char *p = buf;
    char *outer_ptr = NULL;
    for (int i = 0; i < 3; i++) {
        result[i] = strtok_r(outer_ptr, ",", &p);
    }
    for (int a = 0; a < 3; a++) {
        if (strcmp(result[a], compare[a]) != 0) {
            t_error("%s strtok_r in %d get value is %s are not %s\n", __func__, a, result[a], compare[a]);
        }
    }
}

/**
 * @tc.name      : strtok_r_0200
 * @tc.desc      : Test the return value of the strtok_r function when the intercepted string is an empty string
 * @tc.level     : Level 1
 */
void strtok_r_0200(void)
{
    char *p;
    char *outer_ptr = NULL;
    char *result = strtok_r(outer_ptr, ",", &p);
    if (result) {
        t_error("%s strtok_r error get result is %s not null\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    strtok_r_0100();
    strtok_r_0200();
    return t_status;
}