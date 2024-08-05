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

#include <mntent.h>
#include <stdbool.h>
#include "functionalext.h"

/**
 * @tc.name      : getmntent_0100
 * @tc.desc      : The parameters are valid and the file information can be read.
 * @tc.level     : Level 0
 */
void getmntent_0100(void)
{
    char c[1000];
    char str[100];
    FILE *fptr = NULL;
    if ((fptr = fopen("/proc/mounts", "r")) == NULL) {
        t_error("%s Error! fopen failed\n", __func__);
    }
    fscanf(fptr, "%[^\n]", c);
    fclose(fptr);
    sscanf(c, "%[^ ]", str);
    struct mntent *m = NULL;
    FILE *ffp = setmntent("/proc/mounts", "r");
    EXPECT_PTRNE("getmntent_0100", ffp, NULL);
    m = getmntent(ffp);
    EXPECT_TRUE("getmntent_0100", m != NULL);
    EXPECT_EQ("getmntent_0100", strcmp(m->mnt_fsname, str), 0);

    endmntent(ffp);
}

/**
 * @tc.name      : getmntent_0200
 * @tc.desc      : Invalid parameter, failed to read file information.
 * @tc.level     : Level 2
 */
void getmntent_0200(void)
{
    char c[1000];
    char *str = "This is a test";
    FILE *fp = fopen("/data/getmntent.txt", "w");
    EXPECT_PTRNE("getmntent_0200", fp, NULL);
    fputs(str, fp);
    fclose(fp);
    struct mntent *m = NULL;
    FILE *ffp = setmntent("/data/getmntent.txt", "r");
    EXPECT_PTRNE("getmntent_0200", ffp, NULL);

    m = getmntent(ffp);
    EXPECT_EQ("getmntent_0200", m, NULL);
    endmntent(ffp);
    remove("/data/getmntent.txt");
}

int main(void)
{
    getmntent_0100();
    getmntent_0200();
    return t_status;
}