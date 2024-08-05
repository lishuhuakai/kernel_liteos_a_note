/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <locale.h>
#include <stdlib.h>
#include "functionalext.h"

static const int lcMarkArry[] = {
    LC_PAPER_MASK,
    LC_NAME_MASK,
    LC_ADDRESS_MASK,
    LC_TELEPHONE_MASK,
    LC_MEASUREMENT_MASK,
    LC_IDENTIFICATION_MASK,
    LC_ALL_MASK
};

/**
 * @tc.name      : newlocale_0100
 * @tc.desc      : en-US is a invalid locale. The newlocale should return null
 * @tc.level     : Level 0
 */
void newlocale_0100(void)
{
    char *lo = setlocale(LC_ALL, "C");
    if (!lo) {
        EXPECT_PTRNE("newlocale_0100", lo, NULL);
        return;
    }
    locale_t newloc = newlocale(LC_ALL_MASK, "en_US", NULL);
    EXPECT_PTRNE("newlocale_0100", newloc, NULL);

    if (newloc) {
        freelocale(newloc);
        newloc = NULL;
    }
}

/**
 * @tc.name      : newlocale_0200
 * @tc.desc      : Check whether the LC_ALL type is passed to newlocale to create a custom locale environment
 * @tc.level     : Level 0
 */
void newlocale_0200(void)
{
    char *lo = setlocale(LC_ALL, "C");
    if (!lo) {
        EXPECT_PTRNE("newlocale_0200", lo, NULL);
        return;
    }
    locale_t newloc = newlocale(LC_ALL_MASK, "C", NULL);
    EXPECT_PTRNE("newlocale_0200", newloc, NULL);

    if (newloc) {
        freelocale(newloc);
        newloc = NULL;
    }
}

/**
 * @tc.name      : newlocale_0300
 * @tc.desc      : Determines whether the custom locale environment is created successfully
 * by passing different LC data types to newlocale
 * tips: en-US is a invalid locale. The newlocale should return null
 * @tc.level     : Level 0
 */
void newlocale_0300(void)
{
    char *lo = setlocale(LC_ALL, "C");
    if (!lo) {
        EXPECT_PTRNE("newlocale_0300", lo, NULL);
        return;
    }
    for (int i = 0; i < sizeof(lcMarkArry) / sizeof(lcMarkArry[0]); i++) {
        locale_t newloc = newlocale(lcMarkArry[i], "en_US", NULL);
        EXPECT_PTRNE("newlocale_0300", newloc, NULL);

        if (newloc) {
            freelocale(newloc);
            newloc = NULL;
        }
    }
}

/**
 * @tc.name      : newlocale_0400
 * @tc.desc      : Determines whether the custom locale environment is created successfully
 * by passing different LC data types to newlocale
 * tips: en-US is a invalid locale. The newlocale should return null
 * @tc.level     : Level 0
 */
void newlocale_0400(void)
{
    for (int i = 0; i < sizeof(lcMarkArry) / sizeof(lcMarkArry[0]); i++) {
        locale_t newloc = newlocale(lcMarkArry[i], "C", NULL);
        EXPECT_PTRNE("newlocale_0400", newloc, NULL);

        if (newloc) {
            freelocale(newloc);
            newloc = NULL;
        }

        newloc = newlocale(lcMarkArry[i], "C.UTF-8", NULL);
        EXPECT_PTRNE("newlocale_0400", newloc, NULL);

        if (newloc) {
            freelocale(newloc);
            newloc = NULL;
        }
    }
}

/**
 * @tc.name      : newlocale_0500
 * @tc.desc      : Determines whether the custom locale environment is created successfully
 * by passing different LC data types to newlocale zh_CN and zh_CN.UTF-8
 * @tc.level     : Level 0
 */
void newlocale_0500(void)
{
    for (int i = 0; i < sizeof(lcMarkArry) / sizeof(lcMarkArry[0]); i++) {
        locale_t newloc = newlocale(lcMarkArry[i], "zh_CN", NULL);
        EXPECT_PTRNE("newlocale_0500", newloc, NULL);

        if (newloc) {
            freelocale(newloc);
            newloc = NULL;
        }

        newloc = newlocale(lcMarkArry[i], "zh_CN.UTF-8", NULL);
        EXPECT_PTRNE("newlocale_0500", newloc, NULL);

        if (newloc) {
            freelocale(newloc);
            newloc = NULL;
        }
    }
}


/**
 * @tc.name      : newlocale_0600
 * @tc.desc      : Set de_DE to newlocale, which is not supported and should return NULL
 * @tc.level     : Level 0
 */
void newlocale_0600(void)
{
    locale_t newloc = newlocale(LC_ALL_MASK, "de_DE", NULL);
    EXPECT_PTREQ("newlocale_0600", newloc, NULL);

    if (newloc) {
        freelocale(newloc);
        newloc = NULL;
    }
}


int main(void)
{
    newlocale_0100();
    newlocale_0200();
    newlocale_0300();
    newlocale_0400();
    newlocale_0500();
    newlocale_0600();
    
    return t_status;
}