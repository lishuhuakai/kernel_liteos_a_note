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
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "test.h"

/**
 * @tc.name      : strtoimax_0100
 * @tc.desc      : Test that strtoimax interprets string contents as base 10 integers
 * @tc.level     : Level 0
 */
void strtoimax_0100(void)
{
    char *p = {0};
    intmax_t want = -18737357;
    intmax_t result = strtoimax("-18737357foobar12", &p, 10);
    if (result != want) {
        t_error("%s strtoimax get result is %zu are not -18737357\n", __func__, result);
    }
    if (strcmp(p, "foobar12") != 0) {
        t_error("%s strtoimax get is '%s' are not 'foobar12'\n", __func__, p);
    }
}

/**
 * @tc.name      : strtoimax_0200
 * @tc.desc      : Test that strtoimax interprets string contents as base 16 integers
 * @tc.level     : Level 1
 */
void strtoimax_0200(void)
{
    char *p = {0};
    intmax_t want = -0x18737357f;
    intmax_t result = strtoimax("-18737357foobar12", &p, 16);
    if (result != want) {
        t_error("%s strtoimax get result is %zu are not -0x18737357f\n", __func__, result);
    }
    if (strcmp(p, "oobar12") != 0) {
        t_error("%s strtoimax get is '%s' are not 'oobar12'\n", __func__, p);
    }
}

int main(int argc, char *argv[])
{
    strtoimax_0100();
    strtoimax_0200();
    return t_status;
}