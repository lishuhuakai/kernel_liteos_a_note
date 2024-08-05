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
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "filepath_util.h"
#include "functionalext.h"

void handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : readdir_r_0100
 * @tc.desc      : read a directory
 * @tc.level     : Level 0
 */
void readdir_r_0100(void)
{
    char name[PATH_MAX] = {0};
    FILE_ABSOLUTE_DIR(name);
    DIR *dir = opendir(name);
    if (dir == NULL) {
        t_error("%s failed: opendir. name = %s\n", __func__, name);
        return;
    }

    struct dirent buf;
    struct dirent *res;
    int result = readdir_r(dir, &buf, &res);
    if (result != 0) {
        t_error("%s failed: readdir_r. result = %s\n", __func__, result);
        return;
    }
}

/**
 * @tc.name      : readdir_r_0200
 * @tc.desc      : read a directory with a NULL dir
 * @tc.level     : Level 2
 */
void readdir_r_0200(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("readdir_0100: Error forking process");
    } else if (pid == 0) {
        DIR *dir = NULL;
        struct dirent buf;
        struct dirent *res;
        readdir_r(dir, &buf, &res);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("readdir_0100", SIGABRT, sig);
        }
    }
}

int main(int argc, char *argv[])
{
    readdir_r_0100();
    readdir_r_0200();

    return t_status;
}
