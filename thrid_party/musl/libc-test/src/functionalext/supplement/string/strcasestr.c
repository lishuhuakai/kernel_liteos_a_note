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
 * @tc.name      : strcasestr_0100
 * @tc.desc      : Test strcasestr method to intercept string
 * @tc.level     : Level 0
 */
void strcasestr_0100(void)
{
    char *haystate = "bIg dAdDy/gIaNt hAyStAcKs";
    char *result = strcasestr(haystate, "");
    if (strcmp(result, "bIg dAdDy/gIaNt hAyStAcKs") != 0) {
        t_error("%s strcasestr error get result is %s\n", __func__, result);
    }
}

/**
 * @tc.name      : strcasestr_0200
 * @tc.desc      : Test strcasestr method incoming uppercase letter interception
 * @tc.level     : Level 1
 */
void strcasestr_0200(void)
{
    char *haystate = "bIg dAdDy/gIaNt hAyStAcKs";
    char *result = strcasestr(haystate, "B");
    if (strcmp(result, "bIg dAdDy/gIaNt hAyStAcKs") != 0) {
        t_error("%s strcasestr error get result is %s\n", __func__, result);
    }
}

/**
 * @tc.name      : strcasestr_0300
 * @tc.desc      : Test strcasestr method incoming lowercase letter interception
 * @tc.level     : Level 1
 */
void strcasestr_0300(void)
{
    char *haystate = "bIg dAdDy/gIaNt hAyStAcKs";
    char *result = strcasestr(haystate, "i");
    if (strcmp(result, "Ig dAdDy/gIaNt hAyStAcKs") != 0) {
        t_error("%s strcasestr error get result is %s\n", __func__, result);
    }
}

/**
 * @tc.name      : strcasestr_0400
 * @tc.desc      : Test strcasestr method incoming string interception
 * @tc.level     : Level 1
 */
void strcasestr_0400(void)
{
    char *haystate = "bIg dAdDy/gIaNt hAyStAcKs";
    char *result = strcasestr(haystate, "Da");
    if (strcmp(result, "dAdDy/gIaNt hAyStAcKs") != 0) {
        t_error("%s strcasestr error get result is %s\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    strcasestr_0100();
    strcasestr_0200();
    strcasestr_0300();
    strcasestr_0400();
    return t_status;
}