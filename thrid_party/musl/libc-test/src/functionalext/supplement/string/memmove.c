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

char one[50];
char two[50];

/**
 * @tc.name      : memmove_0100
 * @tc.desc      : The source address and the destination address are equal, and no copying is performed.
 * @tc.level     : Level 1
 */
void memmove_0100(void)
{
    memset(one, 0, sizeof(one));

    strcpy(one, "abcdefgh");
    memmove(one, one, 9);
    if (strcmp(one, "abcdefgh")) {
        t_error("%s memmove failed\n", __func__);
    }
}

/**
 * @tc.name      : memmove_0200
 * @tc.desc      : The source address and the destination address do not overlap, copy it.
 * @tc.level     : Level 0
 */
void memmove_0200(void)
{
    memset(one, 0, sizeof(one));
    memset(two, 0, sizeof(two));

    strcpy(one, "helloWorld");
    strcpy(two, "foo");
    memmove(two, one, 11);
    if (strcmp(one, "helloWorld")) {
        t_error("%s memmove failed\n", __func__);
    }
    if (strcmp(two, "helloWorld")) {
        t_error("%s memmove failed\n", __func__);
    }
}

/**
 * @tc.name      : memmove_0300
 * @tc.desc      : The first address of dest is less than the first address of src, and forward copy is performed.
 * @tc.level     : Level 1
 */
void memmove_0300(void)
{
    memset(one, 0, sizeof(one));

    strcpy(one, "abcdefgh");
    memmove(one + 1, one + 2, 7);
    if (strcmp(one, "acdefgh")) {
        t_error("%s memmove failed\n", __func__);
    }
}

/**
 * @tc.name      : memmove_0400
 * @tc.desc      : The first address of dest is greater than the first address of src, and reverse copy is performed.
 * @tc.level     : Level 1
 */
void memmove_0400(void)
{
    memset(one, 0, sizeof(one));

    strcpy(one, "abcdefgh");
    memmove(one + 1, one, 9);
    if (strcmp(one, "aabcdefgh")) {
        t_error("%s memmove failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    memmove_0100();
    memmove_0200();
    memmove_0300();
    memmove_0400();

    return t_status;
}