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

#include <stdlib.h>
#include <sys/wait.h>
#include "functionalext.h"

/**
 * @tc.name      : putenv_0100
 * @tc.desc      : Add or change the environment variable test
 * @tc.level     : Level 0
 */
void putenv_0100(void)
{
    int ret = unsetenv("test");
    EXPECT_EQ("putenv_0100", ret, CMPFLAG);

    ret = putenv("test=putenv_0100");
    EXPECT_EQ("putenv_0100", ret, CMPFLAG);
    char *test = getenv("test");
    if (test) {
        EXPECT_STREQ("putenv_0100", test, "putenv_0100");
    } else {
        EXPECT_PTRNE("putenv_0100", test, NULL);
    }

    ret = putenv("test=putenv_new");
    EXPECT_EQ("putenv_0100", ret, CMPFLAG);
    test = getenv("test");
    EXPECT_PTRNE("putenv_0100", test, NULL);
    if (test) {
        EXPECT_STREQ("putenv_0100", test, "putenv_new");
    }
    unsetenv("test");
}

/**
 * @tc.name      : putenv_0200
 * @tc.desc      : Provide exception parameters, add or modify the environment variable test
 * @tc.level     : Level 2
 */
void putenv_0200(void)
{
    int ret = unsetenv("test");
    EXPECT_EQ("putenv_0200", ret, CMPFLAG);

    ret = putenv("test");
    EXPECT_EQ("putenv_0200", ret, CMPFLAG);
    char *test = getenv("test");
    if (!test) {
        EXPECT_PTREQ("putenv_0200", test, NULL);
    }
    pid_t pid = fork();
    if (pid == -1) {
        t_error("unsetenv_0100: Error forking process");
    } else if (pid == 0) {
        putenv("");
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            EXPECT_EQ("putenv_0200", SIGABRT, sig);
        }
    }
    unsetenv("test");
}

int main(void)
{
    putenv_0100();
    putenv_0200();
    return t_status;
}