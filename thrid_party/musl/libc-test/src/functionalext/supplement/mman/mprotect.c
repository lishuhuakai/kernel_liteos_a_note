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

#include <stdlib.h>
#include <sys/mman.h>
#include "functionalext.h"

const int SUCCESS = 0;
const int FAILED = -1;

/**
 * @tc.name      : mprotect_0100
 * @tc.desc      : The port parameter is PROT_READ,which can set the protection attribute of memory area.
 * @tc.level     : Level 0
 */
void mprotect_0100(void)
{
    size_t align = getpagesize();
    void *buffer = memalign(align, 8 * align);
    int ret = mprotect(buffer, getpagesize(), PROT_READ);
    EXPECT_EQ("mprotect_0100", ret, SUCCESS);
}

/**
 * @tc.name      : mprotect_0200
 * @tc.desc      : The port parameter is PROT_WRITE,which can set the protection attribute of memory area.
 * @tc.level     : Level 0
 */
void mprotect_0200(void)
{
    size_t align = getpagesize();
    void *buffer = memalign(align, 8 * align);
    int ret = mprotect(buffer, getpagesize(), PROT_WRITE);
    EXPECT_EQ("mprotect_0200", ret, SUCCESS);
}

/**
 * @tc.name      : mprotect_0300
 * @tc.desc      : The port parameter is PROT_EXEC,which can set the protection attribute of memory area.
 * @tc.level     : Level 0
 */
void mprotect_0300(void)
{
    size_t align = getpagesize();
    void *buffer = memalign(align, 8 * align);
    int ret = mprotect(buffer, getpagesize(), PROT_EXEC);
    EXPECT_EQ("mprotect_0300", ret, SUCCESS);
}

/**
 * @tc.name      : mprotect_0400
 * @tc.desc      : The port parameter is PROT_NONE,which can set the protection attribute of memory area.
 * @tc.level     : Level 0
 */
void mprotect_0400(void)
{
    size_t align = getpagesize();
    void *buffer = memalign(align, 8 * align);
    int ret = mprotect(buffer, getpagesize(), PROT_NONE);
    EXPECT_EQ("mprotect_0400", ret, SUCCESS);
}

/**
 * @tc.name      : mprotect_0500
 * @tc.desc      : The port parameter is PROT_READ|PROT_WRITE,which can set the protection attribute of memory area.
 * @tc.level     : Level 1
 */
void mprotect_0500(void)
{
    size_t align = getpagesize();
    void *buffer = memalign(align, 8 * align);
    int ret = mprotect(buffer, getpagesize(), PROT_READ | PROT_WRITE);
    EXPECT_EQ("mprotect_0500", ret, SUCCESS);
}

/**
 * @tc.name      : mprotect_0600
 * @tc.desc      : The start parameter is not an integer multiple of the page,
 *                 and the protection attribute of the memory area cannot be set.
 * @tc.level     : Level 2
 */
void mprotect_0600(void)
{
    int ret = mprotect((void *)0x0001, getpagesize(), PROT_READ);
    EXPECT_EQ("mprotect_0600", ret, FAILED);
}

int main(int argc, char *argv[])
{
    mprotect_0100();
    mprotect_0200();
    mprotect_0300();
    mprotect_0400();
    mprotect_0500();
    mprotect_0600();

    return t_status;
}