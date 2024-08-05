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

#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include "functionalext.h"

#define TEST_FD_SIZE 2
#define TEST_BUFFER_SIZE 2

static void pipe2_test(const char *msg, int flag)
{
    int pipefd[TEST_FD_SIZE];
    char buf[TEST_BUFFER_SIZE];
    int ret = pipe2(pipefd, flag);
    EXPECT_EQ(msg, ret, CMPFLAG);
    if (ret != 0) {
        return;
    }

    memset(buf, 0x0, sizeof(buf));
    pid_t pid = fork();
    if (pid == 0) {
        close(pipefd[1]);
        while (read(pipefd[0], buf, 1) > 0) {
            EXPECT_STREQ(msg, buf, "a");
        }
        close(pipefd[0]);
        _exit(0);
    } else if (pid > 0) {
        buf[0] = 'a';
        close(pipefd[0]);
        write(pipefd[1], buf, 1);
        close(pipefd[1]);
        wait(NULL);
    } else {
        close(pipefd[0]);
        close(pipefd[1]);
        EXPECT_FALSE(msg, 1);
    }
}

/**
 * @tc.name      : pipe2_0100
 * @tc.desc      : Create pipelines in two ways: O_CLOEXEC and O_NONBLOCK, and implement data transfer
 * @tc.level     : Level 0
 */
void pipe2_0100(void)
{
    pipe2_test("pipe2_0100", 0);
    pipe2_test("pipe2_0100", O_CLOEXEC);
    pipe2_test("pipe2_0100", O_NONBLOCK);
}

int main(void)
{
    pipe2_0100();
    return t_status;
}