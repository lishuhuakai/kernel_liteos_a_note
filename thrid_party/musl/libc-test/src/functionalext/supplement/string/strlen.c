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
 * @tc.name       :strlen_0100
 * @tc.desc       :Verify strlen is successed when src is "this is a test 1234~@#$%".
 * @tc.desc       :level 0
 */
void strlen_0100(void)
{
    char src[] = "this is a test 1234~@#$%";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0100", ret, srcLen);
}

/**
 * @tc.name       :strlen_0200
 * @tc.desc       :Verify strlen is successed when src is "测试1234~@#$%".
 * @tc.desc       :level 1
 */
void strlen_0200(void)
{
    char src[] = "测试1234~@#$%";
    int srcLen = sizeof(src) / sizeof(src[0]) - 1;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0200", ret, srcLen);
}

/**
 * @tc.name       :strlen_0300
 * @tc.desc       :Verify strlen is successed when src is "".
 * @tc.desc       :level 1
 */
void strlen_0300(void)
{
    char src[] = "";
    int srcLen = INIT_LEN;
    int ret = strlen(src);
    EXPECT_EQ("strlen_0300", ret, srcLen);
}

int main(void)
{
    strlen_0100();
    strlen_0200();
    strlen_0300();
    return t_status;
}
