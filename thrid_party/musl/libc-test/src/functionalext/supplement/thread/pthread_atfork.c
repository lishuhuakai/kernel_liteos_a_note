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

#include <pthread.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "functionalext.h"

#define TEST_THREAD_SLEEP 3

pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;
static int gprepare_flag = 0;
static int gparent_flag = 0;
static int gchild_flag = 0;

void reset_data(void)
{
    gprepare_flag = 0;
    gparent_flag = 0;
    gchild_flag = 0;
}

static void prepare(void)
{
    int ret = pthread_mutex_lock(&glock);
    EXPECT_EQ("pthread_atfork prepare", ret, 0);
    gprepare_flag++;
}

static void parent(void)
{
    int ret = pthread_mutex_unlock(&glock);
    EXPECT_EQ("pthread_atfork prepare", ret, 0);
    gparent_flag++;
}

static void child(void)
{
    int ret = pthread_mutex_unlock(&glock);
    EXPECT_EQ("pthread_atfork prepare", ret, 0);
    gchild_flag++;
}

static void *thread_fun(void *arg)
{
#ifdef FEATURE_PTHREAD_CANCEL
    while (1) {
        pthread_testcancel();
        pthread_mutex_lock(&glock);
        sleep(TEST_THREAD_SLEEP);
        pthread_mutex_unlock(&glock);
        sleep(1);
    }
#else
    pthread_mutex_lock(&glock);
    sleep(TEST_THREAD_SLEEP);
    pthread_mutex_unlock(&glock);
    sleep(1);
#endif
    return NULL;
}

/**
 * @tc.name      : pthread_atfork_0100
 * @tc.desc      : Provide a callback to test the relationship between pthread_atfork and fork
 * @tc.level     : Level 1
 */
void pthread_atfork_0100(void)
{
    reset_data();
#ifdef FEATURE_PTHREAD_CANCEL
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
#endif
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    EXPECT_EQ("pthread_atfork_0100", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }

    ret = pthread_atfork(prepare, parent, child);
    EXPECT_EQ("pthread_atfork_0100", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }
    sleep(1);

    pid_t pid = fork();
    if (pid == 0) {
        pthread_mutex_lock(&glock);
        sleep(1);
        pthread_mutex_unlock(&glock);
        EXPECT_EQ("pthread_atfork_0100", gchild_flag, 1);
        _exit(0);
    } else if (pid > 0) {
#ifdef FEATURE_PTHREAD_CANCEL
        wait(NULL);
        pthread_cancel(tid);
#endif
        pthread_join(tid, NULL);

        EXPECT_EQ("pthread_atfork_0100", gprepare_flag, 1);
        EXPECT_EQ("pthread_atfork_0100", gparent_flag, 1);
    } else {
        EXPECT_FALSE("pthread_atfork_0100", 1);
    }
}

/**
 * @tc.name      : pthread_atfork_0200
 * @tc.desc      : No callback is provided, test the relationship between pthread_atfork and fork
 * @tc.level     : Level 2
 */
void pthread_atfork_0200(void)
{
    reset_data();
    int ret = pthread_atfork(NULL, NULL, NULL);
    EXPECT_EQ("pthread_atfork_0200", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        EXPECT_EQ("pthread_atfork_0200", gchild_flag, 0);
        _exit(0);
    } else if (pid > 0) {
        wait(NULL);

        EXPECT_EQ("pthread_atfork_0200", gprepare_flag, 0);
        EXPECT_EQ("pthread_atfork_0200", gparent_flag, 0);
    } else {
        EXPECT_FALSE("pthread_atfork_0200", 1);
    }
}

int main(void)
{
    pthread_atfork_0200();
    pthread_atfork_0100();
    return t_status;
}