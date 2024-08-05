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
#include <locale.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : strxfrm_0100
 * @tc.desc      : Put all characters in src into dest
 * @tc.level     : Level 0
 */
void strxfrm_0100(void)
{
    char desp[20] = {};
    char *str = "abcdef";
    int want = 6;
    size_t result = strxfrm(desp, str, want + 1);
    if (result != want) {
        t_error("%s strxfrm get size is %d is not want %d\n", __func__, result, want);
    }
    if (strcmp(desp, str) != 0) {
        t_error("%s strxfrm get desp is %s is not want %s\n", __func__, desp, str);
    }
}

/**
 * @tc.name      : strxfrm_0200
 * @tc.desc      : Put all characters in src into dest, and set n to be the same as the length of src
 * @tc.level     : Level 1
 */
void strxfrm_0200(void)
{
    char desp[20] = {};
    char *str = "abcdef";
    int want = 6;
    size_t result = strxfrm(desp, str, want);
    if (result != want) {
        t_error("%s strxfrm get size is %d is not want %d\n", __func__, result, want);
    }
    if (strcmp(desp, str) == 0) {
        t_error("%s strxfrm get desp is %s is not want %s\n", __func__, desp, str);
    }
}

/**
 * @tc.name      : strxfrm_0300
 * @tc.desc      : Put all characters in src into dest, set n less than the length of src
 * @tc.level     : Level 1
 */
void strxfrm_0300(void)
{
    char desp[20] = {};
    char *str = "abcdef";
    int want = 3;
    size_t result = strxfrm(desp, str, want);
    if (result != strlen(str)) {
        t_error("%s strxfrm get size is %d is not want %d\n", __func__, result, strlen(str));
    }
    if (strcmp(desp, str) == 0) {
        t_error("%s strxfrm get desp is %s is not want %s\n", __func__, desp, str);
    }
}

/**
 * @tc.name      : strxfrm_0400
 * @tc.desc      : Test empty string dest array size is less than src
 * @tc.level     : Level 1
 */
void strxfrm_0400(void)
{
    char desp[5];
    char *str = "abcdef";
    int want = 6;
    size_t result = strxfrm(desp, str, want + 1);
    if (result != want) {
        t_error("%s strxfrm get size is %d is not want %d\n", __func__, result, want);
    }
    if (strcmp(desp, str) != 0) {
        t_error("%s strxfrm get desp is %s is not want %s\n", __func__, desp, str);
    }
}

/**
 * @tc.name      : strxfrm_0500
 * @tc.desc      : Test to copy the content of src to the string dest of the existing content
 * @tc.level     : Level 1
 */
void strxfrm_0500(void)
{
    char desp[10];
    strcpy(desp, "123");
    char *str = "abcdef";
    int want = 6;
    size_t result = strxfrm(desp, str, want + 1);
    if (result != want) {
        t_error("%s strxfrm get size is %d is not want %d\n", __func__, result, want);
    }
    if (strcmp(desp, str) != 0) {
        t_error("%s strxfrm get desp is %s is not want %s\n", __func__, desp, str);
    }
}

/**
 * @tc.name      : strxfrm_0600
 * @tc.desc      : The test copies the content of src to dest whose length is greater than src through strxfrm
 * @tc.level     : Level 1
 */
void strxfrm_0600(void)
{
    char desp[10];
    strcpy(desp, "1234567");
    char *str = "abcdef";
    int want = 6;
    size_t result = strxfrm(desp, str, want + 1);
    if (result != want) {
        t_error("%s strxfrm get size is %d is not want %d\n", __func__, result, want);
    }
    if (strcmp(desp, str) != 0) {
        t_error("%s strxfrm get desp is %s is not want %s\n", __func__, desp, str);
    }
}

/**
 * @tc.name      : strxfrm_0700
 * @tc.desc      : Test move over src
 * @tc.level     : Level 1
 */
void strxfrm_0700(void)
{
    char desp[10];
    strcpy(desp, "1234567");
    char *str = "abcdef";
    int move = 10;
    size_t result = strxfrm(desp, str, move);
    int want = 6;
    if (result != want) {
        t_error("%s strxfrm get size is %d is not want %d\n", __func__, result, want);
    }
    if (strcmp(desp, str) != 0) {
        t_error("%s strxfrm get desp is %s is not want %s\n", __func__, desp, str);
    }
}

int main(int argc, char *argv[])
{
    strxfrm_0100();
    strxfrm_0200();
    strxfrm_0300();
    strxfrm_0400();
    strxfrm_0500();
    strxfrm_0600();
    strxfrm_0700();
    return t_status;
}