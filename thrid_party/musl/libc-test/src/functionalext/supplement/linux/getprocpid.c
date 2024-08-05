/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <malloc.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "functionalext.h"

/**
 * @tc.name      : getprocpid_0100
 * @tc.desc      : Get the pid by parsing the proc information
 * @tc.level     : Level 0
 */
void getprocpid_0100(void)
{
    pid_t pid1 = getprocpid();
    EXPECT_EQ("getprocpid_0100", pid1 > 1, true);
    pid_t pid2 = getprocpid();
    EXPECT_EQ("getprocpid_0100", pid1, pid2);
    EXPECT_EQ("getprocpid_0100", pid1, getpid());
}

/**
 * @tc.name      : getprocpid_0200
 * @tc.desc      : Get the pid by parsing the proc information
 * @tc.level     : Level 0
 */
void getprocpid_0200(void)
{
    pid_t pid1 = getprocpid();
    EXPECT_EQ("getprocpid_0200", pid1, getpid());

    pid_t child = fork();
    if (child == 0) {
        pid_t pid2 = getprocpid();
        if (pid2 != getpid()) {
            exit(1);
        }
        exit(0);
    }
    EXPECT_EQ("getprocpid_0200", child > 0, true);

    int status = 0;
    int ret = waitpid(child, &status, 0);
    EXPECT_EQ("getprocpid_0200", ret, child);
    EXPECT_EQ("getprocpid_0200", WIFEXITED(status), true);
}

static int child_func(void *arg)
{
    pid_t parent = *(pid_t *)arg;
    if (getpid() != 1) {
        return ESRCH;
    }

    pid_t pid = getprocpid();
    if (pid == parent) {
        return ENOSYS;
    }
    return 0;
}

/**
 * @tc.name      : getprocpid_0300
 * @tc.desc      : Get the pid by parsing the proc information
 * @tc.level     : Level 0
 */
void getprocpid_0300(void)
{
    pid_t pid1 = getprocpid();
    EXPECT_EQ("getprocpid_0300", pid1, getpid());

    pid_t child = clone(child_func, NULL, CLONE_NEWPID | SIGCHLD, &pid1);
    EXPECT_EQ("getprocpid_0300", child > 0, true);

    int status = 0;
    int ret = waitpid(child, &status, 0);
    EXPECT_EQ("getprocpid_0300", ret, child);
    EXPECT_EQ("getprocpid_0300", WIFEXITED(status), true);
}

static void *pthread_func(void *arg)
{
    pid_t parent = *(pid_t *)arg;
    pid_t pid1 = getproctid();
    if (parent == pid1) {
        return (void *)ESRCH;
    }
    pid_t pid2 = gettid();
    if (pid1 != pid2) {
        return (void *)ENOSYS;
    }
    return NULL;
}

/**
 * @tc.name      : getproctid_0100
 * @tc.desc      : Get the tid by parsing the proc information
 * @tc.level     : Level 0
 */
void getproctid_0100(void)
{
    pid_t pid1 = getproctid();
    EXPECT_EQ("getproctid_0100", pid1 > 1, true);
    EXPECT_EQ("getproctid_0100", pid1, gettid());

    void *retVal = NULL;
    pthread_t thread;
    int ret = pthread_create(&thread, NULL, pthread_func, &pid1);
    EXPECT_EQ("getproctid_0100", ret, 0);

    ret = pthread_join(thread, &retVal);
    EXPECT_EQ("getproctid_0100", ret, 0);
    EXPECT_EQ("getproctid_0100", retVal, NULL);
}

/**
 * @tc.name      : getproctid_0200
 * @tc.desc      : Get the tid by parsing the proc information
 * @tc.level     : Level 0
 */
void getproctid_0200(void)
{
    pid_t pid1 = getproctid();
    EXPECT_EQ("getproctid_0200", pid1, gettid());

    pid_t child = fork();
    if (child == 0) {
        pid_t pid2 = getproctid();
        if (pid2 != gettid()) {
            exit(1);
        }
        exit(0);
    }

    EXPECT_EQ("getproctid_0200", child > 0, true);

    int status = 0;
    int ret = waitpid(child, &status, 0);
    EXPECT_EQ("getproctid_0200", ret, child);
    EXPECT_EQ("getproctid_0200", WIFEXITED(status), true);
}

static int child_func_tid(void *arg)
{
    pid_t parent = *(pid_t *)arg;
    if (getpid() != 1) {
        return ESRCH;
    }

    pid_t pid = getproctid();
    if (pid == parent) {
        return ENOSYS;
    }
    return 0;
}

/**
 * @tc.name      : getproctid_0300
 * @tc.desc      : Get the tid by parsing the proc information
 * @tc.level     : Level 0
 */
void getproctid_0300(void)
{
    pid_t pid1 = getproctid();
    EXPECT_EQ("getproctid_0300", pid1, gettid());

    pid_t child = clone(child_func_tid, NULL, CLONE_NEWPID | SIGCHLD, &pid1);
    EXPECT_EQ("getproctid_0300", child > 0, true);

    int status = 0;
    int ret = waitpid(child, &status, 0);
    EXPECT_EQ("getproctid_0300", ret, child);
    EXPECT_EQ("getproctid_0300", WIFEXITED(status), true);
}

int main(int argc, char *argv[])
{
    getprocpid_0100();
    getprocpid_0200();
    getprocpid_0300();
    getproctid_0100();
    getproctid_0200();
    getproctid_0300();
    return t_status;
}
