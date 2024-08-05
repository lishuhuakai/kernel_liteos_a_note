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

#include <stdlib.h>
#include <string.h>
#include "functionalext.h"

const int INIT_LEN = 0;

/**
 * @tc.name       :memchr_0100
 * @tc.desc       :Verify memchr is successed and return 0.
 * @tc.level      :level 0.
 */
void memchr_0100(void)
{
    const char *srcstring = "this is a unittest";
    char fitch = 'u';
    char *findch = (char *)memchr(srcstring, fitch, strlen(srcstring));
    if (findch) {
        EXPECT_EQ("memchr_0100", fitch, findch[0]);
        EXPECT_STREQ("memchr_0100", "unittest", findch);
    } else {
        EXPECT_PTRNE("memchr_0100", findch, NULL);
    }
}

/**
 * @tc.name       :memchr_0200
 * @tc.desc       :Verify memchr is failed and return NULL.
 * @tc.level      :level 1.
 */
void memchr_0200(void)
{
    const char *srcstring = "this is a unittest";
    char *findch = (char *)memchr(srcstring, 'w', strlen(srcstring));
    EXPECT_PTREQ("memchr_0200", findch, NULL);
}

/**
 * @tc.name       :memchr_0300
 * @tc.desc       :Verify memchr is failed when first param src is invalid.
 * @tc.level      :level 2.
 */
void memchr_0300(void)
{
    const char *srcstring = "";
    char *findch = (char *)memchr(srcstring, 'w', strlen(srcstring));
    EXPECT_PTREQ("memchr_0300", findch, NULL);
}

/**
 * @tc.name       :memchr_0400
 * @tc.desc       :Verify memchr is failed when second param c is invalid.
 * @tc.level      :level 2.
 */
void memchr_0400(void)
{
    const char *srcstring = "this is a unittest";
    char *findch = (char *)memchr(srcstring, '\0', strlen(srcstring));
    EXPECT_PTREQ("memchr_0400", findch, NULL);
}

/**
 * @tc.name       :memchr_0500
 * @tc.desc       :Verify memchr is failed when third param n is invalid.
 * @tc.level      :level 2.
 */
void memchr_0500(void)
{
    const char *srcstring = "this is a unittest";
    char *findch = (char *)memchr(srcstring, 'u', INIT_LEN);
    EXPECT_PTREQ("memchr_0500", findch, NULL);
}

int main(void)
{
    memchr_0100();
    memchr_0200();
    memchr_0300();
    memchr_0400();
    memchr_0500();
    return t_status;
}
