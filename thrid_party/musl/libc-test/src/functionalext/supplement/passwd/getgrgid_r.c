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
#include <grp.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : getgrgid_r_0100
 * @tc.desc      :
 * @tc.level     : Level 0
 */
void getgrgid_r_0100(void)
{
    errno = 0;
    char buf[512];
    gid_t gid = 0;

    struct group *grp;
    struct group grp_storage;
    const char *group_name = "root";

    int result = getgrgid_r(gid, &grp_storage, buf, sizeof(buf), &grp);
    if (result != 0) {
        t_error("%s getgrgid_r failed\n", __func__);
    }
    if (errno != 0) {
        t_error("%s errno should be zero\n", __func__);
    }

    if (!grp) {
        t_error("%s failed, grp is NULL\n", __func__);
    }
    if (strcmp(group_name, grp->gr_name)) {
        t_error("%s grp->gr_name is %s\n", __func__, grp->gr_name);
    }
    if (gid != grp->gr_gid) {
        t_error("%s gr_gid is %d\n", __func__, grp->gr_gid);
    }
    if (!grp->gr_mem) {
        t_error("%s grp->gr_mem is NULL\n", __func__);
    }
}

/**
 * @tc.name      : getgrgid_r_0200
 * @tc.desc      : Invalid parameter test
 * @tc.level     : Level 2
 */
void getgrgid_r_0200(void)
{
    char buf[512];
    struct group *grp;
    struct group grp_storage;

    int result = getgrgid_r(-1, &grp_storage, buf, sizeof(buf), &grp);
    if (result) {
        t_error("%s getgrgid_r should be failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    getgrgid_r_0100();
    getgrgid_r_0200();
    return t_status;
}