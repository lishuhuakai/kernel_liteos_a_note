/*
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

#include <limits.h>
#include <locale.h>
#include <string.h>
#include <uchar.h>
#include "test.h"

/**
 * @tc.name      : c16rtomb_0100
 * @tc.desc      : Converts a variable-length 16-bit wide character representation to its narrow multibyte character
 *                 representation.
 * @tc.level     : Level 0
 */
void c16rtomb_0100(void)
{
    char bytes[MB_LEN_MAX];
    memset(bytes, 0, sizeof(bytes));
    size_t result = c16rtomb(bytes, L'h', NULL);
    if (result != 1U) {
        t_error("%s c16rtomb failed, result is %d\n", __func__, result);
    }
    if (bytes[0] != 'h') {
        t_error("%s bytes[0] is %c, not \'h\'\n", bytes[0]);
    }

    char *ret = setlocale(LC_CTYPE, "C.UTF-8");
    if (strcmp(ret, "C.UTF-8")) {
        t_error("%s setlocale failed\n", __func__);
    }
    uselocale(LC_GLOBAL_LOCALE);

    // 1-byte UTF-8.
    memset(bytes, 0, sizeof(bytes));
    result = c16rtomb(bytes, L'h', NULL);
    if (result != 1U) {
        t_error("%s c16rtomb failed, result is %d\n", __func__, result);
    }
    if (bytes[0] != 'h') {
        t_error("%s bytes[0] is %c, not \'h\'\n", bytes[0]);
    }

    // 2-byte UTF-8.
    memset(bytes, 0, sizeof(bytes));
    result = c16rtomb(bytes, 0x00a2, NULL);
    if (result != 2U) {
        t_error("%s c16rtomb failed, result is %d\n", __func__, result);
    }
    if (bytes[0] != '\xc2') {
        t_error("%s bytes[0] is %c, not \'\\xc2\'\n", bytes[0]);
    }
    if (bytes[1] != '\xa2') {
        t_error("%s bytes[0] is %c, not \'\\xa2\'\n", bytes[1]);
    }

    // 3-byte UTF-8.
    memset(bytes, 0, sizeof(bytes));
    result = c16rtomb(bytes, 0x20ac, NULL);
    if (result != 3U) {
        t_error("%s c16rtomb failed, result is %d\n", __func__, result);
    }
    if (bytes[0] != '\xe2') {
        t_error("%s bytes[0] is %c, not \'\\xe2\'\n", bytes[0]);
    }
    if (bytes[1] != '\x82') {
        t_error("%s bytes[0] is %c, not \'\\x82\'\n", bytes[1]);
    }
    if (bytes[2] != '\xac') {
        t_error("%s bytes[0] is %c, not \'\\xac\'\n", bytes[2]);
    }
}

/**
 * @tc.name      : c16rtomb_0200
 * @tc.desc      : When the s parameter is invalid, test the return value of the function.
 * @tc.level     : Level 1
 */
void c16rtomb_0200(void)
{
    size_t result = c16rtomb(NULL, L'\0', NULL);
    if (result != 1U) {
        t_error("%s c16rtomb failed, result is %d\n", __func__, result);
    }

    result = c16rtomb(NULL, L'h', NULL);
    if (result != 1U) {
        t_error("%s c16rtomb failed, result is %d\n", __func__, result);
    }
}

/**
 * @tc.name      : c16rtomb_0300
 * @tc.desc      : Test the return value of this function when c16 is equal to 0xdbea.
 * @tc.leve      : Level 1
 */
void c16rtomb_0300(void)
{
    char bytes[MB_LEN_MAX];
    memset(bytes, 0, sizeof(bytes));
    size_t result = c16rtomb(bytes, 0xdbea, NULL);
    if (result != 0U) {
        t_error("%s c16rtomb failed\n", __func__);
    }
}

/**
 * @tc.name      : c16rtomb_0400
 * @tc.desc      : Test the return value of this function when c16 is equal to 0xdfcd.
 * @tc.leve      : Level 1
 */
void c16rtomb_0400(void)
{
    char bytes[MB_LEN_MAX];
    memset(bytes, 0, sizeof(bytes));
    size_t result = c16rtomb(bytes, 0xdfcd, NULL);
    if (result != 4U) {
        t_error("%s c16rtomb failed\n", __func__);
    }

    if (bytes[0] != '\xf4') {
        t_error("%s bytes[0] is %c, not \'\\xf4\'\n", bytes[0]);
    }
    if (bytes[1] != '\x8a') {
        t_error("%s bytes[0] is %c, not \'\\x8a\'\n", bytes[1]);
    }
    if (bytes[2] != '\xaf') {
        t_error("%s bytes[0] is %c, not \'\\xaf\'\n", bytes[2]);
    }
    if (bytes[3] != '\x8d') {
        t_error("%s bytes[0] is %c, not \'\\x8d\'\n", bytes[3]);
    }
}

/**
 * @tc.name      : c16rtomb_0500
 * @tc.desc      : Test the return value of this function when c16 is equal to 0xdfcd.
 * @tc.leve      : Level 2
 */
void c16rtomb_0500(void)
{
    char bytes[MB_LEN_MAX];
    memset(bytes, 0, sizeof(bytes));
    size_t result = c16rtomb(bytes, 0xdfcd, NULL);
    if (result != (size_t)-1) {
        t_error("%s c16rtomb failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    c16rtomb_0100();
    c16rtomb_0200();
    c16rtomb_0300();
    c16rtomb_0400();
    c16rtomb_0500();
    return t_status;
}