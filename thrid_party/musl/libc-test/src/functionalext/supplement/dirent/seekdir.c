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

#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

const char *path = "/data";

void handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : seekdir_0100
 * @tc.desc      : set the position of the next readdir() call in the directory stream
 * @tc.level     : Level 0
 */
void seekdir_0100(void)
{
    DIR *dir = opendir(path);
    if (dir == NULL) {
        t_error("%s failed: opendir. path = %s\n", __func__, path);
    }

    int counter = 0;
    struct dirent *result;
    long offset = 0;
    while ((result = readdir(dir)) != NULL) {
        counter++;

        if (counter > 1) {
            offset = telldir(dir);
            break;
        }
    }

    result = readdir(dir);
    if (result == NULL) {
        t_error("%s failed: readdir\n", __func__);
    }

    char d_name[BUFSIZ] = {0};
    strcpy(d_name, result->d_name);

    errno = 0;
    seekdir(dir, offset);
    if (errno != 0) {
        t_error("%s failed: seekdir. errno = %d\n", __func__, errno);
    }

    result = readdir(dir);
    if (result == NULL) {
        t_error("%s failed: readdir\n", __func__);
    }

    if (strcmp(result->d_name, d_name)) {
        t_error("%s failed: strcmp. result->d_name = %s, d_name = %s\n", __func__, result->d_name, d_name);
    }

    closedir(dir);
}

/**
 * @tc.name      : seekdir_0200
 * @tc.desc      : set the position of the next readdir() call in the directory stream to zero
 * @tc.level     : Level 1
 */
void seekdir_0200(void)
{
    DIR *dir = opendir(path);
    if (dir == NULL) {
        t_error("%s failed: opendir. path = %s\n", __func__, path);
    }

    struct dirent *result = readdir(dir);
    if (result == NULL) {
        t_error("%s failed: readdir\n", __func__);
    }

    char d_name[BUFSIZ] = {0};
    strcpy(d_name, result->d_name);

    errno = 0;
    seekdir(dir, 0);
    if (errno != 0) {
        t_error("%s failed: seekdir. errno = %d\n", __func__, errno);
    }

    result = readdir(dir);
    if (result == NULL) {
        t_error("%s failed: readdir\n", __func__);
    }

    if (strcmp(result->d_name, d_name)) {
        t_error("%s failed: strcmp. result->d_name = %s, d_name = %s\n", __func__, result->d_name, d_name);
    }

    closedir(dir);
}

/**
 * @tc.name      : seekdir_0300
 * @tc.desc      : set the position of the next readdir() call in the directory stream with NULL
 * @tc.level     : Level 2
 */
void seekdir_0300(void)
{
    signal(SIGSEGV, handler);

    DIR *dir = NULL;
    seekdir(dir, -1);
}

int main(int argc, char *argv[])
{
    seekdir_0100();
    seekdir_0200();
    seekdir_0300();

    return t_status;
}
