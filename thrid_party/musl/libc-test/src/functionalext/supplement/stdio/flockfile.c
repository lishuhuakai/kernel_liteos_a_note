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

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"

static FILE *fp;
const char *path = "/data/test.txt";

void *do_child_one(void *a)
{
    puts("start do_child_one");

    flockfile(stdout);

    puts("after first flockfile");

    flockfile(stdout);

    puts("foo");

    funlockfile(stdout);

    puts("after first funlockfile");

    funlockfile(stdout);

    puts("all done");
    return a;
}

void *do_child_two(void *p)
{
    char buf[64];

    flockfile(fp);

    if (fseek(fp, 0L, SEEK_SET) == -1) {
        t_error("%s fseek failed\n", __func__);
    }
    int ret = fread(buf, 64, 1, fp);

    int count = atoi(buf);
    ++count;
    sprintf(buf, "%d", count);
    if (fseek(fp, 0L, SEEK_SET) == -1) {
        t_error("%s fseek failed\n", __func__);
    }
    ret = fwrite(buf, strlen(buf), 1, fp);

    funlockfile(fp);
    return NULL;
}

/**
 * @tc.name      : flockfile_0100
 * @tc.desc      : Acquire for a thread ownership of a (FILE *) object (stdout)
 * @tc.level     : Level 0
 */
void flockfile_0100(void)
{
    pthread_t th;

    if (pthread_create(&th, NULL, do_child_one, NULL) != 0) {
        t_error("%s pthread_create failed\n", __func__);
    }

    void *result;
    if (pthread_join(th, &result) != 0) {
        t_error("%s pthread_join failed\n", __func__);
    } else if (result != NULL) {
        t_error("%s wrong return value: %p, expected %p\n", __func__, result, NULL);
    }
}

/**
 * @tc.name      : flockfile_0200
 * @tc.desc      : Shared FILE* to implements mutual exclusion
 * @tc.level     : Level 1
 */
void flockfile_0200(void)
{
    pthread_t tid[100];

    fp = fopen(path, "w+");
    if (fp == NULL) {
        t_error("%s fopen failed\n", __func__);
    }

    for (int i = 0; i < 100; i++) {
        if (pthread_create(tid + i, NULL, do_child_two, NULL) != 0) {
            t_error("%s pthread_create failed\n", __func__);
        }
    }

    for (int j = 0; j < 100; j++) {
        if (pthread_join(tid[j], NULL) != 0) {
            t_error("%s pthread_join failed\n", __func__);
        }
    }

    fclose(fp);
    remove(path);
}

int main(int argc, char *argv[])
{
    flockfile_0100();
    flockfile_0200();
    return t_status;
}