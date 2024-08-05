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

#include <grp.h>
#include <stdio.h>
#include <unistd.h>
#include "test.h"

#define GROUPS_MAX 2

/**
 * @tc.name      : setgroups_0100
 * @tc.desc      : Sets the supplementary group IDs for the calling process.
 * @tc.level     : Level 0
 */
void setgroups_0100(void)
{
    gid_t groups[GROUPS_MAX];
    int count = GROUPS_MAX;
    gid_t groupslist[GROUPS_MAX];
    int size = GROUPS_MAX;
    groups[0] = 0;
    groups[1] = 1;
    int result = setgroups(count, groups);
    if (result == -1) {
        t_error("%s setgroups failed", __func__);
    }
    int num = getgroups(size, groupslist);
    if (groupslist[0] != groups[0] && groupslist[1] != groups[1]) {
        t_error("%s setgroups invalid", __func__);
    }
}

/**
 * @tc.name      : setgroups_0200
 * @tc.desc      : Drop all of its supplementary groups.
 * @tc.level     : Level 0
 */
void setgroups_0200(void)
{
    gid_t groups[GROUPS_MAX];
    int count = GROUPS_MAX;
    gid_t groupslist[GROUPS_MAX];
    int size = GROUPS_MAX;
    groups[0] = 0;
    groups[1] = 1;
    int result = setgroups(count, groups);
    if (result == -1) {
        t_error("%s setgroups failed", __func__);
    }
    if (setgroups(0, NULL) == -1) {
        t_error("%s drop all of its supplementary groups failed", __func__);
        return;
    }
    int num = getgroups(size, groupslist);
    if (num != 0) {
        t_error("%s drop failed", __func__);
    }
}

int main(int argc, char *argv[])
{
    setgroups_0100();
    setgroups_0200();
    return t_status;
}