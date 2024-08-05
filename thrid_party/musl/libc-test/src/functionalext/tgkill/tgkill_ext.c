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
#include <pthread.h>
#include <signal.h>
#include <sys/tgkill.h>
#include "test.h"

#define TEST(c, ...) ((c) || (t_error(#c " failed: " __VA_ARGS__),0))

#define SLEEP_50_MS (50*1000)

static void *tgkillTest(void *arg)
{
    pid_t *threadPid = (pid_t *)arg;
    *threadPid = getpid();
    return arg;
}

static void SignalHandler(int signum)
{
    return;
}

/**
 * @tc.name     : tgkill_0010
 * @tc.desc     : test tgkill by sending SIGCONT sig in the same thread
 * @tc.level    : Level 0
 */
static void tgkill_0010(void)
{
    TEST(tgkill(getpgid(getpid()), getpid(), SIGCONT) == 0);
}

/**
 * @tc.name     : tgkill_0020
 * @tc.desc     : test tgkill by sending SIGUSR1 sig in a different thread
 * @tc.level    : Level 0
 */
static void tgkill_0020(void)
{
    pid_t threadPid;
    pthread_t tid;
	struct sigaction sigusr1 = {
		.sa_handler = SignalHandler,
	};
    sigaction(SIGUSR1, &sigusr1, NULL);

    TEST(pthread_create(&tid, NULL, tgkillTest, (void*)&threadPid) == 0);
    usleep(SLEEP_50_MS);
    TEST(tgkill(getpgid(threadPid), threadPid, SIGUSR1) == 0);
    TEST(pthread_join(tid, NULL) == 0);
}

int main(void)
{
    tgkill_0010();
    tgkill_0020();

	return t_status;
}