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

#include <signal.h>
#include <stdlib.h>
#include <threads.h>
#include "test.h"
#include <sys/wait.h>
#include <pthread.h>
#include "functionalext.h"

static int count = 0;

void exception_handler(int sig)
{
    exit(t_status);
}

int threadfunc(void *arg)
{
    count++;
    return 0;
}

/**
 * @tc.name      : thrd_join_0100
 * @tc.desc      : Blocks current thread until it identified by thr finishes execution with no return value
 * @tc.level     : Level 0
 */
void thrd_join_0100(void)
{
    thrd_t id;
    int result;
    result = thrd_create(&id, threadfunc, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_sleep(&(struct timespec){.tv_nsec = 1000000}, NULL);
    if (result != 0) {
        t_error("%s thrd_sleep failed", __func__);
    }

    result = thrd_join(id, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    if (count != 1) {
        t_error("%s failed, count is %d", __func__, count);
    }
    count = 0;
}

/**
 * @tc.name      : thrd_join_0200
 * @tc.desc      : Blocks current thread until it identified by thr finishes execution with return value
 * @tc.level     : Level 1
 */
void thrd_join_0200(void)
{
    thrd_t id;
    int ret = 1;
    int result;
    result = thrd_create(&id, threadfunc, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_sleep(&(struct timespec){.tv_nsec = 1000000}, NULL);
    if (result != 0) {
        t_error("%s thrd_sleep failed", __func__);
    }

    result = thrd_join(id, &ret);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    if (ret != 0) {
        t_error("%s return value failed", __func__);
    }

    if (count != 1) {
        t_error("%s failed, count is %d", __func__, count);
    }
    count = 0;
}

/**
 * @tc.name      : thrd_join_0300
 * @tc.desc      : Invalid parameter test
 * @tc.level     : Level 2
 */
void thrd_join_0300(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("thrd_join_0300: Error forking process");
    } else if (pid == 0) {
        thrd_join(NULL, NULL);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("thrd_join_0300", SIGABRT, sig);
        }
    }
}

int main(int argc, char *argv[])
{
    thrd_join_0100();
    thrd_join_0200();
    thrd_join_0300();
    return t_status;
}