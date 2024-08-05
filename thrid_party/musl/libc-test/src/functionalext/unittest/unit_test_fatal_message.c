/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <info/fatal_message.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <test.h>
#include <pthread.h>
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)(void);
static const int WAIT_TIME = 1;

/**
 * @tc.name      : set_fatal_message
 * @tc.desc      : Test the function of set_fatal_message.
 * @tc.level     : Level 0
 */
static void fatal_message_0010(void)
{
    const char msg[1024] = {"abcdefghijklmnopqrstuvwxyz1234567890"};
    fatal_msg_t *fatal_message = NULL;
    int pidChild = 0;
    pid_t fpid;
    fpid = fork();
    if (fpid < 0) {
        t_printf("error in fork!");
    } else if (fpid == 0) {
        pidChild = getpid();
        set_fatal_message(msg);
        fatal_message = get_fatal_message();
        EXPECT_EQ("fatal_message_0010", fatal_message, NULL);
        exit(pidChild);
    }
}

TEST_FUN G_Fun_Array[] = {
    fatal_message_0010,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
