/*
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

#include <malloc.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include "functionalext.h"

const int STACK_SIZE = 1024 * 8192;
const int SUCCESS = 0;
void *test(void *p)
{
    return NULL;
}

/**
 * @tc.name      : clone_0100
 * @tc.desc      : Each parameter is valid, and a new thread can be created through the clone function.
 * @tc.level     : Level 0
 */
void clone_0100(void)
{
    void *stack = malloc(STACK_SIZE);
    int cpid = -1;
    cpid = clone((int (*)(void *))test, (char *)stack + STACK_SIZE, CLONE_VM | CLONE_FS | CLONE_FILES | SIGCHLD, NULL);
    sleep(1);
    EXPECT_NE("clone_0100", cpid, -1);
}

/**
 * @tc.name      : clone_0200
 * @tc.desc      : Parameter flags is 0.
 * @tc.level     : Level 2
 */
void clone_0200(void)
{
    void *stack = malloc(STACK_SIZE);
    int cpid = -1;
    cpid = clone((int (*)(void *))test, (char *)stack + STACK_SIZE, 0, NULL);
    sleep(1);
    EXPECT_NE("clone_0200", cpid, -1);
}

/**
 * @tc.name      : clone_0300
 * @tc.desc      : Parameter stack is NULL.
 * @tc.level     : Level 2
 */
void clone_0300(void)
{
    int cpid = -1;
    cpid = clone((int (*)(void *))test, NULL, 0, NULL);
    sleep(1);
    EXPECT_NE("clone_0300", cpid, -1);
}

int main(int argc, char *argv[])
{
    clone_0100();
    clone_0200();
    clone_0300();
    return t_status;
}
