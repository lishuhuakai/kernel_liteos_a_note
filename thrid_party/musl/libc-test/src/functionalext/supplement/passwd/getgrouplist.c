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

#include <grp.h>
#include <malloc.h>
#include <pwd.h>
#include <stdio.h>
#include "test.h"

static int server_ngroups;
static gid_t *server_groups;

/*
 * @tc.name      : getgrouplist_0100
 * @tc.desc      : Get list of groups to which a user belongs
 * @tc.level     : Level 0
 */
void getgrouplist_0100()
{
    int result;
    struct group *gr;
    const char *server_user = "root";

    struct passwd *pwd = getpwnam(server_user);
    if (!pwd) {
        t_error("%s getpwnam failed\n", __func__);
    }

    gid_t server_gid = pwd->pw_gid;

    result = getgrouplist(server_user, server_gid, NULL, &server_ngroups);
    if (result == 0) {
        t_error("%s getgrouplist should be failed\n", __func__);
    }

    server_groups = (gid_t *)malloc(server_ngroups * sizeof(gid_t));
    result = getgrouplist(server_user, server_gid, server_groups, &server_ngroups);
    if (result == -1) {
        t_error("%s getgrouplist failed\n", __func__);
    }

    for (int i = 0; i < server_ngroups; i++) {
        printf("%d", server_groups[i]);
        gr = getgrgid(server_groups[i]);
        if (gr != NULL) {
            printf(" (%s)", gr->gr_name);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    getgrouplist_0100();
    return t_status;
}