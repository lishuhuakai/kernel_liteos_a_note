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

#include <malloc.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : memalign_0100
 * @tc.desc      : Each parameter is valid, and align is obtained by getpagesize(), which can allocate a memory block
 *                 whose size is specified by len and whose address is a multiple of align.
 * @tc.level     : Level 0
 */
void memalign_0100(void)
{
    int align = getpagesize();
    EXPECT_NE("memalign_0100", align, -1);
    void *buff = (void *)memalign(align, 8 * align);
    EXPECT_TRUE("memalign_0100", buff != NULL);
    free(buff);
    buff = NULL;
}

/**
 * @tc.name      : memalign_0200
 * @tc.desc      : Each parameter is valid, and it can allocate a memory block with the size specified by
 *                 len and the address is a multiple of align
 * @tc.level     : Level 1
 */
void memalign_0200(void)
{
    int align = 16;
    void *buff = NULL;
    buff = (void *)memalign(align, 8 * align);
    EXPECT_TRUE("memalign_0200", buff != NULL);
    free(buff);
    buff = NULL;
}

/**
 * @tc.name      : memalign_0300
 * @tc.desc      : The align parameter is invalid, and a memory block whose size is specified
 *                 by len and whose address is a multiple of align cannot be allocated
 * @tc.level     : Level 2
 */
void memalign_0300(void)
{
    int align = getpagesize() - 1;
    EXPECT_NE("memalign_0300", align, -1);
    void *buff = (void *)memalign(align, 128);
    EXPECT_EQ("memalign_0300", buff, NULL);
    free(buff);
    buff = NULL;
}

/**
 * @tc.name      : memalign_0400
 * @tc.desc      : The len parameter is invalid, and a memory block whose size is specified by
 *                 len and whose address is a multiple of align cannot be allocated
 * @tc.level     : Level 2
 */
void memalign_0400(void)
{
    int align = getpagesize();
    EXPECT_NE("memalign_0400", align, -1);
    void *buff = (void *)memalign(align, SIZE_MAX - align + 1);
    EXPECT_EQ("memalign_0400", buff, NULL);
    free(buff);
    buff = NULL;
}

/**
 * @tc.name      : memalign_0500
 * @tc.desc      : Insufficient memory space to allocate a memory block
 *                 with a size specified by len and an address that is a multiple of align
 * @tc.level     : Level 2
 */
void memalign_0500(void)
{
    int align = (4 * sizeof(size_t)) + 1;
    void *buff = (void *)memalign(align, align * 1024);
    EXPECT_EQ("memalign_0500", buff, NULL);
    free(buff);
    buff = NULL;
}

int main(int argc, char *argv[])
{
    memalign_0100();
    memalign_0200();
    memalign_0300();
    memalign_0400();
    memalign_0500();

    return t_status;
}
