/*
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

#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : getgrgid_0100
 * @tc.desc      : Verify that the specified GID can be retrieved from the group file (parameter valid)
 * @tc.level     : Level 0
 */
void getgrgid_0100(void)
{
    errno = 0;
    const char *group_name = "root";
    gid_t gid = 0;

    struct group *grp = getgrgid(gid);
    if (errno != 0) {
        t_error("%s failed, errno is %d\n", __func__, errno);
    }
    if (!grp) {
        t_error("%s getgrgid failed\n", __func__);
    }
    if (strcmp(group_name, grp->gr_name)) {
        t_error("%s failed, grp->gr_name is %s\n", __func__, grp->gr_name);
    }
    if (grp->gr_gid != gid) {
        t_error("%s failed, grp->gr_gid is %d\n", __func__, grp->gr_gid);
    }
    if (grp->gr_mem == NULL) {
        t_error("%s failed, grp->gr_mem is NULL\n", __func__);
    }
}

/**
 * @tc.name      : getgrgid_0200
 * @tc.desc      : Verify that data with the specified GID cannot be retrieved from the group file
 *                 (gid argument is invalid)
 * @tc.level     : Level 2
 */
void getgrgid_0200(void)
{
    struct group *data = getgrgid(-1);
    if (data != NULL) {
        t_error("%s data should be NULL", __func__);
    }
}

int main(int argc, char *argv[])
{
    getgrgid_0100();
    getgrgid_0200();
    return t_status;
}