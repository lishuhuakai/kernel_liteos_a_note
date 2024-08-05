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

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <utime.h>
#include "test.h"

extern int __utime64(const char *, const struct utimbuf *);

/**
 * @tc.name      : utime_0100
 * @tc.desc      : Specify file modification and access times
 * @tc.level     : Level 0
 */
void utime_0100(void)
{
    int fd;
    char file[] = "/data/utime1.txt";
    struct utimbuf ubuf;
    struct stat info;

    if ((fd = creat(file, S_IWUSR)) < 0) {
        t_error("%s creat failed", __func__);
    } else {
        close(fd);
        stat(file, &info);
        ubuf.modtime = 0;
        time(&ubuf.actime);
        if (utime(file, &ubuf) != 0) {
            t_error("%s utime failed", __func__);
        } else {
            stat(file, &info);
            if (info.st_mtim.tv_sec != 0) {
                t_error("%s modify file time failed", __func__);
            }
        }
    }
    remove(file);
}

/**
 * @tc.name      : utime_0200
 * @tc.desc      : When times is a NULL pointer, the access and modification times are set to the current time
 * @tc.level     : Level 1
 */
void utime_0200(void)
{
    const char *path = "/data/utime.txt";
    int fd = open(path, O_RDWR | O_RSYNC | O_CREAT, 0664);
    if (fd == -1) {
        t_error("%s write create file error", __func__);
        return;
    }
    close(fd);
    struct stat buf1;
    struct stat buf2;
    time_t t_mold, t_aold, t_now, t_mnew, t_anew;
    stat(path, &buf1);

    t_mold = buf1.st_mtime;
    t_aold = buf1.st_atime;
    t_now = time(NULL);
    if (utime(path, NULL) != 0) {
        stat(path, &buf2);
        t_mnew = buf2.st_mtime;
        t_anew = buf2.st_atime;
        if (t_mnew != t_now && t_anew != t_now) {
            t_error("%s utime failed", __func__);
        }
    }
    remove(path);
}

/**
 * @tc.name      : utime64_0100
 * @tc.desc      : Specify file modification and access times
 * @tc.level     : Level 0
 */
void utime64_0100(void)
{
    int fd;
    char file[] = "/data/utime641.txt";
    struct utimbuf ubuf;
    struct stat info;

    if ((fd = creat(file, S_IWUSR)) < 0) {
        t_error("%s creat failed", __func__);
    } else {
        close(fd);
        stat(file, &info);
        ubuf.modtime = 0;
        time(&ubuf.actime);
        if (__utime64(file, &ubuf) != 0) {
            t_error("%s __utime64 failed", __func__);
        } else {
            stat(file, &info);
            if (info.st_mtim.tv_sec != 0) {
                t_error("%s modify file time failed", __func__);
            }
        }
    }
    remove(file);
}

int main(int argc, char *argv[])
{
    utime_0100();
    utime_0200();
    utime64_0100();
    return t_status;
}