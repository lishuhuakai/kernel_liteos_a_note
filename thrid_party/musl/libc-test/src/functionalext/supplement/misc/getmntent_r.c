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
#include <pthread.h>
#include <stdbool.h>
#include "functionalext.h"

static pthread_barrier_t g_barrier;
#define WAIT() pthread_barrier_wait(&g_barrier)

void *getmntent_rOne()
{
    char c[1000];
    char str[100];
    FILE *fptr = NULL;
    char strings[1024];
    if ((fptr = fopen("/proc/mounts", "r")) == NULL) {
        t_error("%s Error! fopen failed\n", __func__);
    }
    fscanf(fptr, "%[^\n]", c);
    fclose(fptr);
    sscanf(c, "%[^ ]", str);
    struct mntent *m = NULL;
    struct mntent mntbuf;
    memset(&m, 0x00, sizeof(m));
    memset(&strings[0], 0x00, sizeof(strings));
    FILE *ffp = setmntent("/proc/mounts", "r");
    EXPECT_PTRNE("getmntent_r_0100", ffp, NULL);

    WAIT();
    m = getmntent_r(ffp, &mntbuf, strings, sizeof(strings));
    EXPECT_TRUE("getmntent_r_0100", m != NULL);
    EXPECT_EQ("getmntent_r_0100", strcmp(m->mnt_fsname, str), 0);
    EXPECT_EQ("getmntent_r_0100", strcmp(mntbuf.mnt_fsname, str), 0);

    endmntent(ffp);
    return m;
}

/**
 * @tc.name      : getmntent_r_0100
 * @tc.desc      : Multiple threads simultaneously read file information.
 * @tc.level     : Level 0
 */
void getmntent_r_0100(void)
{
    pthread_t srv;
    pthread_t cli;
    int ret = pthread_barrier_init(&g_barrier, 0, 2);
    EXPECT_EQ("getmntent_r_0100", 0, ret);
    ret = pthread_create(&srv, NULL, getmntent_rOne, NULL);
    EXPECT_EQ("getmntent_r_0100", 0, ret);
    ret = pthread_create(&cli, NULL, getmntent_rOne, NULL);
    EXPECT_EQ("getmntent_r_0100", 0, ret);
    ret = pthread_join(cli, NULL);
    EXPECT_EQ("getmntent_r_0100", 0, ret);
    ret = pthread_join(srv, NULL);
    EXPECT_EQ("getmntent_r_0100", 0, ret);
    ret = pthread_barrier_destroy(&g_barrier);
    EXPECT_EQ("getmntent_r_0100", 0, ret);
}

/**
 * @tc.name      : getmntent_r_0200
 * @tc.desc      : The f parameter is invalid, reading the file information failed.
 * @tc.level     : Level 2
 */
void getmntent_r_0200(void)
{
    char c[1000];
    char str[100];
    char strings[1024];

    FILE *fptr = fopen("/data/getmntent.txt", "w");
    EXPECT_TRUE("getmntent_r_0200", fptr != NULL);
    fclose(fptr);

    struct mntent *m = NULL;
    struct mntent mntbuf;
    memset(&m, 0x00, sizeof(m));
    memset(&strings[0], 0x00, sizeof(strings));
    FILE *ffp = setmntent("/data/getmntent.txt", "r");
    EXPECT_PTRNE("getmntent_r_0200", ffp, NULL);
    m = getmntent_r(ffp, &mntbuf, strings, sizeof(strings));
    EXPECT_EQ("getmntent_r_0200", m, NULL);
    endmntent(ffp);
    remove("/data/getmntent.txt");
}

int main(int argc, char *argv[])
{
    getmntent_r_0100();
    getmntent_r_0200();

    return t_status;
}