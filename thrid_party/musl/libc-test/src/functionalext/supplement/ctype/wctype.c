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

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include "test.h"

/**
 * @tc.name      : wctype_0100
 * @tc.desc      : Test the return value when wctype input parameters are alnum and error strings
 * @tc.level     : Level 0
 */
void wctype_0100(void)
{
    wctype_t expect0 = wctype("alnum");
    wctype_t target = 1;
    if (expect0 != target) {
        t_error("%s expect type is not alnum", __func__);
    }

    wctype_t expect1 = wctype("alnumm");
    if (expect1 == target) {
        t_error("%s error input type is alnum", __func__);
    }
}

/**
 * @tc.name      : wctype_0200
 * @tc.desc      : Test the return value when wctype input parameters are alpha and error strings
 * @tc.level     : Level 0
 */
void wctype_0200(void)
{
    wctype_t expect0 = wctype("alpha");
    wctype_t target = 2;
    if (expect0 != target) {
        t_error("%s expect type is not alpha", __func__);
    }

    wctype_t expect1 = wctype("alphaa");
    if (expect1 == target) {
        t_error("%s error input type is alpha", __func__);
    }
}

/**
 * @tc.name      : wctype_0300
 * @tc.desc      : Test the return value when wctype input parameters are blank and error strings
 * @tc.level     : Level 0
 */
void wctype_0300(void)
{
    wctype_t expect0 = wctype("blank");
    wctype_t target = 3;
    if (expect0 != target) {
        t_error("%s expect type is not blank", __func__);
    }

    wctype_t expect1 = wctype("blankk");
    if (expect1 == target) {
        t_error("%s error input type is blank", __func__);
    }
}

/**
 * @tc.name      : wctype_0400
 * @tc.desc      : Test the return value when wctype input parameters are cntrl and error strings
 * @tc.level     : Level 0
 */
void wctype_0400(void)
{
    wctype_t expect0 = wctype("cntrl");
    wctype_t target = 4;
    if (expect0 != target) {
        t_error("%s expect type is not cntrl", __func__);
    }

    wctype_t expect1 = wctype("cntrll");
    if (expect1 == target) {
        t_error("%s error input type is cntrl", __func__);
    }
}

/**
 * @tc.name      : wctype_0500
 * @tc.desc      : Test the return value when wctype input parameters are digit and error strings
 * @tc.level     : Level 0
 */
void wctype_0500(void)
{
    wctype_t expect0 = wctype("digit");
    wctype_t target = 5;
    if (expect0 != target) {
        t_error("%s expect type is not digit", __func__);
    }

    wctype_t expect1 = wctype("digitt");
    if (expect1 == target) {
        t_error("%s error input type is digit", __func__);
    }
}

/**
 * @tc.name      : wctype_0600
 * @tc.desc      : Test the return value when wctype input parameters are graph and error strings
 * @tc.level     : Level 0
 */
void wctype_0600(void)
{
    wctype_t expect0 = wctype("graph");
    wctype_t target = 6;
    if (expect0 != target) {
        t_error("%s expect type is not graph", __func__);
    }

    wctype_t expect1 = wctype("graphh");
    if (expect1 == target) {
        t_error("%s error input type is graph", __func__);
    }
}

/**
 * @tc.name      : wctype_0700
 * @tc.desc      : Test the return value when wctype input parameters are lower and error strings
 * @tc.level     : Level 0
 */
void wctype_0700(void)
{
    wctype_t expect0 = wctype("lower");
    wctype_t target = 7;
    if (expect0 != target) {
        t_error("%s expect type is not lower", __func__);
    }

    wctype_t expect1 = wctype("lowerr");
    if (expect1 == target) {
        t_error("%s error input type is lower", __func__);
    }
}

/**
 * @tc.name      : wctype_0800
 * @tc.desc      : Test the return value when wctype input parameters are print and error strings
 * @tc.level     : Level 0
 */
void wctype_0800(void)
{
    wctype_t expect0 = wctype("print");
    wctype_t target = 8;
    if (expect0 != target) {
        t_error("%s expect type is not print", __func__);
    }

    wctype_t expect1 = wctype("printt");
    if (expect1 == target) {
        t_error("%s error input type is print", __func__);
    }
}

/**
 * @tc.name      : wctype_0900
 * @tc.desc      : Test the return value when wctype input parameters are punct and error strings
 * @tc.level     : Level 0
 */
void wctype_0900(void)
{
    wctype_t expect0 = wctype("punct");
    wctype_t target = 9;
    if (expect0 != target) {
        t_error("%s expect type is not punct", __func__);
    }

    wctype_t expect1 = wctype("punctt");
    if (expect1 == target) {
        t_error("%s error input type is punct", __func__);
    }
}

/**
 * @tc.name      : wctype_1000
 * @tc.desc      : Test the return value when wctype input parameters are space and error strings
 * @tc.level     : Level 0
 */
void wctype_1000(void)
{
    wctype_t expect0 = wctype("space");
    wctype_t target = 10;
    if (expect0 != target) {
        t_error("%s expect type is not space", __func__);
    }

    wctype_t expect1 = wctype("spacee");
    if (expect1 == target) {
        t_error("%s error input type is space", __func__);
    }
}

/**
 * @tc.name      : wctype_1100
 * @tc.desc      : Test the return value when wctype input parameters are upper and error strings
 * @tc.level     : Level 0
 */
void wctype_1100(void)
{
    wctype_t expect0 = wctype("upper");
    wctype_t target = 11;
    if (expect0 != target) {
        t_error("%s expect type is not upper", __func__);
    }

    wctype_t expect1 = wctype("upperr");
    if (expect1 == target) {
        t_error("%s error input type is upper", __func__);
    }
}

/**
 * @tc.name      : wctype_1200
 * @tc.desc      : Test the return value when wctype input parameters are xdigit and error strings
 * @tc.level     : Level 0
 */
void wctype_1200(void)
{
    wctype_t expect0 = wctype("xdigit");
    wctype_t target = 12;
    if (expect0 != target) {
        t_error("%s expect type is not xdigit", __func__);
    }

    wctype_t expect1 = wctype("xdigitt");
    if (expect1 == target) {
        t_error("%s error input type is xdigit", __func__);
    }
}

int main(int argc, char *argv[])
{
    wctype_0100();
    wctype_0200();
    wctype_0300();
    wctype_0400();
    wctype_0500();
    wctype_0600();
    wctype_0700();
    wctype_0800();
    wctype_0900();
    wctype_1000();
    wctype_1100();
    wctype_1200();
    return t_status;
}