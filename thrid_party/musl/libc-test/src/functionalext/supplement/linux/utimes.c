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

#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include "test.h"

extern int __utimes_time64(const char *, const struct timeval [2]);

/**
 * @tc.name      : utimes_0100
 * @tc.desc      : Change file last access and modification times
 * @tc.level     : Level 0
 */
void utimes_0100(void)
{
    const char *path = "/data/utimes.txt";
    int fd = open(path, O_RDWR | O_RSYNC | O_CREAT, 0664);
    if (fd == -1) {
        t_error("%s write create file error", __func__);
        return;
    }
    close(fd);
    struct stat buf1;
    struct stat buf2;
    time_t t_mold, t_aold, t_now, t_new;
    stat(path, &buf1);

    t_mold = buf1.st_mtime;
    t_aold = buf1.st_atime;
    t_now = time(NULL);
    if (utimes(path, NULL) != 0) {
        t_error("%s utimes failed", __func__);
    } else {
        stat(path, &buf2);
        t_new = buf2.st_mtime;
        if (t_new != t_now) {
            t_error("%s utimes failed", __func__);
        }
    }
    remove(path);
}

/**
 * @tc.name      : utimes_0200
 * @tc.desc      : Specify time to change file last access and modification time
 * @tc.level     : Level 1
 */
void utimes_0200(void)
{
    const char *path = "/data/utimes.txt";
    int fd = open(path, O_RDWR | O_RSYNC | O_CREAT, 0664);
    if (fd == -1) {
        t_error("%s write create file error", __func__);
        return;
    }
    close(fd);
    struct stat st;
    struct timeval tv[2] = {{1, 0}, {1, 0}};

    int result = utimes(path, tv);
    if (result != 0) {
        t_error("%s utimes failed", __func__);
    } else {
        stat(path, &st);
        if (st.st_atime != tv[0].tv_sec && st.st_mtime != tv[1].tv_sec) {
            t_error("%s utimes failed", __func__);
        }
    }
    remove(path);
}

/**
 * @tc.name      : utimes_time64_0100
 * @tc.desc      : Change file last access and modification times
 * @tc.level     : Level 0
 */
void utimes_time64_0100(void)
{
    const char *path = "/data/utimes_time64.txt";
    int fd = open(path, O_RDWR | O_RSYNC | O_CREAT, 0664);
    if (fd == -1) {
        t_error("%s write create file error", __func__);
        return;
    }
    close(fd);
    struct stat buf1;
    struct stat buf2;
    time_t t_mold, t_aold, t_now, t_new;
    stat(path, &buf1);

    t_mold = buf1.st_mtime;
    t_aold = buf1.st_atime;
    t_now = time(NULL);
    if (__utimes_time64(path, NULL) != 0) {
        t_error("%s __utimes_time64 failed", __func__);
    } else {
        stat(path, &buf2);
        t_new = buf2.st_mtime;
        if (t_new != t_now) {
            t_error("%s __utimes_time64 failed", __func__);
        }
    }
    remove(path);
}

int main(int argc, char *argv[])
{
    utimes_0100();
    utimes_0200();
    utimes_time64_0100();
    return t_status;
}