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

#include <mntent.h>
#include <fcntl.h>
#include "functionalext.h"

const int SIZE = 10;

/**
 * @tc.name      : hasmntopt_0100
 * @tc.desc      : Verify hasmntopt process success
 * @tc.level     : Level 0
 */
void hasmntopt_0100(void)
{
    char *mnt_opts = "test_hasmntopt";
    struct mntent ent;
    memset(&ent, 0, sizeof(ent));
    ent.mnt_opts = mnt_opts;
    char *ret = hasmntopt(&ent, "hasmntopt");
    EXPECT_STREQ("hasmntopt_0100", ret, "hasmntopt");
}

/**
 * @tc.name      : hasmntopt_0200
 * @tc.desc      : Verify hasmntopt process fail. Because opt not in mnt_opts
 * @tc.level     : Level 2
 */
void hasmntopt_0200(void)
{
    char *mnt_opts = "test_hasmntopt";
    struct mntent ent;
    memset(&ent, 0, sizeof(ent));
    ent.mnt_opts = mnt_opts;
    char *ret = hasmntopt(&ent, "0200");
    EXPECT_PTREQ("hasmntopt_0100", ret, NULL);
}

int main(void)
{
    hasmntopt_0100();
    hasmntopt_0200();
    return t_status;
}
