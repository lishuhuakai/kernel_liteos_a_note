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

const char *path = "/data/test.txt";

void *child_func(void *p)
{
    char buf[64];

    FILE *filep = fopen(path, "r+");
    if (filep == NULL) {
        t_error("%s fopen failed\n", __func__);
    }

    flockfile(filep);

    if (fseek(filep, 0L, SEEK_SET) == -1) {
        t_error("%s fseek failed\n", __func__);
    }
    int ret = fread(buf, 64, 1, filep);

    int count = atoi(buf);
    ++count;

    sprintf(buf, "%d", count);
    if (fseek(filep, 0L, SEEK_SET) == -1) {
        t_error("%s fseek failed\n", __func__);
    }
    ret = fwrite(buf, strlen(buf), 1, filep);

    funlockfile(filep);

    fclose(filep);
    return NULL;
}

/**
 * @tc.name      : funlockfile_0100
 * @tc.desc      : Mutual exclusion has been tested in the test case of flockfile, so this test case tests that threads
 *                 cannot be mutually exclusive for the case where each thread fopens a descriptor
 * @tc.level     : Level 0
 */
void funlockfile_0100(void)
{
    pthread_t tid[100];

    for (int i = 0; i < 100; i++) {
        if (pthread_create(tid + i, NULL, child_func, NULL) != 0) {
            t_error("%s pthread_create failed\n", __func__);
        }
    }

    for (int j = 0; j < 100; j++) {
        if (pthread_join(tid[j], NULL) != 0) {
            t_error("%s pthread_join failed\n", __func__);
        }
    }
}

int main(int argc, char *argv[])
{
    FILE *fp = fopen(path, "w+");
    if (fp == NULL) {
        t_error("%s fopen failed\n", __func__);
    }
    fclose(fp);

    funlockfile_0100();

    remove(path);
    return t_status;
}