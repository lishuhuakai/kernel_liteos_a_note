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
#include <wchar.h>
#include <ctype.h>
#include <wctype.h>
#include "test.h"

/**
 * @tc.name      : wctrans_0100
 * @tc.desc      : Test that wctrans returns a character conversion of a scalar type
 * @tc.level     : Level 0
 */
void wctrans_0100(void)
{
    if (!wctrans("tolower")) {
        t_error("%s wctrans error get result is NULL\n", __func__);
    }
}

/**
 * @tc.name      : wctrans_0200
 * @tc.desc      : Test result of wctrans when incoming uppercase conversion
 * @tc.level     : Level 1
 */
void wctrans_0200(void)
{
    if (!wctrans("toupper")) {
        t_error("%s wctrans error get result is NULL\n", __func__);
    }
}

/**
 * @tc.name      : wctrans_0300
 * @tc.desc      : Test the result of wctrans when passing in an exception parameter
 * @tc.level     : Level 2
 */
void wctrans_0300(void)
{
    if (wctrans("monkeys")) {
        t_error("%s wctrans error get result is not NULL\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    wctrans_0100();
    wctrans_0200();
    wctrans_0300();
    return t_status;
}