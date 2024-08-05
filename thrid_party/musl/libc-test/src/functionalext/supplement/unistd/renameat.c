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
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : renameat_0100
 * @tc.desc      : When newname already exists, test the renameat function
 * @tc.level     : Level 0
 */
void renameat_0100(void)
{
    int oldfd, newfd;
    char oldPath[] = "/data/renameat.txt";
    char newPath[] = "/data/newrenameat.txt";
    const char *msg = "test code";
    int len = strlen(msg);
    char buf[1024] = {0};

    if ((oldfd = creat(oldPath, S_IWUSR)) < 0) {
        t_error("%s creat oldfd failed\n", __func__);
    }
    if ((newfd = creat(newPath, S_IWUSR)) < 0) {
        t_error("%s creat newfd failed\n", __func__);
    }
    close(oldfd);
    close(newfd);
    oldfd = open(oldPath, O_RDWR);
    int wresult = write(oldfd, msg, len);
    if (wresult != len) {
        t_error("%s write get result is %d not want %d\n", __func__, wresult, len);
    }
    close(oldfd);
    if (renameat(oldfd, oldPath, newfd, newPath) == -1) {
        t_error("%s renameat failed\n", __func__);
        return;
    }
    newfd = open(newPath, O_RDWR);
    int bytes = read(newfd, buf, len);
    if (bytes == -1) {
        t_error("%s read file failed\n", __func__);
        return;
    }
    if (strcmp(msg, buf)) {
        t_error("%s wrong string written to file\n", __func__);
        return;
    }
    close(newfd);
    remove(newPath);
}

int main(int argc, char *argv[])
{
    renameat_0100();
    return t_status;
}