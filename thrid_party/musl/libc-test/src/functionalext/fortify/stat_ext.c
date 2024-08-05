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
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sigchain.h>
#include "fortify_test.h"
#include "test.h"

/**
 * @tc.name     : umask_0010
 * @tc.desc     : test umask normal condition
 * @tc.level    : Level 0
 */
static void umask_0010(void)
{
    mode_t mask = atoi("077");
    umask(mask);

    return;
}

/**
 * @tc.name     : umask_0020
 * @tc.desc     : test umask suppress compiler optimizations
 * @tc.level    : Level 2
 */
static void umask_0020(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    mode_t mask = atoi("1023");  // 01777 in octal
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            umask(mask);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }

    return;
}

int main(int argc, char *argv[]) {
    remove_all_special_handler(SIGABRT);
    umask_0010();
    umask_0020();

    return t_status;
}