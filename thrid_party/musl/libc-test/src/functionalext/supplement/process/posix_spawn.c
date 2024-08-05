/**
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

#include <spawn.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : posix_spawn_0100
 * @tc.desc      : Use the default process attributes, create a subprocess and
 *                 execute shell commands, which are input from outside
 * @tc.level     : Level 0
 */
void posix_spawn_0100(char *cmd)
{
    pid_t pid;
    char *argv[] = {"sh", "-c", cmd, NULL};
    int ret = posix_spawn(&pid, "/bin/sh", NULL, NULL, argv, NULL);
    EXPECT_EQ("posix_spawn_0100", ret, CMPFLAG);
    if (ret != 0) {
        t_error("posix_spawn create failed\n");
        return;
    }
    wait(NULL);
}

/**
 * @tc.name      : posix_spawn_0200
 * @tc.desc      : Add SIGCHLD to the signal set, create a subprocess and
 *                 execute shell commands, which are input from outside
 * @tc.level     : Level 0
 */
void posix_spawn_0200(char *cmd)
{
    pid_t pid;
    sigset_t def;
    posix_spawnattr_t attr;
    char *argv[] = {"sh", "-c", cmd, NULL};
    int ret = posix_spawnattr_init(&attr);

    ret = sigemptyset(&def);
    ret = sigaddset(&def, SIGCHLD);
    if (ret == ERREXPECT) {
        EXPECT_NE("posix_spawn_0200", ret, ERREXPECT);
        return;
    }

    ret = posix_spawnattr_setsigdefault(&attr, &def);
    posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSIGMASK);

    ret = posix_spawn(&pid, "/bin/sh", NULL, &attr, argv, NULL);
    EXPECT_EQ("posix_spawn_0200", ret, CMPFLAG);
    if (ret != 0) {
        t_error("posix_spawn create failed\n");
        return;
    }

    wait(NULL);
    posix_spawnattr_destroy(&attr);
}

/**
 * @tc.name      : posix_spawn_0300
 * @tc.desc      : Set process attributes, create subprocesses and execute shell
 *                 commands, which are input externally
 * @tc.level     : Level 0
 */
void posix_spawn_0300(char *cmd)
{
    pid_t pid;
    sigset_t def;
    posix_spawnattr_t attr;
    char *argv[] = {"sh", "-c", cmd, NULL};
    int ret = posix_spawnattr_init(&attr);

    posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSIGDEF);

    ret = posix_spawn(&pid, "/bin/sh", NULL, &attr, argv, NULL);
    EXPECT_EQ("posix_spawn_0300", ret, CMPFLAG);
    if (ret != 0) {
        t_error("posix_spawn create failed\n");
        return;
    }

    wait(NULL);
    posix_spawnattr_destroy(&attr);
}

/**
 * @tc.name      : posix_spawn_0400
 * @tc.desc      : Create a child process and execute a non-existing file in the current
 *                 directory, the process creation fails
 * @tc.level     : Level 2
 */
void posix_spawn_0400(void)
{
    pid_t pid;

    int ret = posix_spawn(&pid, "unexitfile", NULL, NULL, NULL, NULL);
    EXPECT_NE("posix_spawn_0400", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }
    waitpid(pid, NULL, WNOHANG);
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        posix_spawn_0100(argv[1]);
        posix_spawn_0200(argv[1]);
        posix_spawn_0300(argv[1]);
        posix_spawn_0400();
    }
    return t_status;
}
