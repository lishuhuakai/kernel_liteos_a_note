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
#include "functionalext.h"

const int32_t BUF_SIZE = 1024;

/**
 * @tc.name      : getsubopt_0100
 * @tc.desc      : Verify getsubopt process success find subparam
 * @tc.level     : Level 0
 */
void getsubopt_0100(void)
{
    char *tokens[] = {"getsubopt", "test", NULL};
    char buf[BUF_SIZE];
    strcpy(buf, "getsubopt=0100,unknown");
    char *subopts = buf;
    char *value = NULL;
    EXPECT_EQ("getsubopt_0100", 0, getsubopt(&subopts, tokens, &value));
    EXPECT_STREQ("getsubopt_0100", "0100", value);
}

/**
 * @tc.name      : getsubopt_0200
 * @tc.desc      : Verify getsubopt process fail not find subparam
 * @tc.level     : Level 2
 */
void getsubopt_0200(void)
{
    char *tokens[] = {"getsubopt", "test", NULL};
    char buf[BUF_SIZE];
    strcpy(buf, "0200=unknown, test");
    char *subopts = buf;
    char *value = NULL;
    EXPECT_EQ("getsubopt_0100", -1, getsubopt(&subopts, tokens, &value));
}

int main(void)
{
    getsubopt_0100();
    getsubopt_0200();
    return t_status;
}
