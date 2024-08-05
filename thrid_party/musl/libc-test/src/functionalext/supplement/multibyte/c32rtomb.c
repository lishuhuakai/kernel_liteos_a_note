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

#include <errno.h>
#include <limits.h>
#include <locale.h>
#include <string.h>
#include <uchar.h>
#include "test.h"

/**
 * @tc.name      : c32rtomb_0100
 * @tc.desc      :
 * @tc.leve      : Level 0
 */
void c32rtomb_0100(void)
{
    size_t result = c32rtomb(NULL, L'\0', NULL);
    if (result != 1U) {
        t_error("%s c32rtomb failed\n", __func__);
    }

    result = c32rtomb(NULL, L'h', NULL);
    if (result != 1U) {
        t_error("%s c32rtomb failed\n", __func__);
    }
}

/**
 * @tc.name      : c32rtomb_0200
 * @tc.desc      :
 * @tc.leve      : Level 0
 */
void c32rtomb_0200(void)
{
    char bytes[MB_LEN_MAX];
    memset(bytes, 1, sizeof(bytes));

    size_t result = c32rtomb(bytes, L'\0', NULL);
    if (result != 1U) {
        t_error("%s c32rtomb failed\n", __func__);
    }
    if (bytes[0] != '\0') {
        t_error("%s bytes[0] failed\n", __func__);
    }
    if (bytes[1] != '\x01') {
        t_error("%s bytes[1] failed\n", __func__);
    }

    memset(bytes, 0, sizeof(bytes));
    result = c32rtomb(bytes, L'h', NULL);
    if (result != 1U) {
        t_error("%s c32rtomb failed\n", __func__);
    }
    if (bytes[0] != 'h') {
        t_error("%s bytes[0] failed\n", __func__);
    }
}

/**
 * @tc.name      : c32rtomb_0300
 * @tc.desc      :
 * @tc.leve      : Level 0
 */
void c32rtomb_0300(void)
{
    char bytes[MB_LEN_MAX];

    char *ret = setlocale(LC_CTYPE, "C.UTF-8");
    if (strcmp(ret, "C.UTF-8")) {
        t_error("%s setlocale failed\n", __func__);
    }
    uselocale(LC_GLOBAL_LOCALE);

    // 1-byte UTF-8.
    memset(bytes, 0, sizeof(bytes));
    size_t result = c32rtomb(bytes, L'h', NULL);
    if (result != 1U) {
        t_error("%s c32rtomb failed\n", __func__);
    }
    if (bytes[0] != 'h') {
        t_error("%s bytes[0] failed\n", __func__);
    }
    // 2-byte UTF-8.
    memset(bytes, 0, sizeof(bytes));
    result = c32rtomb(bytes, 0x00a2, NULL);
    if (result != 2U) {
        t_error("%s c32rtomb failed\n", __func__);
    }
    if (bytes[0] != '\xc2') {
        t_error("%s bytes[0] failed\n", __func__);
    }
    if (bytes[1] != '\xa2') {
        t_error("%s bytes[1] failed\n", __func__);
    }
    // 3-byte UTF-8.
    memset(bytes, 0, sizeof(bytes));
    result = c32rtomb(bytes, 0x20ac, NULL);
    if (result != 3U) {
        t_error("%s c32rtomb failed\n", __func__);
    }
    if (bytes[0] != '\xe2') {
        t_error("%s bytes[0] failed\n", __func__);
    }
    if (bytes[1] != '\x82') {
        t_error("%s bytes[1] failed\n", __func__);
    }
    if (bytes[2] != '\xac') {
        t_error("%s bytes[2] failed\n", __func__);
    }
    // 4-byte UTF-8.
    memset(bytes, 0, sizeof(bytes));
    result = c32rtomb(bytes, 0x24b62, NULL);
    if (result != 4U) {
        t_error("%s c32rtomb failed\n", __func__);
    }
    if (bytes[0] != '\xf0') {
        t_error("%s bytes[0] failed\n", __func__);
    }
    if (bytes[1] != '\xa4') {
        t_error("%s bytes[1] failed\n", __func__);
    }
    if (bytes[2] != '\xad') {
        t_error("%s bytes[2] failed\n", __func__);
    }
    if (bytes[3] != '\xa2') {
        t_error("%s bytes[3] failed\n", __func__);
    }
}

/**
 * @tc.name      : c32rtomb_0400
 * @tc.desc      :
 * @tc.leve      : Level 2
 */
void c32rtomb_0400(void)
{
    char bytes[MB_LEN_MAX];
    memset(bytes, 0, sizeof(bytes));
    size_t result = c32rtomb(bytes, 0xffffffff, NULL);
    if (result != (size_t)-1) {
        t_error("%s c32rtomb should be failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    c32rtomb_0100();
    c32rtomb_0200();
    c32rtomb_0300();
    c32rtomb_0400();
    return t_status;
}