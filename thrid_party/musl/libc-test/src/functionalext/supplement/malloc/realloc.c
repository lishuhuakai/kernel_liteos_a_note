/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include "functionalext.h"

#define SIZE_ALIGN (4 * sizeof(size_t))

const int32_t RELLOCTLEN = 100;
const int32_t INVALIDLEN = -100;

/**
 * @tc.name:      realloc_0100
 * @tc.desc:      Verify realloc process success when first args is null second args is 50.
 * @tc.level:     level 0.
 */
void realloc_0100(void)
{
    void *ret = realloc(NULL, RELLOCTLEN);
    EXPECT_PTRNE("realloc_0100", ret, NULL);
}

/**
 * @tc.name:      realloc_0200
 * @tc.desc:      Verify realloc process success when first args is null second args is 0.
 * @tc.level:     level 2
 */
void realloc_0200(void)
{
    void *ret = realloc(NULL, 0);
    EXPECT_PTRNE("realloc_0200", ret, NULL);
}

/**
 * @tc.name:      realloc_0300
 * @tc.desc:      Verify realloc process fail when first args is null second args is -100.
 * @tc.level:     level 2.
 */
void realloc_0300(void)
{
    void *ret = realloc(NULL, INVALIDLEN);
    EXPECT_PTREQ("realloc_0300", ret, NULL);
}

/**
 * @tc.name:      realloc_0400
 * @tc.desc:      Verify realloc process success when first args is not null second args is 200
 * @tc.level:     level 0.
 */
void realloc_0400(void)
{
    char *cptr = (char *)malloc(RELLOCTLEN);
    void *ret = realloc(cptr, RELLOCTLEN + RELLOCTLEN);
    EXPECT_PTRNE("realloc_0400", ret, NULL);
}

/**
 * @tc.name:      realloc_0500
 * @tc.desc:      Verify realloc process success when first args is not null second args is 100
 * @tc.level:     level  1.
 */
void realloc_0500(void)
{
    char *cptr = (char *)malloc(RELLOCTLEN);
    void *ret = realloc(cptr, RELLOCTLEN);
    EXPECT_PTRNE("realloc_0500", ret, NULL);
}

/**
 * @tc.name:      realloc_0600
 * @tc.desc:      Verify realloc process success when first args is not null second args is 50
 * @tc.level:     level 1.
 */
void realloc_0600(void)
{
    char *cptr = (char *)malloc(RELLOCTLEN);
    void *ret = realloc(cptr, RELLOCTLEN / 2);
    EXPECT_PTRNE("realloc_0600", ret, NULL);
}

int main(void)
{
    realloc_0100();
    realloc_0200();
    realloc_0300();
    realloc_0400();
    realloc_0500();
    realloc_0600();
    return t_status;
}
